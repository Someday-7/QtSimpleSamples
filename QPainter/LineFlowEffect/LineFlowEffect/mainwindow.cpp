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
    LineFlowEffectObj->SetArrowDirection(LineStatus::ARROW_OUT);
    LineFlowEffectObj->EnbaleActiveDisplay(true);
#else
    QVector<QPoint> Lines;
    Lines.push_back(QPoint(100,300));
    Lines.push_back(QPoint(300,300));
    Lines.push_back(QPoint(300,100));
    LineFlowEffectObj->SetPosition(Lines,20);
    LineFlowEffectObj->SetArrowDirection(LineStatus::ARROW_OUT);
    LineFlowEffectObj->EnbaleActiveDisplay(true);
#endif
    LineFlowEffectObj->SetArrowStyle(Qt::DotLine,QColor(2,211,254));
    DataTypeLineEffectHash.insert(QStringLiteral("Test"),LineFlowEffectObj);

    QWidget* widget = new QWidget;
    QHBoxLayout* mainLayOut = new QHBoxLayout;
    mainLayOut->addWidget(LineFlowEffectObj);
    widget->setLayout(mainLayOut);
    this->setCentralWidget(widget);

    m_UpdateTimer = new QTimer;
    m_UpdateTimer->setInterval(200);
    QObject::connect(m_UpdateTimer,SIGNAL(timeout()),this,SLOT(UpdateTimerSlot()));
    m_UpdateTimer->start();
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
    QPainter painter(this);
    if(DataTypeLineEffectHash.size()>0)
    {
        QHash<QString,LineFlowEffect*>::iterator it = DataTypeLineEffectHash.begin();
        for(it;it!=DataTypeLineEffectHash.end();it++)
        {
             it.value()->paintEffect(painter);
        }
   }
}

