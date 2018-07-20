#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHBoxLayout* HLayout = new QHBoxLayout;
    QLabel* Label = new QLabel("Test");
    HLayout->addWidget(Label,Qt::AlignHCenter);


    QDockWidget* Left = new QDockWidget("Left",this);
    QLabel* LabelLeft = new QLabel("TestLeft",Left);
    LabelLeft->setStyleSheet("color:#ff0000");
    Left->setFeatures(QDockWidget::DockWidgetMovable);
    Left->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    Left->setWidget(LabelLeft);
    Left->setStyleSheet("color:#ff0000;background-color:transparent");

    QLabel* LabelRight = new QLabel("TestRight");
    HLayout->addWidget(Label,Qt::AlignHCenter);
    QDockWidget* Right = new QDockWidget("Right",this);
    Right->setWidget(LabelRight);
    Right->setFeatures(QDockWidget::DockWidgetMovable);
    Right->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    QLabel* LabelTop = new QLabel("TestTop");
    QDockWidget* Top = new QDockWidget("Top",this);
    Top->setWidget(LabelTop);
    Top->setFeatures(QDockWidget::DockWidgetMovable);
    Top->setAllowedAreas(Qt::TopDockWidgetArea|Qt::BottomDockWidgetArea);

    QLabel* LabelBottom= new QLabel("TestBottom");
    QDockWidget* Bottom = new QDockWidget("Bottom",this);
    Bottom->setWidget(LabelBottom);
    Bottom->setFeatures(QDockWidget::DockWidgetMovable);
    Bottom->setAllowedAreas(Qt::TopDockWidgetArea|Qt::BottomDockWidgetArea);

    QWidget* MainWidget = new QWidget;
    MainWidget->setLayout(HLayout);
    this->addDockWidget(Qt::LeftDockWidgetArea,Left,Qt::Vertical);
    this->addDockWidget(Qt::RightDockWidgetArea,Right,Qt::Vertical);
    this->addDockWidget(Qt::TopDockWidgetArea,Top,Qt::Horizontal);
    this->addDockWidget(Qt::BottomDockWidgetArea,Bottom,Qt::Horizontal);
    this->setCentralWidget(MainWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
