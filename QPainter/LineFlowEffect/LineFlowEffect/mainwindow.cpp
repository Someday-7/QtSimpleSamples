#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LineFlowEffect* LineFlowEffectObj = new LineFlowEffect;
    LineFlowEffectObj->SetPosition(QPoint(0,0),QPoint(100,100),10,-45);
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
