#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QTime>
#include <QTimer>
#include <QEvent>
#include <fstream>
#include "MainWindow.h"
#include "ClockWindow.h"
#include "SignalWindow.h"
#include "DoNotDisturbWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QLabel>

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::StatusTip) return true;
    return false;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    clocks = {};
    isClosestExists = false;
    currentIndex = 0;

    QTimer *clockTimer = new QTimer(this);
    connect(clockTimer, SIGNAL(timeout()), this, SLOT(counting()));

    doNotDisturbTimer = new QTimer(this);
    doNotDisturbTimer->setSingleShot(true);
    connect(doNotDisturbTimer, SIGNAL(timeout()), this, SLOT(updateDoNotDisturb_slot()));

    QStringList horizHeaders;
    horizHeaders << "№" << "Name" << "Type" << "Status" << "Value" << "End time" << "Time left" << "Repeating";
    ui->table->setColumnCount(horizHeaders.size());
    ui->table->setHorizontalHeaderLabels(horizHeaders);
    ui->table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->table->verticalHeader()->setDefaultSectionSize(20);   

    std::ifstream out;

    out.open("config.dat");
    if(out.is_open())
    {
        size_t width;
        int sortingCol;
        for(size_t i = 0; i<ui->table->horizontalHeader()->count(); i++) //read columns width
        {
            out >> width;
            ui->table->setColumnWidth(i, width);
        }
        out >> sortingCol; //read the sorting column number (or -1, if no sorting)
        if(sortingCol != -1)
        {
            int sortingOrder;
            out >> sortingOrder;
            if(sortingOrder == 1) ui->table->horizontalHeader()->setSortIndicator(sortingCol, Qt::SortOrder::DescendingOrder);
            else ui->table->horizontalHeader()->setSortIndicator(sortingCol, Qt::SortOrder::AscendingOrder);
        }
        out.close();
    }

    out.open("do_not_disturb.dat");
    if(out.is_open())
    {
        out >> statusDoNotDisturb;
        out >> isScheduledDoNotDisturb;
        out >> startDoNotDisturb;
        out >> endDoNotDisturb;
        out.close();
    }
    else
    {
        statusDoNotDisturb = 0;
        isScheduledDoNotDisturb = false;
        startDoNotDisturb = QTime(0,0,0);
        endDoNotDisturb = QTime(0,0,0);
    }
    updateDoNotDisturb();

    out.open("clocks.dat");
    if(out.is_open())
    {
        Clock clock(this);
        QString string = "";
        std::vector <size_t> finished;
        qint64 currTime = QDateTime::currentSecsSinceEpoch();
        while(out >> clock)
        {
            clocks.push_back(clock);
            if(clock.getStatus()!=0 && clock.getEndTime()<currTime)
            {
                finished.push_back(clock.getId());
                string += clock.getName()+"\n";
            }
        }
        this->show();
        updateTable();
        clockTimer->start(1000);
        if(string!="")
        {
            stopClocks(finished, true);
            for(size_t i = 0; i<finished.size(); i++)
            {
                if(!clocks[finished[i]].getRepeating())
                {
                    finished.erase(finished.begin()+i);
                    i--;
                }
            }
            startClocks(finished, true);
            QMessageBox msgBox(QMessageBox::Information, "", "While the program was offline, the following clocks finished:\n\n"+string);
            msgBox.setStyleSheet("QMessageBox QPushButton{"
                                    "background-color: rgb(220, 240, 255);}"
                                 "QMessageBox{"
                                    "background-color: rgb(129, 190, 255);}");
            msgBox.exec();
        }
        out.close();
    }
    else clockTimer->start(1000);
}

