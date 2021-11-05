#include "MainWindow.h"

MainWindow::Clock::Clock(const size_t& id, const std::string& title, const short& type, const short& status,
                         const QDateTime& value, const qint64& endTime, const bool& repeating)
{
    this->set(id, title, type, status, value, endTime, repeating);
}
void MainWindow::Clock::set(const size_t& id, const std::string& title, const short& type, const short& status,
                         const QDateTime& value, const qint64& endTime, const bool& repeating)
{
    this->id = id;
    this->title = title;
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
void MainWindow::Clock::setTitle(const std::string& title)
{
    this->title = title;
}
void MainWindow::Clock::setType(const short& type)
{
    this->type = type;
}
void MainWindow::Clock::setStatus(const short& status)
{
    this->status = status;
}
void MainWindow::Clock::setValue(const QDateTime& value)
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
size_t MainWindow::Clock::getId()
{
    return id;
}
std::string MainWindow::Clock::getTitle()
{
    return title;
}
short MainWindow::Clock::getType()
{
    return type;
}
short MainWindow::Clock::getStatus()
{
    return status;
}
QDateTime MainWindow::Clock::getValue()
{
    return this->value;
}
qint64 MainWindow::Clock::getEndTime()
{
    return endTime;
}
bool MainWindow::Clock::getRepeating()
{
    return repeating;
}

