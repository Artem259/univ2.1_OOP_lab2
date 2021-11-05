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



    ui->table->insertRow(ui->table->rowCount());
    ui->table->insertRow(ui->table->rowCount());
    ui->table->insertRow(ui->table->rowCount());

    ui->table->setItem(0,0,new QTableWidgetItem("0"));
    ui->table->setItem(0,1,new QTableWidgetItem("2"));
    ui->table->setItem(0,2,new QTableWidgetItem("Ab"));

    ui->table->setItem(1,0,new QTableWidgetItem("1"));
    ui->table->setItem(1,1,new QTableWidgetItem("4"));
    ui->table->setItem(1,2,new QTableWidgetItem("Ac"));

    ui->table->setItem(2,0,new QTableWidgetItem("2"));
    ui->table->setItem(2,1,new QTableWidgetItem("6"));
    ui->table->setItem(2,2,new QTableWidgetItem("Acс"));

}

void MainWindow::addNew()
{
    auto clockWindow = new ClockWindow(this, true);
    clockWindow->setModal(true);
    clockWindow->setWindowTitle("New Clock");
    clockWindow->show();
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

    if(add_new) connect(add_new, SIGNAL(triggered()), this, SLOT(addNew()));

    menu->popup(ui->table->viewport()->mapToGlobal(pos));
}
