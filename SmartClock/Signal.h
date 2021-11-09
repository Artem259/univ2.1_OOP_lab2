#ifndef SIGNAL_H
#define SIGNAL_H

#include <QDialog>

namespace Ui {
class Signal;
}

class Signal : public QDialog
{
    Q_OBJECT

public:
    explicit Signal(QWidget *parent = nullptr);
    ~Signal();

private:
    Ui::Signal *ui;
};

#endif // SIGNAL_H
