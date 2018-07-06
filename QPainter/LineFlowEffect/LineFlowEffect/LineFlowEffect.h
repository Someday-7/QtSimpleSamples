#ifndef LINEFLOWEFFECT_H
#define LINEFLOWEFFECT_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QWidget>
#include <QHBoxLayout>
#include <math.h>
#include <QDebug>
#define Pi 3.1415926
class LineFlowEffect : public QWidget
{
    Q_OBJECT
public:
    explicit LineFlowEffect(QWidget *parent = nullptr);
    ~LineFlowEffect();
    void SetPosition(QPoint StartPos,QPoint EndPos,int Width,int LeftOrRight);
    void SetLineStyle(Qt::PenStyle Style);
    void StartEffect();
protected:
    void paintEvent(QPaintEvent *event);
public slots:

private slots:
    void UpdateTimerSlot();
signals:

private:
    QPen m_Pen;
    QTimer* m_UpdateTimer;//第一根线的起点
    QPoint m_PointAStart;//第二根线的终点
    QPoint m_PointAEnd;//第二根线的起点
    QPoint m_PointBStart;//第二根线的终点
    QPoint m_PointBEnd;//第二根线的终点
    bool m_DoubleLine;//是否要绘制双线平行
    int m_Angle;//线相对于视图坐标系偏移角度
    int m_DoubleLineDistance;//两根线之间距离
    int m_PolygonCount;//线上面总共有多少三角形
    int m_CurrentPolygonIndex;//当前领头滚动的三角形下标
    double m_HypotenuseLength;
};

#endif // LINEFLOWEFFECT_H
