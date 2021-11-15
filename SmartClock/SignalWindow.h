#ifndef SIGNALWINDOW_H
#define SIGNALWINDOW_H

#include <QDialog>
#include <QSound>
#include "MainWindow.h"

namespace Ui
{
class SignalWindow;
}

class SignalWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SignalWindow(MainWindow *parent, MainWindow::Clock *clock);
    ~SignalWindow();

private slots:
    void on_pushButton_plus1_clicked();
    void on_pushButton_plus5_clicked();
    void on_pushButton_plus10_clicked();
    void on_pushButton_plus15_clicked();
    void on_pushButton_stop_clicked();

private:
    Ui::SignalWindow *ui;
    MainWindow::Clock *clock;
    QSound *signal;
protected:
    MainWindow *parent;
};

#endif
