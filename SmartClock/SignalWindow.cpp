#include "SignalWindow.h"
#include "ui_SignalWindow.h"

#include <QDebug>

SignalWindow::SignalWindow(MainWindow *parent, MainWindow::Clock *clock) : QDialog(parent), ui(new Ui::SignalWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->parent = parent;
    this->clock = clock;
    this->signal = nullptr;

    if(parent->getStatusDoNotDisturb() == 0)
    {
        if(clock->getType()==0) this->signal = new QSound(":/prefix1/sounds/timer_sound.wav", this);
        else //clock->getType()==1
            this->signal = new QSound(":/prefix1/sounds/alarm_clock_sound.wav", this);
        signal->setLoops(QSound::Infinite);
        signal->play();
    }

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
    parent->updateTable();
    this->close();
}

void SignalWindow::on_pushButton_plus5_clicked()
{
    clock->delayBySec(5*60);
    parent->updateTable();
    this->close();
}

void SignalWindow::on_pushButton_plus10_clicked()
{
    clock->delayBySec(10*60);
    parent->updateTable();
    this->close();
}

void SignalWindow::on_pushButton_plus15_clicked()
{
    clock->delayBySec(15*60);
    parent->updateTable();
    this->close();
}
