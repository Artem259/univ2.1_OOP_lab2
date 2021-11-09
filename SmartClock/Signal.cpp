#include "Signal.h"
#include "ui_Signal.h"

Signal::Signal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Signal)
{
    ui->setupUi(this);
}

Signal::~Signal()
{
    delete ui;
}
