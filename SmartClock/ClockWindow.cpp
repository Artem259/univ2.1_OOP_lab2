#include <QMessageBox>
#include "ClockWindow.h"
#include "MainWindow.h"
#include "ui_ClockWindow.h"

ClockWindow::ClockWindow(MainWindow *parent, bool newClock, size_t row) : QDialog(parent), ui(new Ui::ClockWindow)
{
    ui->setupUi(this);
    this->parent = parent;
    this->newClock = newClock;
    this->row = row;
}

ClockWindow::~ClockWindow()
{
    delete ui;
}

void ClockWindow::on_buttonBox_accepted()
{
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
    if(this->newClock)
    {
        auto newClock = new MainWindow::Clock(parent);

        newClock->setTitle(ui->titleEdit->text());
        if(ui->radioButtonTimer->isChecked()) newClock->setType(0);
        else if(ui->radioButtonAlarmClock->isChecked()) newClock->setType(1);
        newClock->setValue(ui->timeEdit->time());
        newClock->setRepeating(ui->checkRepeating->isChecked());

        this->close();
        parent->addNewClock(newClock);
    }
}

void ClockWindow::on_buttonBox_rejected()
{
    this->close();
}

