#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    class Clock
    {
    public:
        Clock(MainWindow *parent);
        Clock(MainWindow *parent, const Clock& clock);
        void set(const size_t& id, const QString& name, const short& type, const short& status,
                 const QTime& value, const qint64& endTime, const bool& repeating);
        void setId(const size_t& id);
        void setName(const QString& name);
        void setType(const short& type);
        void setStatus(const short& status);
        void setValue(const QTime& value);
        void setEndTime(const qint64& endTime);
        void setRepeating(const bool& repetitive);
        size_t getId() const;
        QString getName() const;
        short getType() const;
        short getStatus() const;
        QTime getValue() const;
        qint64 getEndTime() const;
        bool getRepeating() const;

        void printToTable(const size_t& row) const;
    private:
        size_t id;
        QString name;
        short type; //0->Timer, 1->Alarm clock
        short status; //0->off, 1->Active
        QTime value;
        qint64 endTime;
        bool repeating;
    protected:
        MainWindow *parent;
    };

    void addNewClock(Clock* clock);
    void editClock(Clock* clock);
private slots:
    void on_table_customContextMenuRequested(const QPoint &pos);
    void addNewClockWindow();
    void editClockWindow();
    void removeClocks();
private:
    Ui::MainWindow *ui;
    std::vector<Clock> clocks;
};

#endif
