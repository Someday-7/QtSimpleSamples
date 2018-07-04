#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_Step = 10;
    m_Interval = 100;
    m_BackGroudColor = QColor(0, 255, 0);

    m_UpdateTimer = new QTimer(this);
    connect(m_UpdateTimer, SIGNAL(timeout()), this, SLOT(UpdateTimerSlot()));
    m_UpdateTimer->start(100);
    m_Offset = 0;
    m_Add = false;
    m_TestLabel = new QLabel;

    SetFontSize(32);
}
void MainWindow::SetFontSize(int Size)
{
    m_Font.setFamily(QStringLiteral("黑体"));
    m_Font.setPointSize(Size);
    m_TestLabel->setFont(m_Font);
    m_FontWidth = m_TestLabel->fontMetrics().width(QChar('A'));
    qDebug()<<"[MainWindow] Font Width:"<<m_FontWidth;
    m_FontHeight = m_TestLabel->fontMetrics().leading();
    qDebug()<<"[MainWindow] Font height:"<<m_FontHeight;
}
MainWindow::~MainWindow()
{
    delete m_TestLabel;
    delete ui;
}
void MainWindow::UpdateTimerSlot()
{
    update();
}
void MainWindow::paintEvent(QPaintEvent *event)
{
   QPainter painter(this);
   // 绘制准备工作 启用反锯齿
   painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
   //绘制背景
   // 半径为当前 宽 或者 高 的一半
   int radius = qMin(this->width(), this->height())/2;
   // 以点为中心的渐变色
   QRadialGradient g(QPoint(radius/2, radius/2), radius);
   // 循环加减
   (m_Offset < 100 && m_Add) ? (m_Offset += m_Step) : (m_Add = false);
   (m_Offset > 0 && !m_Add) ? (m_Offset -= m_Step) : (m_Add = true);
   // 按照 点范围[0.0,1.0] 对于 每点的颜色
   m_BackGroudColor.setAlpha( 200+m_Offset > 255 ? 255 : 200+m_Offset );
   g.setColorAt(0.0, m_BackGroudColor);
   m_BackGroudColor.setAlpha( 140+m_Offset);
   g.setColorAt(0.2, m_BackGroudColor);
   m_BackGroudColor.setAlpha( 80+m_Offset);
   g.setColorAt(0.4, m_BackGroudColor);
   m_BackGroudColor.setAlpha( 20+m_Offset >= 0 ? 20+m_Offset : 0 );
   g.setColorAt(0.6, m_BackGroudColor);
   m_BackGroudColor.setAlpha( -60+m_Offset >= 0 ? -50+m_Offset : 0 );
   g.setColorAt(0.8, m_BackGroudColor);
   m_BackGroudColor.setAlpha( 0 );
   g.setColorAt(1.0, m_BackGroudColor);
   // 设置 画笔 图形的边界线

   painter.setPen(Qt::NoPen);
   // 设置 画刷 画刷为 点向外辐射的渐变色
   painter.setBrush(g);
   // 画椭圆，长=宽 为原型
   painter.drawEllipse(0, 0, radius , radius );

   QPen pen;
   pen.setWidth(3);//线宽
   pen.setColor(Qt::blue);//划线颜色
   pen.setStyle(Qt::SolidLine);//线的样式，实线虚线等
   pen.setCapStyle(Qt::RoundCap);//线端点样式
   pen.setJoinStyle(Qt::RoundJoin);//线的连接点样式

   painter.setFont(m_Font);
   painter.setPen(pen);
   QString text = QString("APS");
   int totalLength = text.length();
   painter.drawText(radius/2 - (m_FontWidth*totalLength)/2, radius/2 + m_FontHeight/2 ,text);
}
