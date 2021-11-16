#include "DoNotDisturbWindow.h"
#include "ui_DoNotDisturbWindow.h"

DoNotDisturbWindow::DoNotDisturbWindow(MainWindow *parent) : QDialog(parent), ui(new Ui::DoNotDisturbWindow)
{
    ui->setupUi(this);
}

DoNotDisturbWindow::~DoNotDisturbWindow()
{
    delete ui;
}
