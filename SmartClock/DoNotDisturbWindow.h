#ifndef DONOTDISTURBWINDOW_H
#define DONOTDISTURBWINDOW_H

#include <QDialog>
#include "MainWindow.h"

namespace Ui {
class DoNotDisturbWindow;
}

class DoNotDisturbWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DoNotDisturbWindow(MainWindow *parent);
    ~DoNotDisturbWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DoNotDisturbWindow *ui;
protected:
    MainWindow *parent;
};

#endif
