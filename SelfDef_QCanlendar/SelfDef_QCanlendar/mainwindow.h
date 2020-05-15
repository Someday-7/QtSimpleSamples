#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QComboBox>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initComponent();
    void initAppointMothCalendar(int iYear,int iMonth);
    void getCalendarValue();
private slots:
    void slot_ComboboxValueChanged(int);
private:
    Ui::MainWindow *ui;
    QTableWidget* m_TableWidget;
    QMap<int,QComboBox*> m_CalendarWidgetMap;
};
#endif // MAINWINDOW_H
