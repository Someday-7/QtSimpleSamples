#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    QPixmap *pixw;

    pixw = new QPixmap(ui->label->size());

    pixw->fill(QColor(255,0,0,150));

    ui->label->setPixmap(*pixw);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::mousePressEvent(QMouseEvent *event)

{

    if(event->button()==Qt::LeftButton)//左键拖动窗体
    {

        relativePos = this->pos()- event->globalPos();

    }

    if(event->button()==Qt::MidButton)//中键关闭窗体
    {

        this->close();

    }

    if(event->button()==Qt::RightButton)//右键打开网址或文件
    {

        QDesktopServices::openUrl(QUrl("file:///C:/windows", QUrl::TolerantMode));

      //  QDesktopServices::openUrl(QUrl("http://www.baidu.com"));

    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)//移动窗体

{

    this->move(event->globalPos() + relativePos);

}
