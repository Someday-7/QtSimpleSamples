#include "MovementImage.h"
#include <QDebug>
MovementImage::MovementImage(QWidget* parent):QLabel(parent)
{
    m_Animation = NULL;
    m_CurrentDirection = AnimationDirection::BottomToTop;
    m_CurrentRotate = 0;
    qDebug()<<"[MovementImage] New Object";
}
MovementImage::~MovementImage()
{
}
void MovementImage::SetAnimationDirection(AnimationDirection Direction,int RotateValue)
{
    m_CurrentDirection = Direction;
    m_CurrentRotate = RotateValue;
}
void MovementImage::StartAnimation(QRect ShapeRect)
{
    if(!m_Animation)
    {
        delete m_Animation;
        m_Animation = NULL;
    }
    QImage Image;
    if(!Image.load(QString(":/flow/images/Resources/Arrow.png")))
    {
        qDebug()<<"[MovementImage]"<<"Internal Image Load Faild!";
        return;
    }

    QMatrix matrix;
    matrix.rotate(m_CurrentRotate);
    Image = Image.transformed(matrix,Qt::FastTransformation);
    Image = Image.scaled(ShapeRect.width(),ShapeRect.width());
    this->setPixmap(QPixmap::fromImage(Image));
    /*  声明动画类，并将控制对象 this (this一定是继承自QObject的窗口部件)  以及属性名 "geometry" 传入构造函数  */
    m_Animation = new QPropertyAnimation(this, "geometry");
    m_Animation->setDuration(500);

    //int x = 0,y = 0,width = 0,height = ;
    if(m_CurrentDirection == AnimationDirection::TopToBottom)
    {
        m_Animation->setDirection(QAbstractAnimation::Forward);
    }
    else if(m_CurrentDirection == AnimationDirection::BottomToTop)
    {
        m_Animation->setDirection(QAbstractAnimation::Backward);
    }

    //默认垂直向下为初始状态
    if(m_CurrentRotate == 0 || m_CurrentRotate == 180)//x不变
    {
        m_Animation->setKeyValueAt(0.0,QRect(ShapeRect.x()-ShapeRect.width()/2, ShapeRect.y()-ShapeRect.height()/2, ShapeRect.width(),ShapeRect.height()));
        m_Animation->setKeyValueAt(0.2,QRect(ShapeRect.x()-ShapeRect.width()/2, ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height()/6*1, ShapeRect.width(),ShapeRect.height()));
        m_Animation->setKeyValueAt(0.4,QRect(ShapeRect.x()-ShapeRect.width()/2, ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height()/6*2, ShapeRect.width(),ShapeRect.height()));
        m_Animation->setKeyValueAt(0.6,QRect(ShapeRect.x()-ShapeRect.width()/2, ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height()/6*3, ShapeRect.width(),ShapeRect.height()));
        m_Animation->setKeyValueAt(0.8,QRect(ShapeRect.x()-ShapeRect.width()/2, ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height()/6*4, ShapeRect.width(),ShapeRect.height()));
        m_Animation->setKeyValueAt(0.9,QRect(ShapeRect.x()-ShapeRect.width()/2, ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height()/6*5, ShapeRect.width(),ShapeRect.height()));
        m_Animation->setKeyValueAt(1.0,QRect(ShapeRect.x()-ShapeRect.width()/2, ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height(), ShapeRect.width(),ShapeRect.height()));
    }
    else if(m_CurrentRotate == 90 || m_CurrentRotate == 270)//x不变
    {

    }

    /*  设置动画效果  */
    m_Animation->setEasingCurve(QEasingCurve::Linear);
    m_Animation->start(QAbstractAnimation::DeleteWhenStopped);
    m_Animation->setLoopCount(-1);
    //特别说明，控件拥有自己的坐标系，在空间中心x y为0，传入进来的rect属于视图的坐标系，从左上角0 0 开始
    qDebug()<<"------------------------------------------------------------------";
    qDebug()<<"[MovementImage] Original"<<"w:"<<ShapeRect.width()<<"h:"<<ShapeRect.height();
    qDebug()<<"[MovementImage] Original"<<"x:"<<ShapeRect.x()<<"y:"<<ShapeRect.y();

    qDebug()<<"[MovementImage]"<<"x:"<<ShapeRect.x()-ShapeRect.width()/2<<"y:"<<ShapeRect.y()-ShapeRect.height()/2;
    qDebug()<<"[MovementImage]"<<"x:"<<ShapeRect.x()-ShapeRect.width()/2<<"y:"<<ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height()/6*1;
    qDebug()<<"[MovementImage]"<<"x:"<<ShapeRect.x()-ShapeRect.width()/2<<"y:"<<ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height()/6*2;
    qDebug()<<"[MovementImage]"<<"x:"<<ShapeRect.x()-ShapeRect.width()/2<<"y:"<<ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height()/6*3;
    qDebug()<<"[MovementImage]"<<"x:"<<ShapeRect.x()-ShapeRect.width()/2<<"y:"<<ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height()/6*4;
    qDebug()<<"[MovementImage]"<<"x:"<<ShapeRect.x()-ShapeRect.width()/2<<"y:"<<ShapeRect.y()-ShapeRect.height()/2+ShapeRect.height()/6*5;

}