MainWindow::~MainWindow()
{
    std::ofstream in;

    in.open("config.dat", std::ios_base::trunc);
    if(in.is_open())
    {
        for(size_t i = 0; i<ui->table->horizontalHeader()->count(); i++) //write columns width
        {
            in << ui->table->horizontalHeader()->sectionSize(i) << " ";
        }
        if(ui->table->horizontalHeader()->sortIndicatorSection() != ui->table->horizontalHeader()->count()) //is sorted?
        {
            in << ui->table->horizontalHeader()->sortIndicatorSection() << " "; //write the sorting column number
            in << ui->table->horizontalHeader()->sortIndicatorOrder(); //write the sorting order
        }
        else in << -1;
        in.close();
    }

    in.open("do_not_disturb.dat", std::ios_base::trunc);
    if(in.is_open())
    {
        in << statusDoNotDisturb << " ";
        in << isScheduledDoNotDisturb << " ";
        in << startDoNotDisturb;
        in << endDoNotDisturb;
        in.close();
    }

    in.open("clocks.dat", std::ios_base::trunc);
    if(in.is_open())
    {
        for(size_t i = 0; i<clocks.size(); i++) //write clocks data
        {
            in << clocks[i];
            if(i != clocks.size()-1) in << "\n";
        }
        in.close();
    }

    delete ui;
}

short MainWindow::getStatusDoNotDisturb() const
{
    return statusDoNotDisturb;
}

bool MainWindow::getIsScheduledDoNotDisturb() const
{
    return isScheduledDoNotDisturb;
}

QTime MainWindow::getStartDoNotDisturb() const
{
    return startDoNotDisturb;
}

QTime MainWindow::getEndDoNotDisturb() const
{
    return endDoNotDisturb;
}

void MainWindow::setStatusDoNotDisturb(const short &status)
{
    statusDoNotDisturb = status;
}

void MainWindow::setIsScheduledDoNotDisturb(const bool& is)
{
    isScheduledDoNotDisturb = is;
}

void MainWindow::setStartDoNotDisturb(const QTime &time)
{
    startDoNotDisturb = time;
}

void MainWindow::setEndDoNotDisturb(const QTime &time)
{
    endDoNotDisturb = time;
}

void MainWindow::counting()
{
    this->setWindowTitle("SmartClock     " + QTime::currentTime().toString("hh:mm:ss"));
    QTime timeLeft;
    size_t index, secLeft;
    ui->table->setSortingEnabled(false);
    for(size_t i = 0; i<clocks.size(); i++)
    {
        index = (ui->table->item(i,0)->data(Qt::DisplayRole)).toULongLong()-1;
        if(clocks[index].getStatus()!=0)
        {
            secLeft = clocks[index].getEndTime() - QDateTime::currentSecsSinceEpoch();
            timeLeft = QTime(0,0,0).addSecs(secLeft);
            ui->table->item(i,6)->setText(timeLeft.toString("hh : mm : ss"));
            if(secLeft<=0)
            {
                stopClocks({index}, false);
                if(clocks[index].getRepeating())
                {
                    startClocks({index}, false);
                }

                auto signalWindow = new SignalWindow(this, &clocks[index]);
                signalWindow->setModal(true);
                signalWindow->setWindowTitle("Signal: "+clocks[index].getName());
                signalWindow->show();

                clocks[index].printToTable(i);
                continue;
            }
            if(!isClosestExists)
            {
                indexOfClosest = index;
                isClosestExists = true;
            }
            else if(clocks[index].getEndTime()<clocks[indexOfClosest].getEndTime())
            {
                indexOfClosest = index;
            }
        }
    }
    ui->table->setSortingEnabled(true);

    if(isClosestExists)
    {
        timeLeft = QTime(0,0,0).addSecs(clocks[indexOfClosest].getEndTime() - QDateTime::currentSecsSinceEpoch());
        statusBar()->showMessage(timeLeft.toString("hh : mm : ss - ")+clocks[indexOfClosest].getName());
    }
    else
    {
        statusBar()->showMessage("No active clocks");
    }
}

void MainWindow::updateDoNotDisturb_slot()
{
    updateDoNotDisturb();
}

