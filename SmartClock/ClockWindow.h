#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <QDialog>
#include <QTime>
#include "MainWindow.h"

namespace Ui
{
class ClockWindow;
}


class ClockWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ClockWindow(MainWindow *parent, MainWindow::Clock *clock=nullptr);
    ~ClockWindow();
    void setData(QString title, short radio, QTime time);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ClockWindow *ui;
    MainWindow::Clock *clock;
protected:
    MainWindow *parent;
};

#endif
