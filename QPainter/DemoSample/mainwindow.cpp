#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UpdateTimer = new QTimer;
    UpdateTimer->setInterval(1000);
    connect(UpdateTimer,SIGNAL(timeout()),this,SLOT(UpdateTimerSlot()));
    UpdateTimer->start();

    //设置背景颜色
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,Qt::black);
    this->setPalette(palette);
    this->resize(800,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::UpdateTimerSlot()
{
    update();
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);//创建QPainter对象
    /*
    enum RenderHint {
        Antialiasing = 0x01,//原始物体边沿抗锯齿
        TextAntialiasing = 0x02,//字体抗锯齿
        SmoothPixmapTransform = 0x04,//平滑的像素
        HighQualityAntialiasing = 0x08,//高质量抗锯齿
        NonCosmeticDefaultPen = 0x10,//QPen默认值不可修饰
        Qt4CompatiblePainting = 0x20//基于Qt4规则
    }*/
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    int width = this->width();
    int height = this->height();
    QRect rect(width/4,height/4,width/4,height/4);

    //设置画笔
    QPen pen;
    pen.setWidth(1);//线宽
    pen.setColor(Qt::red);//划线颜色
    pen.setStyle(Qt::SolidLine);//线的样式，实线虚线等
    pen.setCapStyle(Qt::FlatCap);//线端点样式
    pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
    painter.setPen(pen);

    //设置画刷，画笔线包裹的区域设置颜色
    QBrush brush;
    brush.setColor(Qt::yellow);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    //绘制一个矩形区域
    painter.drawRect(rect);

    //QBrush 不同风格
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::DiagCrossPattern);
    painter.setBrush(brush);
    //绘制一个椭圆形区域
    painter.drawEllipse(width-50,height-50,20,50);


    QFont font;
    font.setFamily("黑体");
    font.setBold(false);
    font.setPointSize(16);
    painter.setFont(font);
    //绘制文字，这里是时间
    painter.drawText(QRect(width/4,height/4,width/4,height/4),QTime::currentTime().toString("hh:mm:ss"));

    //画笔的不同样式
    pen.setWidth(3);//线宽
    pen.setColor(Qt::red);//划线颜色
    //style：SolidLine DashLine DotLine 这三种的混合
    pen.setStyle(Qt::DotLine);//线的样式，实线虚线等
    pen.setCapStyle(Qt::RoundCap);//线端点样式
    pen.setJoinStyle(Qt::RoundJoin);//线的连接点样式
    painter.setPen(pen);
    painter.drawLine(0,0,100,100);

    //渐变填充色
    QRadialGradient radialGradient(width-100,0,100,width-100,0);//填充中心点x，填充中心点y，半径，焦点坐标x,焦点坐标y
    radialGradient.setColorAt(0,Qt::green);//辐射中心点，起点，0-1之间可设置中间点颜色例如0.5 Q
    radialGradient.setColorAt(1,Qt::blue);//终点，由起点绿变终点蓝
    radialGradient.setSpread(QGradient::ReflectSpread);//反射传播
    painter.setBrush(radialGradient);
    painter.drawRect(width-100,0,width-80,80);


}
