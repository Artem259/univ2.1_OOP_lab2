#include <QDateTime>
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::Clock::Clock(MainWindow *parent)
{
    this->parent = parent;

    this->id = 0;
    this->name = "";
    this->type = -1;
    this->status = -1;
    this->value = QTime();
    this->endTime = 0;
    this->repeating = false;
}
MainWindow::Clock::Clock(MainWindow *parent, const Clock& clock)
{
    this->parent = parent;

    this->id = clock.getId();
    this->name = clock.getName();
    this->type = clock.getType();
    this->status = clock.getStatus();
    this->value = clock.getValue();
    this->endTime = clock.getEndTime();
    this->repeating = clock.getRepeating();
}
void MainWindow::Clock::set(const size_t& id, const QString& title, const short& type, const short& status,
                         const QTime& value, const qint64& endTime, const bool& repeating)
{
    this->id = id;
    this->name = title;
    this->type = type;
    this->status = status;
    this->value = value;
    this->endTime = endTime;
    this->repeating = repeating;
}
void MainWindow::Clock::setId(const size_t& id)
{
    this->id = id;
}
void MainWindow::Clock::setName(const QString& title)
{
    this->name = title;
}
void MainWindow::Clock::setType(const short& type)
{
    this->type = type;
}
void MainWindow::Clock::setStatus(const short& status)
{
    this->status = status;
}
void MainWindow::Clock::setValue(const QTime& value)
{
    this->value = value;
}
void MainWindow::Clock::setEndTime(const qint64& endTime)
{
    this->endTime = endTime;
}
void MainWindow::Clock::setRepeating(const bool& repeating)
{
    this->repeating = repeating;
}
size_t MainWindow::Clock::getId() const
{
    return id;
}
QString MainWindow::Clock::getName() const
{
    return name;
}
short MainWindow::Clock::getType() const
{
    return type;
}
short MainWindow::Clock::getStatus() const
{
    return status;
}
QTime MainWindow::Clock::getValue() const
{
    return this->value;
}
qint64 MainWindow::Clock::getEndTime() const
{
    return endTime;
}
bool MainWindow::Clock::getRepeating() const
{
    return repeating;
}

void MainWindow::Clock::printToTable(const size_t& row) const
{
    parent->ui->table->setSortingEnabled(false);
    //№
    parent->ui->table->item(row,0)->setText(QString::number(id+1));
    //Name
    parent->ui->table->item(row,1)->setText(name);
    //Type
    if(type==0)
    {
        QIcon icon(":/prefix1/images/timer.png");
        parent->ui->table->item(row,2)->setIcon(icon);
        parent->ui->table->item(row,2)->setText("Timer");
    }
    else if(type==1)
    {
        QIcon icon(":/prefix1/images/alarm_clock.png");
        parent->ui->table->item(row,2)->setIcon(icon);
        parent->ui->table->item(row,2)->setText("Alarm clock");
    }
    //Status
    if(status==1) parent->ui->table->item(row,3)->setText("Active");
    //Value
    parent->ui->table->item(row,4)->setText(value.toString("hh : mm : ss"));
    //End time
    if(status==1)
    {
        parent->ui->table->item(row,5)->setText(value.toString("hh : mm : ss"));
    }
    //Time left
    if(status==1)
    {
        QTime timeLeft = QTime().addSecs(endTime - QDateTime::currentSecsSinceEpoch());
        parent->ui->table->item(row,6)->setText(timeLeft.toString("hh : mm : ss"));
    }
    //Repeating
    parent->ui->table->item(row,7)->setText(repeating ? "Yes" : "No");
    parent->ui->table->setSortingEnabled(true);
}
