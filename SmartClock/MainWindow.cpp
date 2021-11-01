#include <QHeaderView>
#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList horizHeaders;
    horizHeaders << "Queue" << "Title" << "Type" << "Value" << "Status" << "Time left";
    ui->table->setColumnCount(horizHeaders.size());
    ui->table->setHorizontalHeaderLabels(horizHeaders);
    ui->table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->table->verticalHeader()->setDefaultSectionSize(20);




    ui->table->insertRow(ui->table->rowCount());
    ui->table->insertRow(ui->table->rowCount());
    ui->table->insertRow(ui->table->rowCount());

    ui->table->setItem(0,0,new QTableWidgetItem("1"));
    ui->table->setItem(0,1,new QTableWidgetItem("2"));
    ui->table->setItem(0,2,new QTableWidgetItem("Ab"));

    ui->table->setItem(1,0,new QTableWidgetItem("3"));
    ui->table->setItem(1,1,new QTableWidgetItem("4"));
    ui->table->setItem(1,2,new QTableWidgetItem("Ac"));

    ui->table->setItem(2,0,new QTableWidgetItem("5"));
    ui->table->setItem(2,1,new QTableWidgetItem("6"));
    ui->table->setItem(2,2,new QTableWidgetItem("Acс"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_table_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(new QAction("Action 1", this));
    menu->addAction(new QAction("Action 2", this));
    menu->addAction(new QAction("Action 3", this));
    menu->popup(ui->table->viewport()->mapToGlobal(pos));
}

