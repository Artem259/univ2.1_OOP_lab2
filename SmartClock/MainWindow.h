#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_table_customContextMenuRequested(const QPoint &pos);
    void addNew();

private:
    Ui::MainWindow *ui;
protected:
    class Clock
    {
    public:
        Clock(const size_t& id, const std::string& title, const short& type, const short& status,
              const QDateTime& value, const qint64& endTime, const bool& repeating);
        void set(const size_t& id, const std::string& title, const short& type, const short& status,
                 const QDateTime& value, const qint64& endTime, const bool& repeating);
        void setId(const size_t& id);
        void setTitle(const std::string& title);
        void setType(const short& type);
        void setStatus(const short& status);
        void setValue(const QDateTime& value);
        void setEndTime(const qint64& endTime);
        void setRepeating(const bool& repetitive);
        size_t getId();
        std::string getTitle();
        short getType();
        short getStatus();
        QDateTime getValue();
        qint64 getEndTime();
        bool getRepeating();
    private:
        size_t id;
        std::string title;
        short type;
        short status;
        QDateTime value;
        qint64 endTime;
        bool repeating;
    };
    std::vector<Clock> clocks;
};

#endif
