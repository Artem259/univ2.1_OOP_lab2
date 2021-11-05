#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <QDialog>
#include <QTime>

namespace Ui
{
class ClockWindow;
}


class ClockWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ClockWindow(QWidget *parent);
    ~ClockWindow();
    void setData(QString title, short radio, QTime time);


private:
    Ui::ClockWindow *ui;
    QString title;
    short radio;
    QTime time;
};

#endif
