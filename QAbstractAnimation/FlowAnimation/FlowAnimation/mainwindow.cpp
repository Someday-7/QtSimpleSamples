#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

}

MainWindow::~MainWindow()
{

    delete ui;
}
void MainWindow::resizeEvent(QResizeEvent *event)
{

#if 1
    ui->label_VerticalGround->SetAnimationDirection(AnimationDirection::TopToBottom,0);
    ui->label_VerticalGround->StartAnimation(QRect(ui->label_VerticalGround->pos().x(),ui->label_VerticalGround->pos().y(),ui->label_VerticalGround->width(),ui->label_VerticalGround->height()));
#else
    ui->label_VerticalTop->SetAnimationDirection(AnimationDirection::BottomToTop,180);
    ui->label_VerticalTop->StartAnimation(ui->label_VerticalTop->geometry());
#endif
}
