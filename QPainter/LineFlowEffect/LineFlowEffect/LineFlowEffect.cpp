#include "LineFlowEffect.h"

LineFlowEffect::LineFlowEffect(QWidget *parent) : QWidget(parent)
{
    m_DoubleLine = false;
    m_CurrentForwardPolygonIndex = 0;
    m_CurrentReversePolygonIndex = 0;
    m_PolygonCount = 0;
    m_HypotenuseLength = 0.0;
    m_IsMoreLine = false;
    m_LineDirection = 0;
    CurrentDisplayStatus[3] = {false};
    m_MultiLinesPolygonIndex = NULL;
    m_DisplayCurrentLineEffect = 0;
}
LineFlowEffect::~LineFlowEffect()
{
    if(m_MultiLinesPolygonIndex!=NULL)
    {
        delete []m_MultiLinesPolygonIndex;
    }
}
void LineFlowEffect::SetArrowDirection(LineStatus Line)
{
    if(LineStatus::ARROW_IN == Line)
    {
        m_LineDirection = 0;
    }
    else if(LineStatus::ARROW_OUT == Line)
    {
        m_LineDirection = 1;
    }
}
bool LineFlowEffect::GetLineStatus()
{
    return CurrentDisplayStatus[m_LineDirection];
}
void LineFlowEffect::EnbaleActiveDisplay(bool Status)
{
    CurrentDisplayStatus[m_LineDirection] = Status;
}
void LineFlowEffect::SetPosition(QPoint StartPos,QPoint EndPos,int Width)
{
    m_IsMoreLine = false;
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
#if ENABLE_EFFECT_DEBUG_OUTPUT
    qDebug()<<"[Line Effect] m_PolygonCount:"<<m_PolygonCount;
#endif
    m_CurrentForwardPolygonIndex = 0;//由起点方向向前
    m_CurrentReversePolygonIndex = m_PolygonCount;//第二条线由终点反向
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
    qDebug()<<"[Line Effect] m_Angle:"<<m_Angle<<" m_MovementAngle:"<<m_MovementAngle;
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
        qDebug()<<"[Line Effect] m_PointAStart.x:"<<m_PointAStart.x()<<" m_PointAStart.y"<<m_PointAStart.y();
        qDebug()<<"[Line Effect] m_PointAEnd.x:"<<m_PointAEnd.x()<<" m_PointAEnd.y"<<m_PointAEnd.y();
        qDebug()<<"[Line Effect] m_PointBStart.x:"<<m_PointBStart.x()<<" m_PointBStart.y"<<m_PointBStart.y();
        qDebug()<<"[Line Effect] m_PointBEnd.x:"<<m_PointBEnd.x()<<" m_PointBEnd.y"<<m_PointBEnd.y();
#endif
    }
}
void LineFlowEffect::SetPosition(QVector<QPoint>LinesVector,int Width)
{
    m_IsMoreLine = true;
    QPoint BeforePoint = QPoint(0,0);
    int Index = 0;
    QVector<QPoint>::iterator it = LinesVector.begin();
    for(it;it != LinesVector.end();it++)
    {
        if(Index!=0)//初始点不能形成线段
        {
            LinesCombinedParameter linePara;
            //两点之间的距离，斜边距离
            linePara.m_HypotenuseLength = sqrt(pow(it->x()-BeforePoint.x(),2)+pow(it->y()-BeforePoint.y(),2));
            //X轴上的长度
            int tempXLength = abs(it->x() - BeforePoint.x());
            linePara.m_DoubleLineDistance = Width;
            //算出此条线的偏移角度
            linePara.m_Angle = asin((double)tempXLength/linePara.m_HypotenuseLength)*180/3.1415926;
            //三角形总数
            linePara.m_PolygonCount = linePara.m_HypotenuseLength/linePara.m_DoubleLineDistance;
#if ENABLE_EFFECT_DEBUG_OUTPUT
            qDebug()<<"[Line Effect] m_PolygonCount:"<<linePara.m_PolygonCount;
#endif
            //画箭头时，坐标轴的旋转为顺时针方向
            if(BeforePoint.x() == it->x())//Y轴的两个方向
            {
                if(BeforePoint.y() > it->y())
                {
                    linePara.m_Angle = linePara.m_Angle+180;
                    linePara.m_MovementAngle = linePara.m_Angle;
                }
            }
            else if(BeforePoint.y() == it->y())//X轴的两个方向
            {
                if(BeforePoint.x() < it->x())//Y轴右边区域
                {
                    linePara.m_Angle = -linePara.m_Angle;
                    linePara.m_MovementAngle = -linePara.m_Angle;
                }
                else//Y轴左边区域
                {
                    linePara.m_MovementAngle = -linePara.m_Angle;
                }
            }

#if ENABLE_EFFECT_DEBUG_OUTPUT
            qDebug()<<"[Line Effect] m_Angle:"<<linePara.m_Angle<<" m_MovementAngle:"<<linePara.m_MovementAngle;
#endif
            linePara.m_PointAStart.setX(BeforePoint.x());
            linePara.m_PointAStart.setY(BeforePoint.y());
            linePara.m_PointAEnd.setX(it->x());
            linePara.m_PointAEnd.setY(it->y());
#if ENABLE_EFFECT_DEBUG_OUTPUT
            qDebug()<<"[Line Effect] Line Index:"<<Index-1;
#endif
            m_LineNumberParaHash.insert(Index-1,linePara);
        }
        BeforePoint.setX(it->x());
        BeforePoint.setY(it->y());
        Index++;
    }
    if(m_LineNumberParaHash.size()>0)
    {
        m_MultiLinesPolygonIndex = new int[m_LineNumberParaHash.size()];
        memset(m_MultiLinesPolygonIndex,0,sizeof(m_MultiLinesPolygonIndex)*m_LineNumberParaHash.size());
    }
}
void LineFlowEffect::SetArrowStyle(Qt::PenStyle Style,QColor Color)
{
    m_Pen.setWidth(3);//线宽
    m_Pen.setColor(Color);//划线颜色
    m_Pen.setStyle(Style);//线的样式，实线虚线等
    m_Pen.setCapStyle(Qt::FlatCap);//线端点样式
    m_Pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

    m_Brush.setColor(Color);
    m_Brush.setStyle(Qt::SolidPattern);

    //disable style
    m_GrayPen.setWidth(3);//线宽
    m_GrayPen.setColor(Qt::gray);//划线颜色
    m_GrayPen.setStyle(Style);//线的样式，实线虚线等
    m_GrayPen.setCapStyle(Qt::FlatCap);//线端点样式
    m_GrayPen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
    m_GrayBrush.setColor(Qt::gray);
    m_GrayBrush.setStyle(Qt::SolidPattern);
}
void LineFlowEffect::paintEffect(QPainter& painter)
{  
    if(!m_IsMoreLine)
    {
        QPointF ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint;
        if(CurrentDisplayStatus[1])//Output Line1 Enable
        {
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::TextAntialiasing);
            painter.setPen(m_Pen);
            painter.drawLine(m_PointAStart,m_PointAEnd);
            //画箭头
            painter.setPen(Qt::NoPen);
            painter.setBrush(m_Brush);

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
        }
        else//Output disable line1
        {
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::TextAntialiasing);
            painter.setPen(m_GrayPen);
            painter.drawLine(m_PointAStart,m_PointAEnd);
            //画箭头
            painter.setPen(Qt::NoPen);
            painter.setBrush(m_GrayBrush);
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
        }

        if(CurrentDisplayStatus[0])//Input Line2 Enable
        {
            if(m_DoubleLine)//第二根线
            {
                painter.setPen(m_Pen);
                painter.drawLine(m_PointBStart,m_PointBEnd);

                painter.setPen(Qt::NoPen);
                painter.setBrush(m_Brush);
                QPointF ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint;
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
        else//Input disable line2
        {
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::TextAntialiasing);
            painter.setPen(m_GrayPen);
            painter.setBrush(m_GrayBrush);
            painter.drawLine(m_PointBStart,m_PointBEnd);

            painter.setPen(Qt::NoPen);
            QPointF ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint;
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
    }
    else
    {
        if(CurrentDisplayStatus[1])//Output Line Enable(only one line)
        {
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::TextAntialiasing);

            QPointF ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint = QPoint(0,0);
            LinesCombinedParameter tempLinePara = m_LineNumberParaHash.value(m_DisplayCurrentLineEffect);

            m_CurrentForwardPolygonIndex = m_MultiLinesPolygonIndex[m_DisplayCurrentLineEffect];
            painter.setPen(m_Pen);
#if ENABLE_EFFECT_DEBUG_OUTPUT
            qDebug()<<"[Line Effect] Start X:"<<tempLinePara.m_PointAStart.x()<<" Y:"<<tempLinePara.m_PointAStart.y();
            qDebug()<<"[Line Effect] End X:"<<tempLinePara.m_PointAEnd.x()<<" Y:"<<tempLinePara.m_PointAEnd.y();
#endif
            LinesCombinedParameter tempDrawLine;
            QHash<int,LinesCombinedParameter>::iterator it =  m_LineNumberParaHash.begin();
            for(it;it != m_LineNumberParaHash.end(); it++)//画出所有线段
            {
                tempDrawLine = it.value();
                painter.drawLine(tempDrawLine.m_PointAStart,tempDrawLine.m_PointAEnd);
            }
            //移动三角形
            ArrowLeftPoint.setX(-tempLinePara.m_DoubleLineDistance/2);
            ArrowLeftPoint.setY(0);
            ArrowRightPoint.setX(tempLinePara.m_DoubleLineDistance/2);
            ArrowRightPoint.setY(0);
            ArrowTopPoint.setX(0);
            ArrowTopPoint.setY(tempLinePara.m_DoubleLineDistance);
            QBrush brush;
            brush.setStyle(Qt::SolidPattern);
            painter.setPen(Qt::NoPen);
            for(int i=0;i<3;i++)
            {
                painter.save();
                brush.setColor(QColor(255,128,192,128-40*i));
                painter.setBrush(brush);

                qreal tempX = tempLinePara.m_PointAStart.x()+sin(tempLinePara.m_MovementAngle*Pi/180)*(tempLinePara.m_HypotenuseLength*(((m_CurrentForwardPolygonIndex-i)>=0?(m_CurrentForwardPolygonIndex-i):0)/(double)tempLinePara.m_PolygonCount));
                qreal tempY = tempLinePara.m_PointAStart.y()+cos(tempLinePara.m_MovementAngle*Pi/180)*(tempLinePara.m_HypotenuseLength*(((m_CurrentForwardPolygonIndex-i)>=0?(m_CurrentForwardPolygonIndex-i):0)/(double)tempLinePara.m_PolygonCount));
                //qDebug()<<"tempX:"<<tempX<<" tempY:"<<tempY;

                if(i==0 || (tempX != tempLinePara.m_PointAStart.x() || tempY != tempLinePara.m_PointAStart.y()))
                {
                    painter.translate(tempX,tempY);
                    painter.rotate(tempLinePara.m_Angle);
                    QPointF PolygonPoints[]{ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint};
                    painter.drawPolygon(PolygonPoints,3);
                }
                painter.restore();
            }
            if(++m_CurrentForwardPolygonIndex>tempLinePara.m_PolygonCount-1)//下标控制，移动箭头所在线段控制
            {
                m_CurrentForwardPolygonIndex = 0;
                m_MultiLinesPolygonIndex[m_DisplayCurrentLineEffect] = 0;
                if(++m_DisplayCurrentLineEffect >= m_LineNumberParaHash.size())
                {
                    m_DisplayCurrentLineEffect = 0;
                    m_MultiLinesPolygonIndex[0] = 0;
                }
            }
            else
            {
                m_MultiLinesPolygonIndex[m_DisplayCurrentLineEffect] = m_CurrentForwardPolygonIndex;
            }

            //qDebug()<<"[Line Effect] index:"<<m_DisplayCurrentLineEffect<<" "<<m_CurrentForwardPolygonIndex;
            painter.setPen(Qt::NoPen);
            //箭头
            tempLinePara =  m_LineNumberParaHash[m_LineNumberParaHash.size()-1];
            painter.setBrush(m_Brush);
            ArrowLeftPoint.setX(-tempLinePara.m_DoubleLineDistance/2);
            ArrowLeftPoint.setY(0);
            ArrowRightPoint.setX(tempLinePara.m_DoubleLineDistance/2);
            ArrowRightPoint.setY(0);
            ArrowTopPoint.setX(0);
            ArrowTopPoint.setY(tempLinePara.m_DoubleLineDistance/2);
            painter.save();
            painter.translate(tempLinePara.m_PointAEnd.x(),tempLinePara.m_PointAEnd.y());
            painter.rotate(tempLinePara.m_Angle);
            QPointF APolygonPoints[]{ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint};
            painter.drawPolygon(APolygonPoints,3);
            painter.restore();
        }
        else//disabled
        {
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::TextAntialiasing);

            QHash<int,LinesCombinedParameter>::iterator it =  m_LineNumberParaHash.begin();
            LinesCombinedParameter tempLinePara;
            QPointF ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint = QPoint(0,0);
            for(it;it != m_LineNumberParaHash.end(); it++)
            {
                painter.setPen(m_GrayPen);
                tempLinePara = it.value();
                painter.drawLine(tempLinePara.m_PointAStart,tempLinePara.m_PointAEnd);
            }
            painter.setPen(Qt::NoPen);
            //第一根线
            tempLinePara =  m_LineNumberParaHash[m_LineNumberParaHash.size()-1];
            painter.setBrush(m_GrayBrush);
            ArrowLeftPoint.setX(-tempLinePara.m_DoubleLineDistance/2);
            ArrowLeftPoint.setY(0);
            ArrowRightPoint.setX(tempLinePara.m_DoubleLineDistance/2);
            ArrowRightPoint.setY(0);
            ArrowTopPoint.setX(0);
            ArrowTopPoint.setY(tempLinePara.m_DoubleLineDistance/2);
            painter.save();
            painter.translate(tempLinePara.m_PointAEnd.x(),tempLinePara.m_PointAEnd.y());
            painter.rotate(tempLinePara.m_Angle);
            QPointF APolygonPoints[]{ArrowLeftPoint,ArrowRightPoint,ArrowTopPoint};
            painter.drawPolygon(APolygonPoints,3);
            painter.restore();
        }
    }
}

