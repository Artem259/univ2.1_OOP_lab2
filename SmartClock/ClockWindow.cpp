#include "ClockWindow.h"
#include "ui_ClockWindow.h"

ClockWindow::ClockWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ClockWindow)
{
    ui->setupUi(this);
}

ClockWindow::~ClockWindow()
{
    delete ui;
}
