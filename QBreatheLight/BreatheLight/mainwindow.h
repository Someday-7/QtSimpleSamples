#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QLabel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SetFontSize(int Size);
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void UpdateTimerSlot();
private:
    Ui::MainWindow *ui;
    int m_Step;
    int m_Interval;
    QColor m_BackGroudColor;
    QTimer* m_UpdateTimer;
    int m_Offset;
    bool m_Add;
    QLabel* m_TestLabel;
    QFont m_Font;
    int m_FontWidth;
    int m_FontHeight;
};

#endif // MAINWINDOW_H
