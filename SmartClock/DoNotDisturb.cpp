#include "DoNotDisturb.h"
#include "ui_DoNotDisturb.h"

DoNotDisturb::DoNotDisturb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoNotDisturb)
{
    ui->setupUi(this);
}

DoNotDisturb::~DoNotDisturb()
{
    delete ui;
}
