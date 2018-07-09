#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LineFlowEffect* LineFlowEffectObj = new LineFlowEffect;
#if 0
    LineFlowEffectObj->SetPosition(QPoint(300,100),QPoint(100,100),20);//直角
    LineFlowEffectObj->EnbaleActiveDisplay(LineStatus::OUTPUT,false);
    LineFlowEffectObj->EnbaleActiveDisplay(LineStatus::INPUT,false);
#else
    QVector<QPoint> Lines;
    Lines.push_back(QPoint(100,300));
    Lines.push_back(QPoint(300,300));
    Lines.push_back(QPoint(300,100));
    LineFlowEffectObj->SetPosition(Lines,20);
    LineFlowEffectObj->EnbaleActiveDisplay(LineStatus::OUTPUT,true);
#endif
    LineFlowEffectObj->SetArrowStyle(Qt::DotLine,QColor(2,211,254));
    LineFlowEffectObj->StartEffect();
    QWidget* widget = new QWidget;
    QHBoxLayout* mainLayOut = new QHBoxLayout;
    mainLayOut->addWidget(LineFlowEffectObj);
    widget->setLayout(mainLayOut);
    this->setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
