#ifndef QWGRAPHICSVIEW_H
#define QWGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWidget>
class QWGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QWGraphicsView(QWidget *parent = nullptr);
    ~QWGraphicsView();
signals:
    void mouseMovePointSignal(QPoint point);
    void mouseClickedSignal(QPoint point);
public slots:

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

};

#endif // QWGRAPHICSVIEW_H
