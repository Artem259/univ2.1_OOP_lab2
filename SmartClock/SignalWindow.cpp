#include "SignalWindow.h"
#include "ui_SignalWindow.h"

SignalWindow::SignalWindow(QWidget *parent) : QDialog(parent), ui(new Ui::SignalWindow)
{
    ui->setupUi(this);
}

SignalWindow::~SignalWindow()
{
    delete ui;
}
