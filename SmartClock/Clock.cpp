#include <QDateTime>
#include "Clock.h"
#include "ui_MainWindow.h"

Clock::Clock() = default;
Clock::Clock(const Clock& clock)
{
    this->id = clock.getId();
    this->title = clock.getTitle();
    this->type = clock.getType();
    this->status = clock.getStatus();
    this->value = clock.getValue();
    this->endTime = clock.getEndTime();
    this->repeating = clock.getRepeating();
}
void Clock::set(const size_t& id, const QString& title, const short& type, const short& status,
                         const QTime& value, const qint64& endTime, const bool& repeating)
{
    this->id = id;
    this->title = title;
    this->type = type;
    this->status = status;
    this->value = value;
    this->endTime = endTime;
    this->repeating = repeating;
}
void Clock::setId(const size_t& id)
{
    this->id = id;
}
void Clock::setTitle(const QString& title)
{
    this->title = title;
}
void Clock::setType(const short& type)
{
    this->type = type;
}
void Clock::setStatus(const short& status)
{
    this->status = status;
}
void Clock::setValue(const QTime& value)
{
    this->value = value;
}
void Clock::setEndTime(const qint64& endTime)
{
    this->endTime = endTime;
}
void Clock::setRepeating(const bool& repeating)
{
    this->repeating = repeating;
}
size_t Clock::getId() const
{
    return id;
}
QString Clock::getTitle() const
{
    return title;
}
short Clock::getType() const
{
    return type;
}
short Clock::getStatus() const
{
    return status;
}
QTime Clock::getValue() const
{
    return this->value;
}
qint64 Clock::getEndTime() const
{
    return endTime;
}
bool Clock::getRepeating() const
{
    return repeating;
}


void Clock::printToTable(const size_t& row) const
{
    //"№" << "Title" << "Type" << "Status" << "Value" << "End time" << "Time left";
    //№
    qDebug()<<row;
    ui->table->item(row,0)->setText(QString::number(id+1));
    //Title
    ui->table->item(row,1)->setText(title);
    //Type
    if(type==0) ui->table->item(row,2)->setText("Timer");
    else if(type==1) ui->table->item(row,2)->setText("Alarm clock");
    //Status
    if(status==1) ui->table->item(row,3)->setText("Active");
    //Value
    ui->table->item(row,4)->setText(value.toString("hh::mm::ss"));
    //End time
    if(status==1)
    {
        ui->table->item(row,5)->setText(value.toString("hh::mm::ss"));
    }
    //Time left
    if(status==1)
    {
        QTime timeLeft = QTime().addSecs(endTime - QDateTime::currentSecsSinceEpoch());
        ui->table->item(row,6)->setText(timeLeft.toString("hh::mm::ss"));
    }
}
