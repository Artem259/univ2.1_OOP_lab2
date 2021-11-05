#include "ClockWindow.h"
#include "ui_ClockWindow.h"

ClockWindow::ClockWindow(QWidget *parent, bool newClock, size_t row) : QDialog(parent), ui(new Ui::ClockWindow)
{
    this->newClock = newClock;
    this->row = row;
    ui->setupUi(this);
}

ClockWindow::~ClockWindow()
{
    delete ui;
}

void ClockWindow::on_buttonBox_accepted()
{
    if(this->newClock)
    {

    }
}

void ClockWindow::on_buttonBox_rejected()
{
    this->close();
}

