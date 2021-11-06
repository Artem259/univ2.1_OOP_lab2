#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Clock;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addNewClock(Clock* clock);


private slots:
    void on_table_customContextMenuRequested(const QPoint &pos);
    void addNewClockWindow();

private:

protected:
    Ui::MainWindow *ui;
    std::vector<Clock> clocks;
};

#endif
