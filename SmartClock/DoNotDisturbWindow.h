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

private:
    Ui::DoNotDisturbWindow *ui;
};

#endif
