#include "LineFlowEffect.h"

LineFlowEffect::LineFlowEffect(QWidget *parent) : QWidget(parent)
{
    m_DoubleLine = false;
    m_CurrentForwardPolygonIndex = 0;
    m_CurrentReversePolygonIndex = 0;
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
void LineFlowEffect::SetPosition(QPoint StartPos,QPoint EndPos,int Width)
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

    //三角形总数
    m_PolygonCount = m_HypotenuseLength/m_DoubleLineDistance;
    qDebug()<<"m_PolygonCount:"<<m_PolygonCount;
    m_CurrentForwardPolygonIndex = 0;
    m_CurrentReversePolygonIndex = m_PolygonCount;
    //画箭头时，坐标轴的旋转为顺时针方向

    if(m_PointAStart.x() == m_PointAEnd.x())//Y轴的两个方向
    {
        if(m_PointAStart.y() > m_PointAEnd.y())
        {
            m_Angle = m_Angle+180;
            m_MovementAngle = m_Angle;
        }
    }
    else if(m_PointAStart.y() == m_PointAEnd.y())//X轴的两个方向
    {
        if(m_PointAStart.x()<m_PointAEnd.x())//Y轴右边区域
        {
            m_Angle = -m_Angle;
            m_MovementAngle = -m_Angle;
        }
        else//Y轴左边区域
        {
            m_MovementAngle = -m_Angle;
        }
    }

#if ENABLE_EFFECT_DEBUG_OUTPUT
    qDebug()<<"m_Angle:"<<m_Angle<<" m_MovementAngle:"<<m_MovementAngle;
#endif
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
#if ENABLE_EFFECT_DEBUG_OUTPUT
        qDebug()<<"m_PointAStart.x:"<<m_PointAStart.x()<<" m_PointAStart.y"<<m_PointAStart.y();
        qDebug()<<"m_PointAEnd.x:"<<m_PointAEnd.x()<<" m_PointAEnd.y"<<m_PointAEnd.y();
        qDebug()<<"m_PointBStart.x:"<<m_PointBStart.x()<<" m_PointBStart.y"<<m_PointBStart.y();
        qDebug()<<"m_PointBEnd.x:"<<m_PointBEnd.x()<<" m_PointBEnd.y"<<m_PointBEnd.y();
#endif
    }

}
void LineFlowEffect::SetPosition(QVector<QPoint>LinesVector,int Width)
{
    QPoint BeforePoint = QPoint(0,0);
    int Index = 0;
    QVector<QPoint>::iterator it = LinesVector.begin();
    for(it;it != LinesVector.end();it++)
    {
        BeforePoint.setX(it->x());
        BeforePoint.setY(it->y());
        if(Index%2 !=0)
        {
            Index = 0;
            //两点之间的距离，斜边距离
            m_HypotenuseLength = sqrt(pow(it->x()-BeforePoint.x(),2)+pow(it->y()-BeforePoint.y(),2));
            //X轴上的长度
            int tempXLength = abs(it->x() - BeforePoint.x());
            m_DoubleLineDistance = Width;
            //算出此条线的偏移角度
            m_Angle = asin((double)tempXLength/m_HypotenuseLength)*180/3.1415926;
            //三角形总数
            m_PolygonCount = m_HypotenuseLength/m_DoubleLineDistance;
            qDebug()<<"m_PolygonCount:"<<m_PolygonCount;
            m_CurrentForwardPolygonIndex = 0;
            m_CurrentReversePolygonIndex = m_PolygonCount;
            //画箭头时，坐标轴的旋转为顺时针方向
            if(BeforePoint.x() == it->x())//Y轴的两个方向
            {
                if(BeforePoint.y() > it->y())
                {
                    m_Angle = m_Angle+180;
                    m_MovementAngle = m_Angle;
                }
            }
            else if(BeforePoint.y() == it->y())//X轴的两个方向
            {
                if(BeforePoint.x() < it->x())//Y轴右边区域
                {
                    m_Angle = -m_Angle;
                    m_MovementAngle = -m_Angle;
                }
                else//Y轴左边区域
                {
                    m_MovementAngle = -m_Angle;
                }
            }

        #if ENABLE_EFFECT_DEBUG_OUTPUT
            qDebug()<<"m_Angle:"<<m_Angle<<" m_MovementAngle:"<<m_MovementAngle;
        #endif
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
        #if ENABLE_EFFECT_DEBUG_OUTPUT
                qDebug()<<"m_PointAStart.x:"<<m_PointAStart.x()<<" m_PointAStart.y"<<m_PointAStart.y();
                qDebug()<<"m_PointAEnd.x:"<<m_PointAEnd.x()<<" m_PointAEnd.y"<<m_PointAEnd.y();
                qDebug()<<"m_PointBStart.x:"<<m_PointBStart.x()<<" m_PointBStart.y"<<m_PointBStart.y();
                qDebug()<<"m_PointBEnd.x:"<<m_PointBEnd.x()<<" m_PointBEnd.y"<<m_PointBEnd.y();
        #endif
            }
            LinesCombinedParameter linePara;
            m_LineNumberParaHash.insert(Index/2,linePara);
            continue;
        }
        Index++;

    }
}
void LineFlowEffect::SetArrowStyle(Qt::PenStyle Style,QColor Color)
{
    m_Pen.setWidth(2);//线宽
    m_Pen.setColor(Color);//划线颜色
    m_Pen.setStyle(Style);//线的样式，实线虚线等
    m_Pen.setCapStyle(Qt::FlatCap);//线端点样式
    m_Pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

    m_Brush.setColor(Color);
    m_Brush.setStyle(Qt::SolidPattern);
}
void LineFlowEffect::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setPen(m_Pen);
    painter.drawLine(m_PointAStart,m_PointAEnd);
    //画箭头
    painter.setBrush(m_Brush);
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
    //移动三角形
    painter.setPen(Qt::NoPen);
    for(int i=0;i<3;i++)
    {
        painter.save();
        QBrush brush;
        brush.setColor(QColor(255,128,192,128-40*i));
        brush.setStyle(Qt::SolidPattern);
        painter.setBrush(brush);

        ArrowLeftPoint.setX(-m_DoubleLineDistance/2);
        ArrowLeftPoint.setY(0);
        ArrowRightPoint.setX(m_DoubleLineDistance/2);
        ArrowRightPoint.setY(0);
        ArrowTopPoint.setX(0);
        ArrowTopPoint.setY(m_DoubleLineDistance);
        qreal tempX = m_PointAStart.x()+sin(m_MovementAngle*Pi/180)*(m_HypotenuseLength*(((m_CurrentForwardPolygonIndex-i)>=0?(m_CurrentForwardPolygonIndex-i):0)/(double)m_PolygonCount));
        qreal tempY = m_PointAStart.y()+cos(m_MovementAngle*Pi/180)*(m_HypotenuseLength*(((m_CurrentForwardPolygonIndex-i)>=0?(m_CurrentForwardPolygonIndex-i):0)/(double)m_PolygonCount));
        //qDebug()<<"tempX:"<<tempX<<" tempY:"<<tempY;

        if(i==0 || (tempX != m_PointAStart.x() || tempY != m_PointAStart.y()))
        {
            painter.translate(tempX,tempY);
            painter.rotate(m_Angle);
            QPointF PolygonPoints[]{ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint};
            painter.drawPolygon(PolygonPoints,3);
        }
        painter.restore();
    }
    if(++m_CurrentForwardPolygonIndex>m_PolygonCount-1)//下标控制
    {
        m_CurrentForwardPolygonIndex = 0;
    }

    if(m_DoubleLine)//第二根线
    {
        painter.setPen(m_Pen);
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

        painter.setPen(Qt::NoPen);
        for(int i=0;i<3;i++)
        {
            painter.save();
            QBrush brush;
            brush.setColor(QColor(255,128,192,128-40*i));
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);

            ArrowLeftPoint.setX(-m_DoubleLineDistance/2);
            ArrowLeftPoint.setY(0);
            ArrowRightPoint.setX(m_DoubleLineDistance/2);
            ArrowRightPoint.setY(0);
            ArrowTopPoint.setX(0);
            ArrowTopPoint.setY(m_DoubleLineDistance);
            //求坐标系的移动后的原点,将最初的坐标偏移通过起始点为基准，按长度进行移动
            qreal tempX = m_PointBStart.x()+sin(m_MovementAngle*Pi/180)*(m_HypotenuseLength*(((m_CurrentReversePolygonIndex+i)>=0?(m_CurrentReversePolygonIndex+i):0)/(double)m_PolygonCount));
            qreal tempY = m_PointBStart.y()+cos(m_MovementAngle*Pi/180)*(m_HypotenuseLength*(((m_CurrentReversePolygonIndex+i)>=0?(m_CurrentReversePolygonIndex+i):0)/(double)m_PolygonCount));

            if(i==0 || (tempX != m_PointBStart.x() || tempY != m_PointBStart.y()))
            {
                painter.translate(tempX,tempY);
                painter.rotate(m_Angle+180);
                QPointF PolygonPoints[]{ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint};
                painter.drawPolygon(PolygonPoints,3);
            }
            painter.restore();
        }
        if(--m_CurrentReversePolygonIndex<=0)//下标控制
        {
            m_CurrentReversePolygonIndex = m_PolygonCount;
        }
    }
}

