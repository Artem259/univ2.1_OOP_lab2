#ifndef CLOCK_H
#define CLOCK_H

#include <QTime>
#include "MainWindow.h"

class Clock : public MainWindow
{
public:
    Clock();
    Clock(const Clock& clock);
    void set(const size_t& id, const QString& title, const short& type, const short& status,
             const QTime& value, const qint64& endTime, const bool& repeating);
    void setId(const size_t& id);
    void setTitle(const QString& title);
    void setType(const short& type);
    void setStatus(const short& status);
    void setValue(const QTime& value);
    void setEndTime(const qint64& endTime);
    void setRepeating(const bool& repetitive);
    size_t getId() const;
    QString getTitle() const;
    short getType() const;
    short getStatus() const;
    QTime getValue() const;
    qint64 getEndTime() const;
    bool getRepeating() const;

    void printToTable(const size_t& row) const;
private:
    size_t id;
    QString title;
    short type; //0->Timer, 1->Alarm clock
    short status; //0->off, 1->Active
    QTime value;
    qint64 endTime;
    bool repeating;
};

#endif
