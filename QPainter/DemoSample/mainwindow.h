#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QTime>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void UpdateTimerSlot();
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;//重写paintEvent虚函数
private:
    Ui::MainWindow *ui;
    QTimer* UpdateTimer;
};

#endif // MAINWINDOW_H
