#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <QDialog>


namespace Ui {
class ClockWindow;}


class ClockWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ClockWindow(QWidget *parent = nullptr);
    ~ClockWindow();

private:
    Ui::ClockWindow *ui;
};

#endif
