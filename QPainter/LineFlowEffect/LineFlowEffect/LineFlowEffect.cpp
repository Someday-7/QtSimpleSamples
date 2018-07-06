#include "LineFlowEffect.h"

LineFlowEffect::LineFlowEffect(QWidget *parent) : QWidget(parent)
{
    m_DoubleLine = false;
    m_CurrentPolygonIndex = 0;
    m_PolygonCount = 0;
    m_HypotenuseLength = 0.0;

    m_UpdateTimer = new QTimer;
    m_UpdateTimer->setInterval(200);
    connect(m_UpdateTimer,SIGNAL(timeout()),this,SLOT(UpdateTimerSlot()));
}
LineFlowEffect::~LineFlowEffect()
{
    m_UpdateTimer->deleteLater();
}
void LineFlowEffect::StartEffect()
{
    if(!m_UpdateTimer->isActive())
    {
        m_UpdateTimer->start();
    }
}
void LineFlowEffect::UpdateTimerSlot()
{
    update();
}
void LineFlowEffect::SetPosition(QPoint StartPos,QPoint EndPos,int Width,int LeftOrRight)
{
    m_PointAStart = StartPos;
    m_PointAEnd = EndPos;

    //两点之间的距离，斜边距离
    m_HypotenuseLength = sqrt(pow(m_PointAEnd.x()-m_PointAStart.x(),2)+pow(m_PointAEnd.y()-m_PointAStart.y(),2));
    //X轴上的长度
    int tempXLength = abs(m_PointAEnd.x() - m_PointAStart.x());
    m_DoubleLineDistance = Width;
    //算出此条线的偏移角度
    m_Angle = asin((double)tempXLength/m_HypotenuseLength)*180/3.1415926;
    qDebug()<<"m_Angle:"<<m_Angle;
    //三角形总数
    m_PolygonCount = m_HypotenuseLength/m_DoubleLineDistance;
    qDebug()<<"m_PolygonCount:"<<m_PolygonCount;
    m_CurrentPolygonIndex = 0;

    if(LeftOrRight < 0)
    {
        m_Angle = -m_Angle;
    }
    if(Width<=0)
    {
        m_DoubleLine = false;

    }
    else
    {
        m_DoubleLine = true;
        m_PointBStart.setX(StartPos.x()+Width*cos(abs(m_Angle)*3.1415926/180));
        m_PointBStart.setY(StartPos.y()-Width*sin(abs(m_Angle)*3.1415926/180));
        m_PointBEnd.setX(EndPos.x()+Width*cos(abs(m_Angle)*3.1415926/180));
        m_PointBEnd.setY(EndPos.y()-Width*sin(abs(m_Angle)*3.1415926/180));
        qDebug()<<"m_PointAStart.x:"<<m_PointAStart.x()<<" m_PointAStart.y"<<m_PointAStart.y();
        qDebug()<<"m_PointAEnd.x:"<<m_PointAEnd.x()<<" m_PointAEnd.y"<<m_PointAEnd.y();
        qDebug()<<"m_PointBStart.x:"<<m_PointBStart.x()<<" m_PointBStart.y"<<m_PointBStart.y();
        qDebug()<<"m_PointBEnd.x:"<<m_PointBEnd.x()<<" m_PointBEnd.y"<<m_PointBEnd.y();
    }

}
void LineFlowEffect::SetLineStyle(Qt::PenStyle Style)
{
    m_Pen.setWidth(2);//线宽
    m_Pen.setColor(Qt::red);//划线颜色
    m_Pen.setStyle(Style);//线的样式，实线虚线等
    m_Pen.setCapStyle(Qt::FlatCap);//线端点样式
    m_Pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

}
void LineFlowEffect::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setPen(m_Pen);
    painter.drawLine(m_PointAStart,m_PointAEnd);

    QBrush brush;
    brush.setColor(QColor(255,0,0));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    //画箭头
    QPointF ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint;
    ArrowLeftPoint.setX(-m_DoubleLineDistance/2);
    ArrowLeftPoint.setY(0);
    ArrowRightPoint.setX(m_DoubleLineDistance/2);
    ArrowRightPoint.setY(0);
    ArrowTopPoint.setX(0);
    ArrowTopPoint.setY(m_DoubleLineDistance/2);
    painter.save();
    painter.translate(m_PointAEnd.x(),m_PointAEnd.y());
    painter.rotate(m_Angle);
    QPointF PolygonPoints[]{ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint};
    painter.drawPolygon(PolygonPoints,3);
    painter.restore();

    if(m_DoubleLine)//第二根线
    {
        painter.drawLine(m_PointBStart,m_PointBEnd);

        ArrowLeftPoint.setX(-m_DoubleLineDistance/2);
        ArrowLeftPoint.setY(0);
        ArrowRightPoint.setX(m_DoubleLineDistance/2);
        ArrowRightPoint.setY(0);
        ArrowTopPoint.setX(0);
        ArrowTopPoint.setY(m_DoubleLineDistance/2);
        painter.save();
        painter.translate(m_PointBStart.x(),m_PointBStart.y());
        painter.rotate(m_Angle+180);
        QPointF PolygonPoints[]{ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint};
        painter.drawPolygon(PolygonPoints,3);
        painter.restore();
    }
    //移动三角形

    for(int i=0;i<3;i++)
    {
        painter.save();
        QBrush brush;
        brush.setColor(QColor(255,0,0,255-80*i));
        brush.setStyle(Qt::SolidPattern);
        painter.setBrush(brush);

        ArrowLeftPoint.setX(-m_DoubleLineDistance/2);
        ArrowLeftPoint.setY(0);
        ArrowRightPoint.setX(m_DoubleLineDistance/2);
        ArrowRightPoint.setY(0);
        ArrowTopPoint.setX(0);
        ArrowTopPoint.setY(m_DoubleLineDistance/2);
        qreal tempX = m_PointAStart.x()+sin(m_Angle*Pi/180)*(m_HypotenuseLength*(((m_CurrentPolygonIndex-i)>=0?(m_CurrentPolygonIndex-i):0)/(double)m_PolygonCount));
        qreal tempY = m_PointAStart.y()+cos(m_Angle*Pi/180)*(m_HypotenuseLength*(((m_CurrentPolygonIndex-i)>=0?(m_CurrentPolygonIndex-i):0)/(double)m_PolygonCount));
        qDebug()<<"tempX:"<<tempX<<" tempY:"<<tempY;

        if(i==0 || (tempX != m_PointAStart.x() || tempY != m_PointAStart.y()))
        {
            painter.translate(tempX,tempY);
            painter.rotate(m_Angle);
            QPointF PolygonPoints[]{ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint};
            painter.drawPolygon(PolygonPoints,3);
        }
        painter.restore();
    }

    if(++m_CurrentPolygonIndex>m_PolygonCount-1)//下标控制
    {
        m_CurrentPolygonIndex = 0;
    }
}

