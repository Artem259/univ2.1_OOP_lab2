#include <QMessageBox>
#include "ClockWindow.h"
#include "ui_ClockWindow.h"

#include <QDebug>

ClockWindow::ClockWindow(MainWindow *parent, MainWindow::Clock *clock) : QDialog(parent), ui(new Ui::ClockWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->parent = parent;
    this->clock = clock;

    if(clock)
    {
        ui->nameEdit->setText(clock->getName());
        if(clock->getType()==0) ui->radioButtonTimer->setChecked(true);
        else if(clock->getType()==1) ui->radioButtonAlarmClock->setChecked(true);
        ui->timeEdit->setTime(clock->getValue());
        ui->checkRepeating->setChecked(clock->getRepeating());
    }
}

ClockWindow::~ClockWindow()
{
    delete ui;
}

void ClockWindow::on_buttonBox_accepted()
{
    if(ui->nameEdit->text()=="")
    {
        QMessageBox msgBox(QMessageBox::Critical, "Error", "You must enter the clock name.");
        msgBox.setStyleSheet("QMessageBox QPushButton{"
                                "background-color: rgb(220, 240, 255);}"
                             "QMessageBox{"
                                "background-color: rgb(129, 190, 255);}");
        msgBox.exec();
        return;
    }
    if(!ui->radioButtonTimer->isChecked() && !ui->radioButtonAlarmClock->isChecked())
    {
        QMessageBox msgBox(QMessageBox::Critical, "Error", "You must choose the clock type.");
        msgBox.setStyleSheet("QMessageBox QPushButton{"
                                "background-color: rgb(220, 240, 255);}"
                             "QMessageBox{"
                                "background-color: rgb(129, 190, 255);}");
        msgBox.exec();
        return;
    }

    qint64 valueToSec = ui->timeEdit->time().hour()*3600 + ui->timeEdit->time().minute()*60 + ui->timeEdit->time().second();
    if(valueToSec<10 && ui->radioButtonTimer->isChecked() && ui->checkRepeating->isChecked())
    {
        QMessageBox msgBox(QMessageBox::Critical, "Error", "You can't set repeating timers below 10 seconds.");
        msgBox.setStyleSheet("QMessageBox QPushButton{"
                                "background-color: rgb(220, 240, 255);}"
                             "QMessageBox{"
                                "background-color: rgb(129, 190, 255);}");
        msgBox.exec();
        return;
    }

    auto newClock = new MainWindow::Clock(parent);

    newClock->setName(ui->nameEdit->text());
    if(ui->radioButtonTimer->isChecked()) newClock->setType(0);
    else if(ui->radioButtonAlarmClock->isChecked()) newClock->setType(1);
    newClock->setValue(ui->timeEdit->time());
    newClock->setRepeating(ui->checkRepeating->isChecked());

    if(!clock) parent->addNewClock(newClock, true); //adding new clock
    else parent->editClock(newClock, true); //editing clock

    this->close();
}

void ClockWindow::on_buttonBox_rejected()
{
    this->close();
}

