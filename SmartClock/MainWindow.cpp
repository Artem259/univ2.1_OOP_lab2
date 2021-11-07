#include <QHeaderView>
#include "MainWindow.h"
#include "ClockWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->toolBar->setCursor(Qt::PointingHandCursor);

    QStringList horizHeaders;
    horizHeaders << "№" << "Title" << "Type" << "Status" << "Value" << "End time" << "Time left";
    ui->table->setColumnCount(horizHeaders.size());
    ui->table->setHorizontalHeaderLabels(horizHeaders);
    ui->table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->table->verticalHeader()->setDefaultSectionSize(20);
}

void MainWindow::addNewClockWindow()
{
    auto clockWindow = new ClockWindow(this, true);
    clockWindow->setModal(true);
    clockWindow->setWindowTitle("New Clock");
    clockWindow->show();
}

void MainWindow::addNewClock(Clock *clock)
{
    clock->setStatus(0);
    clock->setId(this->clocks.size());
    this->clocks.push_back(*clock);
    size_t row = ui->table->rowCount();
    /*ui->table->insertRow(row);
    ui->table->setItem(row,0,new QTableWidgetItem());
    ui->table->setItem(row,1,new QTableWidgetItem());
    ui->table->setItem(row,2,new QTableWidgetItem());
    ui->table->setItem(row,3,new QTableWidgetItem());
    ui->table->setItem(row,4,new QTableWidgetItem());
    ui->table->setItem(row,5,new QTableWidgetItem());
    ui->table->setItem(row,6,new QTableWidgetItem());*/
    qDebug()<<&ui;
    clock->printToTable(row);
    qDebug()<<ui->table->rowCount();
/*
    ui->table->insertRow(row);
    ui->table->setItem(row,0,new QTableWidgetItem());
    ui->table->setItem(row,1,new QTableWidgetItem());
    ui->table->setItem(row,2,new QTableWidgetItem());
    ui->table->setItem(row,3,new QTableWidgetItem());
    ui->table->setItem(row,4,new QTableWidgetItem());
    ui->table->setItem(row,5,new QTableWidgetItem());
    ui->table->setItem(row,6,new QTableWidgetItem());
    //"№" << "Title" << "Type" << "Status" << "Value" << "End time" << "Time left";
    //№
    ui->table->item(row,0)->setText(QString::number(clock->getId()+1));
    //Title
    ui->table->item(row,1)->setText(clock->getTitle());
    //Type
    if(clock->getType()==0) ui->table->item(row,2)->setText("Timer");
    else if(clock->getType()==1) ui->table->item(row,2)->setText("Alarm clock");
    //Status
    if(clock->getStatus()==1) ui->table->item(row,3)->setText("Active");
    //Value
    ui->table->item(row,4)->setText(clock->getValue().toString("hh::mm::ss"));
    //End time
    if(clock->getStatus()==1)
    {
        ui->table->item(row,5)->setText(clock->getValue().toString("hh::mm::ss"));
    }
    //Time left
    if(clock->getStatus()==1)
    {
        QTime timeLeft = QTime().addSecs(clock->getEndTime() - QDateTime::currentSecsSinceEpoch());
        ui->table->item(row,6)->setText(timeLeft.toString("hh::mm::ss"));
    }
    qDebug()<<"a";
*/
    delete clock;
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

    menu->popup(ui->table->viewport()->mapToGlobal(pos));
}
