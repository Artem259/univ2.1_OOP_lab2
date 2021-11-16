#include <QDateTime>
#include <iostream>
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

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
MainWindow::Clock::Clock(const Clock& clock)
{
    this->parent = clock.getParent();

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
MainWindow* MainWindow::Clock::getParent() const
{
    return parent;
}

void MainWindow::Clock::printToTable(const size_t& row) const
{
    bool sorting = parent->ui->table->isSortingEnabled();
    parent->ui->table->setSortingEnabled(false);
    //№
    parent->ui->table->item(row,0)->setData(Qt::DisplayRole, id+1);
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
    if(status==0) parent->ui->table->item(row,3)->setText("");
    else if(status==1) parent->ui->table->item(row,3)->setText("Active");
    else if(status==2) parent->ui->table->item(row,3)->setText("Delayed");
    //Value
    parent->ui->table->item(row,4)->setText(value.toString("hh : mm : ss"));
    //End time
    if(status==0) parent->ui->table->item(row,5)->setText("");
    else if(status==1 || status==2)
    {
        QDateTime endTimeValue;
        endTimeValue.QDateTime::setSecsSinceEpoch(endTime);
        parent->ui->table->item(row,5)->setText(endTimeValue.toString("hh : mm : ss"));
    }
    //Time left
    if(status==0) parent->ui->table->item(row,6)->setText("");
    else if(status==1 || status==2)
    {
        QTime timeLeft = QTime(0,0,0).addSecs(endTime - QDateTime::currentSecsSinceEpoch());
        parent->ui->table->item(row,6)->setText(timeLeft.toString("hh : mm : ss"));
    }
    //Repeating
    parent->ui->table->item(row,7)->setText(repeating ? "Yes" : "No");
    parent->ui->table->setSortingEnabled(sorting);
}

void MainWindow::Clock::delayBySec(const qint64& seconds)
{
    status = 2;
    endTime = QDateTime::currentSecsSinceEpoch() + seconds;
}


std::ostream& operator <<(std::ostream &in, const QTime &time)
{
    in << time.hour() << " "; //write hours
    in << time.minute() << " "; //write minutes
    in << time.second() << " "; //write seconds
    return in;
}

std::istream& operator >>(std::istream &out, QTime &time)
{
    size_t data;
    out >> data; //read hours
    time = QTime(0,0,0).addSecs(data*3600); //set hours
    out >> data; //read minutes
    time = time.addSecs(data*60); //set minutes
    out >> data; //read seconds
    time = time.addSecs(data); //set seconds
    return out;
}

std::ostream& operator <<(std::ostream &in, const MainWindow::Clock &clock)
{
    in << clock.getId() << " "; //Id
    in << clock.getName().toStdString().size() << " "; //size of the Name
    in << clock.getName().toStdString() << " "; //Name
    in << clock.getType() << " "; //Type
    in << clock.getStatus() << " "; //Status
    in << clock.getValue(); //Value (defined higher)
    in << clock.getEndTime() << " "; //End time
    in << clock.getRepeating(); //Repeating
    return in;
}

std::istream& operator >>(std::istream &out, MainWindow::Clock &clock)
{
    char c;
    char *name;
    size_t data;
    QTime value;
    qint64 endTime;

    std::ios_base::fmtflags flags(out.flags());
    out >> std::skipws;

    out >> data; //read Id
    clock.setId(data); //set Id

    out >> data; //read size of the Name
    out.get(); //skip space
    name = new char[data+1]();
    out >> std::noskipws;
    for(size_t i = 0; i<data; i++)
    {
        out >> c;
        name[i] = c;
    }
    out >> std::skipws;
    clock.setName(name);
    delete []name;

    out >> data; //read Type
    clock.setType(data); //set Type
    out >> data; //read Status
    clock.setStatus(data); //set Status
    out >> value; //read Value (defined higher)
    clock.setValue(value); //set Value
    out >> endTime; //read EndTime
    clock.setEndTime(endTime); //set EndTime
    out >> data; //read Repeating
    clock.setRepeating(data); //set Repeating

    out.flags(flags);
    return out;
}