void MainWindow::updateTable()
{
    ui->table->selectionModel()->clearSelection();
    bool sorting = ui->table->isSortingEnabled();
    ui->table->setSortingEnabled(false);

    size_t rows = ui->table->rowCount();
    while(rows > clocks.size())
    {
        ui->table->removeRow(rows-1);
        rows--;
    }
    while(rows < clocks.size())
    {
        ui->table->insertRow(rows);
        for(size_t k = 0; k<ui->table->horizontalHeader()->count(); k++)
        {
            ui->table->setItem(rows, k, new QTableWidgetItem());
        }
        rows++;
    }
    for(size_t i = 0; i<clocks.size(); i++)
    {
        clocks[i].printToTable(i);
    }

    ui->table->setSortingEnabled(sorting);
}

void MainWindow::updateDoNotDisturb()
{
    if(isScheduledDoNotDisturb && statusDoNotDisturb!=2)
    {
        qint64 currMSec = QTime::currentTime().msecsSinceStartOfDay();
        qint64 startMSec = startDoNotDisturb.msecsSinceStartOfDay();
        qint64 endMSec = endDoNotDisturb.msecsSinceStartOfDay();
        if(startMSec < endMSec)
        {
            if(currMSec < startMSec)
            {
                statusDoNotDisturb = 0;
                doNotDisturbTimer->start(startMSec-currMSec+1000);
            }
            else if(currMSec < endMSec)
            {
                statusDoNotDisturb = 1;
                doNotDisturbTimer->start(endMSec-currMSec+1000);
            }
            else
            {
                statusDoNotDisturb = 0;
                doNotDisturbTimer->start(86400*1000-currMSec+startMSec+1000);
            }
        }
        else
        {
            if(currMSec < endMSec)
            {
                statusDoNotDisturb = 1;
                doNotDisturbTimer->start(endMSec-currMSec+1000);
            }
            else if(currMSec < startMSec)
            {
                statusDoNotDisturb = 0;
                doNotDisturbTimer->start(startMSec-currMSec+1000);
            }
            else
            {
                statusDoNotDisturb = 1;
                doNotDisturbTimer->start(86400*1000-currMSec+endMSec+1000);
            }
        }

        qint64 a = doNotDisturbTimer->interval()/*+QTime::currentTime().msecsSinceStartOfDay()*/;
        qDebug() << QTime(0,0,0).addMSecs(a).toString("hh:mm:ss");
    }
    else if(!isScheduledDoNotDisturb && statusDoNotDisturb==1)
    {
        statusDoNotDisturb = 0;
        doNotDisturbTimer->stop();
    }
    updateDoNotDisturbIcon();
}

void MainWindow::updateDoNotDisturbIcon()
{
    if(statusDoNotDisturb==0) ui->actionDo_Not_Disturb->setIcon(QIcon(":/prefix1/images/do_not_disturb_off.png"));
    else if(statusDoNotDisturb==1) ui->actionDo_Not_Disturb->setIcon(QIcon(":/prefix1/images/do_not_disturb_scheduled.png"));
    else if(statusDoNotDisturb==2) ui->actionDo_Not_Disturb->setIcon(QIcon(":/prefix1/images/do_not_disturb_forced.png"));
}

std::vector<size_t> MainWindow::getSelected() const
{
    QModelIndexList selected = ui->table->selectionModel()->selectedRows();
    std::vector<size_t> indices;
    size_t index;
    for(size_t i = 0; i<selected.size(); i++)
    {
        index = (ui->table->item(selected[i].row(),0)->data(Qt::DisplayRole)).toULongLong()-1;
        indices.push_back(index);
    }
    std::sort(indices.begin(), indices.end());
    return indices;
}

void MainWindow::addNewClockWindow()
{
    auto clockWindow = new ClockWindow(this);
    clockWindow->setModal(true);
    clockWindow->setWindowTitle("New Clock");
    clockWindow->show();
}

void MainWindow::editClockWindow(const size_t& index)
{
    currentIndex = index;
    if(clocks[index].getStatus()==1 || clocks[index].getStatus()==2)
    {
        QMessageBox msgBox(QMessageBox::Critical, "Error", "You can't edit the active clock.");
        msgBox.setStyleSheet("QMessageBox QPushButton{"
                                "background-color: rgb(220, 240, 255);}"
                             "QMessageBox{"
                                "background-color: rgb(129, 190, 255);}");
        msgBox.exec();
        return;
    }
    auto clockWindow = new ClockWindow(this, &clocks[index]);
    clockWindow->setModal(true);
    clockWindow->setWindowTitle("Edit Clock");
    clockWindow->show();
}

