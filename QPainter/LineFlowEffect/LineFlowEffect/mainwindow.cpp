#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LineFlowEffect* LineFlowEffectObj = new LineFlowEffect;
    LineFlowEffectObj->SetPosition(QPoint(100,100),QPoint(100,300),20,-1);
    LineFlowEffectObj->SetLineStyle(Qt::SolidLine);
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
