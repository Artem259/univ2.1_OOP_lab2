#ifndef SIGNALWINDOW_H
#define SIGNALWINDOW_H

#include <QDialog>

namespace Ui
{
class SignalWindow;
}

class SignalWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SignalWindow(QWidget *parent = nullptr);
    ~SignalWindow();

private:
    Ui::SignalWindow *ui;
};

#endif