void MainWindow::addNewClock(Clock *clock, const bool& updateTable)
{
    clock->setStatus(0);
    clock->setId(this->clocks.size());
    clock->setEndTime(0);
    this->clocks.push_back(*clock);
    delete clock;
    if(updateTable) this->updateTable();
}

void MainWindow::editClock(Clock* clock, const bool& updateTable)
{
    clock->setStatus(0);
    clock->setId(currentIndex);
    clocks[currentIndex] = *clock;
    delete clock;
    if(updateTable) this->updateTable();
}

void MainWindow::removeClocks(const std::vector<size_t>& indices, const bool& updateTable)
{
    bool removingClosest = false;
    QString string = "";
    for(size_t i = 0; i<indices.size(); i++)
    {
        if(clocks[indices[i]].getStatus()==1 || clocks[indices[i]].getStatus()==2)
        {
            string += clocks[indices[i]].getName()+"\n";
        }
        if(indices[i] == indexOfClosest)
        {
            removingClosest = true;
        }
    }
    if(string!="")
    {
        QMessageBox msgBox(QMessageBox::Question, "", "This action will remove the following active clocks:\n\n"+string+"\nAre you sure?", QMessageBox::Yes|QMessageBox::No);
        msgBox.setStyleSheet("QMessageBox QPushButton{"
                                "background-color: rgb(220, 240, 255);}"
                             "QMessageBox{"
                                "background-color: rgb(129, 190, 255);}");
        auto button = msgBox.exec();
        if(button == QMessageBox::No)
        {
            return;
        }
    }

    if(removingClosest) isClosestExists = false;
    for(size_t i = indices.size()-1; i<indices.size(); i--) //removing clocks from the vector
    {
        clocks.erase(clocks.begin()+indices[i]);
    }
    for(size_t i = clocks.size()-1; i<clocks.size(); i--) //recovering numeration in the vector
    {
        clocks[i].setId(i);
    }
    if(updateTable) this->updateTable();
}

void MainWindow::startClocks(const std::vector<size_t>& indices, const bool& updateTable)
{
    std::vector<size_t> copy = indices;
    QString string = "";
    for(size_t i = 0; i<copy.size(); i++)
    {
        if(clocks[copy[i]].getStatus()==1 || clocks[copy[i]].getStatus()==2)
        {
            string += clocks[copy[i]].getName()+"\n";
        }
    }
    if(string!="")
    {
        QMessageBox msgBox(QMessageBox::Question, "", "The following clocks are already active:\n\n"+string+"\nRestart them?", QMessageBox::Yes|QMessageBox::No);
        msgBox.setStyleSheet("QMessageBox QPushButton{"
                                "background-color: rgb(220, 240, 255);}"
                             "QMessageBox{"
                                "background-color: rgb(129, 190, 255);}");
        auto button = msgBox.exec();
        if(button==QMessageBox::No)
        {
             for(size_t i = 0; i<copy.size(); i++)
             {
                  if(clocks[copy[i]].getStatus()==1 || clocks[copy[i]].getStatus()==2)
                  {
                      copy.erase(copy.begin()+i);
                      i--;
                  }
             }
        }
    }
    for(size_t i = 0; i<copy.size(); i++)
    {
        clocks[copy[i]].setStatus(1);
        if(clocks[copy[i]].getType()==0) //timer
        {
            qint64 valueToSec = clocks[copy[i]].getValue().hour()*3600 + clocks[copy[i]].getValue().minute()*60 + clocks[copy[i]].getValue().second();
            clocks[copy[i]].setEndTime(QDateTime::currentSecsSinceEpoch() + valueToSec);
        }
        else if(clocks[copy[i]].getType()==1) //alarm clock
        {
            qint64 valueToSec = clocks[copy[i]].getValue().hour()*3600 + clocks[copy[i]].getValue().minute()*60 + clocks[copy[i]].getValue().second();
            qint64 currentTimeToSec = QTime::currentTime().hour()*3600 + QTime::currentTime().minute()*60 + QTime::currentTime().second();
            if(QTime::currentTime()<=clocks[copy[i]].getValue())
            {
                clocks[copy[i]].setEndTime(QDateTime::currentSecsSinceEpoch() - currentTimeToSec + valueToSec);
            }
            else
            {
                const qint64 secInDay = 24*3600;
                clocks[copy[i]].setEndTime(QDateTime::currentSecsSinceEpoch() - currentTimeToSec + valueToSec + secInDay);
            }
        }
    }
    if(updateTable) this->updateTable();
}

