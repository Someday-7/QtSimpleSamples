#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QLabel>
#include "QWGraphicsView.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent* event);
private slots:
    void on_mouseMovePoint(QPoint point);
    void on_mouseClicked(QPoint point);
private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QLabel* labViewCord;
    QLabel* labSceneCord;
    QLabel* labItemCord;
    void iniGraphicsSystem();
};

#endif // MAINWINDOW_H
