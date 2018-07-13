#include "QWGraphicsView.h"
#include <qDebug>
QWGraphicsView::QWGraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}
QWGraphicsView::~QWGraphicsView()
{

}
void QWGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    QPoint point = event->pos();
    emit mouseMovePointSignal(point);
    QGraphicsView::mouseMoveEvent(event);
}
void QWGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint point = event->pos();//QGraphicsView的坐标
        emit mouseClickedSignal(point);
    }
    else
    {
        qDebug()<<"[QWGraphicsView] button:"<<event->button();
    }
    QGraphicsView::mousePressEvent(event);
}
