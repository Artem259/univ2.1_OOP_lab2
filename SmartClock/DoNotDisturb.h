#ifndef DONOTDISTURB_H
#define DONOTDISTURB_H

#include <QDialog>

namespace Ui {
class DoNotDisturb;
}

class DoNotDisturb : public QDialog
{
    Q_OBJECT

public:
    explicit DoNotDisturb(QWidget *parent = nullptr);
    ~DoNotDisturb();

private:
    Ui::DoNotDisturb *ui;
};

#endif // DONOTDISTURB_H
