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
    QRect rect(width/4,height/4,width/2,height/2);

    //设置画笔
    QPen pen;
    pen.setWidth(3);//线宽
    pen.setColor(Qt::red);//划线颜色
    pen.setStyle(Qt::SolidLine);//线的样式，实线虚线等
    pen.setCapStyle(Qt::FlatCap);//线端点样式
    pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
    painter.setPen(pen);

    //设置画刷
    QBrush brush;
    brush.setColor(Qt::yellow);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    //绘图
    painter.drawRect(rect);

    QFont font;
    font.setFamily("黑体");
    font.setBold(false);
    font.setPointSize(16);

    painter.setFont(font);
    painter.drawText(QRect(width/4,height/4,width/4,height/4),QTime::currentTime().toString("hh:mm:ss"));
}
