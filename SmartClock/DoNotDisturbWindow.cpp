#include <QMessageBox>
#include "DoNotDisturbWindow.h"
#include "ui_DoNotDisturbWindow.h"

DoNotDisturbWindow::DoNotDisturbWindow(MainWindow *parent) : QDialog(parent), ui(new Ui::DoNotDisturbWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->parent = parent;

    ui->checkBoxDoNotDisturb->setChecked(parent->getStatusDoNotDisturb()==2);
    ui->scheduled->setChecked(parent->getIsScheduledDoNotDisturb());
    ui->timeEditFrom->setTime(parent->getStartDoNotDisturb());
    ui->timeEditTo->setTime(parent->getEndDoNotDisturb());
}

DoNotDisturbWindow::~DoNotDisturbWindow()
{
    delete ui;
}

void DoNotDisturbWindow::on_buttonBox_accepted()
{
    if(ui->timeEditFrom->time() == ui->timeEditTo->time() && ui->scheduled->isChecked())
    {
        QMessageBox msgBox(QMessageBox::Critical, "Error", "The values of the times \"From\" and \"To\" can't be equal.");
        msgBox.setStyleSheet("QMessageBox QPushButton{"
                                "background-color: rgb(220, 240, 255);}"
                             "QMessageBox{"
                                "background-color: rgb(129, 190, 255);}");
        msgBox.exec();
        return;
    }

    if(ui->checkBoxDoNotDisturb->isChecked()) parent->setStatusDoNotDisturb(2);
    else parent->setStatusDoNotDisturb(0);
    parent->setIsScheduledDoNotDisturb(ui->scheduled->isChecked());
    parent->setStartDoNotDisturb(ui->timeEditFrom->time());
    parent->setEndDoNotDisturb(ui->timeEditTo->time());

    parent->updateDoNotDisturb();
    this->close();
}

void DoNotDisturbWindow::on_buttonBox_rejected()
{
    this->close();
}