void MainWindow::stopClocks(const std::vector<size_t>& indices, const bool& updateTable)
{
    for(size_t i = 0; i<indices.size(); i++)
    {
        clocks[indices[i]].setStatus(0);
        clocks[indices[i]].setEndTime(0);
        if(indices[i] == indexOfClosest)
        {
            isClosestExists = false;
        }
    }
    if(updateTable) this->updateTable();
}

void MainWindow::addNewClockWindow_slot()
{
    addNewClockWindow();
}

void MainWindow::editClockWindow_slot()
{
    std::vector<size_t> selected = getSelected();
    editClockWindow(selected[0]);
}

void MainWindow::removeClocks_slot()
{
    std::vector<size_t> selected = getSelected();
    removeClocks(selected, true);
}

void MainWindow::startClocks_slot()
{
    std::vector<size_t> selected = getSelected();
    startClocks(selected, true);
}

void MainWindow::stopClocks_slot()
{
    std::vector<size_t> selected = getSelected();
    stopClocks(selected, true);
}

void MainWindow::on_table_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("QMenu{background-color: rgb(204, 232, 255);}"
                        "QMenu Action:hover{background-color: red;}");
    QAction *add_new(nullptr), *start(nullptr), *stop(nullptr), *edit(nullptr), *remove(nullptr);

    QModelIndexList selected = ui->table->selectionModel()->selectedRows();
    if(selected.size()==0)
    {
        add_new = new QAction("Add new", this);
        menu->addAction(add_new);

    }
    else if(selected.size()==1)
    {
        start = new QAction("Start", this);
        stop = new QAction("Stop", this);
        edit = new QAction("Edit", this);
        remove = new QAction("Remove", this);
        menu->addAction(start);
        menu->addAction(stop);
        menu->addSeparator();
        menu->addAction(edit);
        menu->addAction(remove);
    }
    else
    {
        start = new QAction("Start", this);
        stop = new QAction("Stop", this);
        remove = new QAction("Remove", this);
        menu->addAction(start);
        menu->addAction(stop);
        menu->addSeparator();
        menu->addAction(remove);
    }

    if(add_new) connect(add_new, SIGNAL(triggered()), this, SLOT(addNewClockWindow_slot()));
    if(edit) connect(edit, SIGNAL(triggered()), this, SLOT(editClockWindow_slot()));
    if(remove) connect(remove, SIGNAL(triggered()), this, SLOT(removeClocks_slot()));
    if(start) connect(start, SIGNAL(triggered()), this, SLOT(startClocks_slot()));
    if(stop) connect(stop, SIGNAL(triggered()), this, SLOT(stopClocks_slot()));

    menu->popup(ui->table->viewport()->mapToGlobal(pos));
}

void MainWindow::on_addNewTool_triggered()
{
    addNewClockWindow();
}

void MainWindow::on_actionDo_Not_Disturb_triggered()
{
    auto doNotDisturbWindow = new DoNotDisturbWindow(this);
    doNotDisturbWindow->setModal(true);
    doNotDisturbWindow->setWindowTitle("Do Not Disturb settings");
    doNotDisturbWindow->show();
}

void MainWindow::on_table_cellDoubleClicked(int row, int column)
{
    std::vector<size_t> selected = getSelected();
    if(clocks[selected[0]].getStatus()==1 || clocks[selected[0]].getStatus()==2) stopClocks(selected, true);
    else startClocks(selected, true);
}


