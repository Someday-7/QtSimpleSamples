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
    QRect rect(0,0,width/6,height/6);

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
    painter.drawEllipse(width/3,width/3,100,100);


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
    painter.drawRect(width/2,0,width/2,80);
#if 0
//画五角星
    //生成五角星的5个顶点坐标，假设原点在五角星中心
    qreal R = 100;//半径
    const qreal Pi = 3.1415926;
    qreal deg = Pi*72/180;
    QPoint points[5] =  {
      QPoint(R,0),
      QPoint(R*cos(deg),-R*sin(deg)),
      QPoint(R*cos(2*deg),-R*sin(2*deg)),
      QPoint(R*cos(3*deg),-R*sin(3*deg)),
      QPoint(R*cos(4*deg),-R*sin(4*deg))
    };
    //设置字体
    font.setPointSize(12);
    font.setBold(false);
    painter.setFont(font);
    //设置画笔
    pen.setWidthF(2); //线宽
    pen.setColor(Qt::red);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    painter.setPen(pen);
    //设置画刷
    brush.setColor(Qt::yellow);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    //设置五角星的painterpath，以便重复使用
    QPainterPath startPath;
    startPath.moveTo(points[0]);
    startPath.lineTo(points[2]);
    startPath.lineTo(points[4]);
    startPath.lineTo(points[1]);
    startPath.lineTo(points[3]);
    startPath.closeSubpath();//闭合路径，最后一个点与第一个点相连
    startPath.addText(points[0],font,"0");
    startPath.addText(points[1],font,"1");
    startPath.addText(points[2],font,"2");
    startPath.addText(points[3],font,"3");
    startPath.addText(points[4],font,"4");

    //绘图 start 1
    painter.save();//保存坐标状态
    painter.translate(100,200);//平移
    painter.drawPath(startPath);
    painter.drawText(0,0,"Start 1");
    painter.restore();//恢复坐标状态

    //绘图 start 2
    painter.translate(300,120);//平移
    painter.scale(0.8,0.8);//缩放
    painter.rotate(90);//顺时针旋转90度
    painter.drawPath(startPath);
    painter.drawText(0,0,"Start 2");

    //绘图 start 3
    painter.resetTransform();//恢复所有坐标变换
    painter.translate(500,120);//平移
    painter.rotate(-145);//逆时针旋转145度
    painter.drawPath(startPath);
    painter.drawText(0,0,"Start 3");

    painter.resetTransform();//恢复所有坐标变换
#else
//使用窗口坐标绘图
    int side = qMin(width,height);
    QRect viewRect((width-side)/2,(height-side)/2,side,side);//viewport 矩形区
    painter.drawRect(viewRect);
    painter.setViewport(viewRect);//设置viewport
    painter.setWindow(-100,-100,200,200);//设置窗口大小
    //设置画笔
    pen.setWidth(1);//线宽
    pen.setColor(Qt::red);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    for(int i=0;i<36;i++)
    {
        painter.drawEllipse(QPoint(50,0),50,50);
        painter.rotate(10);
    }
#endif
}
