#include <QHeaderView>
#include <QMessageBox>
#include "MainWindow.h"
#include "ClockWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->toolBar->setCursor(Qt::PointingHandCursor);

    QStringList horizHeaders;
    horizHeaders << "№" << "Name" << "Type" << "Status" << "Value" << "End time" << "Time left" << "Repeating";
    ui->table->setColumnCount(horizHeaders.size());
    ui->table->setHorizontalHeaderLabels(horizHeaders);
    ui->table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->table->verticalHeader()->setDefaultSectionSize(20);
}

void MainWindow::addNewClockWindow()
{
    auto clockWindow = new ClockWindow(this);
    clockWindow->setModal(true);
    clockWindow->setWindowTitle("New Clock");
    clockWindow->show();
}

void MainWindow::editClockWindow()
{
    QModelIndexList selected = ui->table->selectionModel()->selectedRows();
    size_t index = (ui->table->item(selected[0].row(),0)->text()).toULongLong()-1;
    if(clocks[index].getStatus()==1)
    {
        QMessageBox msgBox(QMessageBox::Critical, "Error", "You can't edit active clock.");
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

void MainWindow::addNewClock(Clock *clock)
{
    clock->setStatus(0);
    clock->setId(this->clocks.size());
    this->clocks.push_back(*clock);
    size_t row = ui->table->rowCount();
    ui->table->setSortingEnabled(false);
    ui->table->insertRow(row);
    ui->table->setItem(row,0,new QTableWidgetItem());
    ui->table->setItem(row,1,new QTableWidgetItem());
    ui->table->setItem(row,2,new QTableWidgetItem());
    ui->table->setItem(row,3,new QTableWidgetItem());
    ui->table->setItem(row,4,new QTableWidgetItem());
    ui->table->setItem(row,5,new QTableWidgetItem());
    ui->table->setItem(row,6,new QTableWidgetItem());
    ui->table->setItem(row,7,new QTableWidgetItem());
    clock->printToTable(row);
    ui->table->setSortingEnabled(true);
    delete clock;
}

void MainWindow::editClock(Clock* clock)
{
    QModelIndexList selected = ui->table->selectionModel()->selectedRows();
    size_t index = (ui->table->item(selected[0].row(),0)->text()).toULongLong()-1;
    clock->setStatus(0);
    clock->setId(index);
    clocks[index] = *clock;
    ui->table->setSortingEnabled(false);
    clock->printToTable(selected[0].row());
    ui->table->setSortingEnabled(true);
    delete clock;
}

void MainWindow::removeClocks()
{
    size_t index;
    QString string = "";
    QModelIndexList selected = ui->table->selectionModel()->selectedRows();
    for(size_t i = 0; i<selected.size(); i++)
    {
        index = (ui->table->item(selected[i].row(),0)->text()).toULongLong()-1;
        if(clocks[index].getStatus()!=0)
        {
            string += clocks[index].getName()+"\n";
        }
    }
    if(string!="")
    {
        QMessageBox::StandardButton msgBox;
        msgBox = QMessageBox::question(this, "", "This action will remove the following active clocks:\n\n"+string+"\nAre you sure?", QMessageBox::Yes|QMessageBox::No);
        setStyleSheet("QMessageBox QPushButton{"
                             "background-color: rgb(220, 240, 255);}"
                             "QMessageBox{"
                             "background-color: rgb(129, 190, 255);}");
        if(msgBox==QMessageBox::No)
        {
            return;
        }
    }


    ui->table->setSortingEnabled(false);
    std::vector<size_t> removed;
    while(!selected.empty()) //removing rows from the table, recovering numeration in the table
    {
        index = (ui->table->item(selected[0].row(),0)->text()).toULongLong()-1;
        auto pos = std::upper_bound(removed.begin(), removed.end(), index);
        removed.insert(pos, index);
        ui->table->removeRow(selected[0].row());
        for(size_t i = 0; i<ui->table->rowCount(); i++)
        {
            if((ui->table->item(i,0)->text()).toULongLong() > index)
            {
                ui->table->item(i,0)->setText(QString::number((ui->table->item(i,0)->text()).toULongLong() - 1));
            }
        }
        selected = ui->table->selectionModel()->selectedRows();
    }
    ui->table->setSortingEnabled(true);
    for(size_t i = removed.size()-1; i<removed.size(); i--) //removing clocks from the vector
    {
        clocks.erase(clocks.begin()+removed[i]);
    }
    for(size_t i = clocks.size()-1; i<clocks.size(); i--) //recovering numeration in the vector
    {
        clocks[i].setId(i);
    }
    qDebug()<<removed[0];
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_table_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("QMenu{background-color: rgb(204, 232, 255);}"
                        "QMenu:hover{background-color: red;}");
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

    if(add_new) connect(add_new, SIGNAL(triggered()), this, SLOT(addNewClockWindow()));
    if(edit) connect(edit, SIGNAL(triggered()), this, SLOT(editClockWindow()));
    if(remove) connect(remove, SIGNAL(triggered()), this, SLOT(removeClocks()));

    menu->popup(ui->table->viewport()->mapToGlobal(pos));
}
