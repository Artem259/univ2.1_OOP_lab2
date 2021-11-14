#include "SignalWindow.h"
#include "ui_SignalWindow.h"

#include <QDebug>

SignalWindow::SignalWindow(MainWindow *parent, MainWindow::Clock *clock) : QDialog(parent), ui(new Ui::SignalWindow)
{
    ui->setupUi(this);
    this->parent = parent;
    this->clock = clock;

    if(clock->getType()==0)
    {
        ui->pushButton_plus1->hide();
        ui->pushButton_plus5->hide();
        ui->pushButton_plus10->hide();
        ui->pushButton_plus15->hide();
    }

    ui->name->setText(clock->getName());
    ui->valueTime->setText(clock->getValue().toString("hh:mm:ss"));
}

SignalWindow::~SignalWindow()
{
    delete ui;
}

void SignalWindow::on_pushButton_stop_clicked()
{
    this->close();
}

void SignalWindow::on_pushButton_plus1_clicked()
{
    clock->delayBySec(1*60);
    this->close();
}

void SignalWindow::on_pushButton_plus5_clicked()
{
    clock->delayBySec(5*60);
    this->close();
}

void SignalWindow::on_pushButton_plus10_clicked()
{
    clock->delayBySec(10*60);
    this->close();
}

void SignalWindow::on_pushButton_plus15_clicked()
{
    clock->delayBySec(15*60);
    this->close();
}
