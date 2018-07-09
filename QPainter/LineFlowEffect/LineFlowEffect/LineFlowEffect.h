#ifndef LINEFLOWEFFECT_H
#define LINEFLOWEFFECT_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QWidget>
#include <QHBoxLayout>
#include <math.h>
#include <QDebug>
#include <QVector>
#include <QHash>
#define Pi 3.1415926
#define ENABLE_EFFECT_DEBUG_OUTPUT 0
typedef enum LineStatus{INPUT = 0,OUTPUT}LineStatus;
typedef struct LinesCombinedParameter
{
    QPoint m_PointAStart;//第一根线的起点
    QPoint m_PointAEnd;//第一根线的终点
    bool m_DoubleLine;//是否要绘制双线平行
    int m_Angle;//线相对于视图坐标系偏移角度
    int m_MovementAngle;//移动的三角形组合，带方向的角度
    int m_DoubleLineDistance;//两根线之间距离
    int m_PolygonCount;//线上面总共有多少三角形
    double m_HypotenuseLength;//线段长度
    LinesCombinedParameter()
    {
        m_PointAStart = QPoint(0,0);//第一根线的起点
        m_PointAEnd = QPoint(0,0);//第一根线的终点
        m_DoubleLine = 0;//是否要绘制双线平行
        m_Angle = 0;//线相对于视图坐标系偏移角度
        m_MovementAngle = 0;//移动的三角形组合，带方向的角度
        m_DoubleLineDistance = 0;//两根线之间距离
        m_PolygonCount = 0;//线上面总共有多少三角形
        m_HypotenuseLength = 0.0;//线段长度
    }
}LinesCombinedParameter;
class LineFlowEffect : public QWidget
{
    Q_OBJECT
public:
    explicit LineFlowEffect(QWidget *parent = nullptr);
    ~LineFlowEffect();
    void SetPosition(QPoint StartPos,QPoint EndPos,int Width);//暂时只支持直角
    void SetPosition(QVector<QPoint>LinesVector,int Width);//暂时只支持直角
    void SetArrowStyle(Qt::PenStyle Style,QColor Color);
    void StartEffect();
    void EnbaleActiveDisplay(LineStatus Line, bool Status);
protected:
    void paintEvent(QPaintEvent *event);
public slots:

private slots:
    void UpdateTimerSlot();
signals:

private:
    QPen m_Pen;
    QPen m_GrayPen;
    QBrush m_Brush;
    QBrush m_GrayBrush;
    QTimer* m_UpdateTimer;//重绘定时器
    QPoint m_PointAStart;//第一根线的起点
    QPoint m_PointAEnd;//第一根线的终点
    QPoint m_PointBStart;//第二根线的起点
    QPoint m_PointBEnd;//第二根线的终点
    bool m_DoubleLine;//是否要绘制双线平行
    int m_Angle;//线相对于视图坐标系偏移角度
    int m_MovementAngle;//移动的三角形组合，带方向的角度
    int m_DoubleLineDistance;//两根线之间距离
    int m_PolygonCount;//线上面总共有多少三角形
    int m_CurrentForwardPolygonIndex;//当前领头滚动的三角形下标正向
    int m_CurrentReversePolygonIndex;//当前领头滚动的三角形下标反向
    double m_HypotenuseLength;//线段长度
    bool m_IsMoreLine;
    QHash<int,LinesCombinedParameter> m_LineNumberParaHash;
    bool CurrentDisplayStatus[2];
};

#endif // LINEFLOWEFFECT_H
