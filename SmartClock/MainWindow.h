#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QTime>



QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
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
        Clock(const Clock& clock);
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
        MainWindow* getParent() const;

        void printToTable(const size_t& row) const;
        void delayBySec(const qint64& seconds);
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

    std::vector<size_t> getSelected() const;
    void addNewClock(Clock* clock, const bool& updateTable);
    void editClock(Clock* clock, const bool& updateTable);
    void removeClocks(const std::vector<size_t>& indices, const bool& updateTable);
    void startClocks(const std::vector<size_t>& indices, const bool& updateTable);
    void stopClocks(const std::vector<size_t>& indices, const bool& updateTable);

    void updateTable();
private slots:    
    void addNewClockWindow_slot();
    void editClockWindow_slot();
    void removeClocks_slot();
    void startClocks_slot();
    void stopClocks_slot();

    void counting();
    void on_table_customContextMenuRequested(const QPoint &pos);
    void on_addNewTool_triggered();
    void on_table_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    std::vector<Clock> clocks;
    size_t indexOfClosest;
    bool isClosestExists;
    size_t currentIndex;

    bool eventFilter(QObject *obj, QEvent *ev);
    void addNewClockWindow();
    void editClockWindow(const size_t& index);
};

std::ostream& operator <<(std::ostream &in, const MainWindow::Clock &clock);
std::istream& operator >>(std::istream &out, MainWindow::Clock &clock);

#endif
