#include "LineFlowEffect.h"

LineFlowEffect::LineFlowEffect(QWidget *parent) : QWidget(parent)
{
    m_UpdateTimer = new QTimer;
    m_UpdateTimer->setInterval(100);
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
void LineFlowEffect::SetPosition(QPoint StartPos,QPoint EndPos,int Width,int Angle)
{
    m_PointAStart = StartPos;
    m_PointAEnd = EndPos;
    if(Angle<=0&&Angle>=-90)
    {
        m_PointBStart.setX(StartPos.x()+Width*cos(abs(Angle)*3.1415926/180));
        m_PointBStart.setY(StartPos.y()-Width*cos(abs(Angle)*3.1415926/180));
        m_PointBEnd.setX(EndPos.x()+Width*cos(abs(Angle)*3.1415926/180));
        m_PointBEnd.setY(EndPos.y()-Width*sin(abs(Angle)*3.1415926/180));
    }
    qDebug()<<"m_PointAStart.x:"<<m_PointAStart.x()<<" m_PointAStart.y"<<m_PointAStart.y();
    qDebug()<<"m_PointAEnd.x:"<<m_PointAEnd.x()<<" m_PointAEnd.y"<<m_PointAEnd.y();
    qDebug()<<"m_PointBStart.x:"<<m_PointBStart.x()<<" m_PointBStart.y"<<m_PointBStart.y();
    qDebug()<<"m_PointBEnd.x:"<<m_PointBEnd.x()<<" m_PointBEnd.y"<<m_PointBEnd.y();

    m_PipelineWidth = Width;
    m_Angle = Angle;
}
void LineFlowEffect::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QPen pen;
    pen.setWidth(1);//线宽
    pen.setColor(Qt::red);//划线颜色
    pen.setStyle(Qt::SolidLine);//线的样式，实线虚线等
    pen.setCapStyle(Qt::FlatCap);//线端点样式
    pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
    painter.setPen(pen);

    painter.drawLine(m_PointAStart,m_PointAEnd);
    painter.drawLine(m_PointBStart,m_PointBEnd);


}

