#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    labViewCord = new QLabel(QString::fromLocal8Bit("View 坐标:"));
    labViewCord->setMinimumWidth(150);
    ui->statusBar->addWidget(labViewCord);

    labSceneCord = new QLabel(QString::fromLocal8Bit("Scene 坐标:"));
    labSceneCord->setMinimumWidth(150);
    ui->statusBar->addWidget(labSceneCord);

    labItemCord = new QLabel(QString::fromLocal8Bit("Item 坐标:"));
    labItemCord->setMidLineWidth(150);
    ui->statusBar->addWidget(labItemCord);

    ui->graphicsView->setCursor(Qt::CrossCursor);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);

    QObject::connect(ui->graphicsView,SIGNAL(mouseMovePointSignal(QPoint)),this,SLOT(on_mouseMovePoint(QPoint)));
    QObject::connect(ui->graphicsView,SIGNAL(mouseClickedSignal(QPoint)),this,SLOT(on_mouseClicked(QPoint)));
    iniGraphicsSystem();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_mouseMovePoint(QPoint point)
{
    labViewCord->setText(QString::asprintf("View 坐标: %d,%d",point.x(),point.y()));
    QPointF pointScene = ui->graphicsView->mapToScene(point);//转换到Scene坐标
    labSceneCord->setText(QString::asprintf("Scene 坐标: %.0f,%.0f",pointScene.x(),pointScene.y()));
}
void MainWindow::on_mouseClicked(QPoint point)
{
    QPointF pointScene = ui->graphicsView->mapToScene(point);//转换到Scene坐标
    QGraphicsItem* item = NULL;
    item = scene->itemAt(pointScene,ui->graphicsView->transform());//获取光标下的图形项
    if(item != NULL)
    {
        QPointF pointItem = ui->graphicsView->mapFromScene(pointScene);//转换到Scene坐标
        labItemCord->setText(QString::asprintf("Item 坐标: %.0f,%.0f",pointItem.x(),pointItem.y()));
    }
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
    ui->labViewSize->setText(QString::asprintf("Graphics View 坐标，左上角总是(0,0),宽度=%d,高度=%d",ui->graphicsView->width(),ui->graphicsView->height()));
    QRectF rectF =ui->graphicsView->sceneRect();//Scene的矩形区
    ui->labSceneRect->setText(QString::asprintf("QGraphicsView::sceneRect()=(left,top,width,height)=%.0f,%.0f,%.0f,%.0f",rectF.left(),rectF.top(),rectF.width(),rectF.height()));
}
void MainWindow::iniGraphicsSystem()
{
    QRectF rect(-200,-100,400,200);//左上角坐标，宽度，高度
    scene = new QGraphicsScene(rect);//scene逻辑坐标定义
    ui->graphicsView->setScene(scene);
    //画一个矩形框，大小等于scene
    QGraphicsRectItem* item = new QGraphicsRectItem(rect);
    item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    QPen pen;
    pen.setWidth(2);
    item->setPen(pen);
    scene->addItem(item);
    //画一个位于scene中心的椭圆，测试局部坐标
    QGraphicsEllipseItem* item2 = new QGraphicsEllipseItem(-100,-50,200,100);
    item2->setPos(0,0);
    item2->setBrush(QBrush(Qt::blue));
    item2->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
    scene->addItem(item2);
    //画一个圆，中心位于scene的边缘
    QGraphicsEllipseItem* item3 = new QGraphicsEllipseItem(-50,-50,100,100);
    item2->setPos(rect.right(),rect.bottom());
    item2->setBrush(QBrush(Qt::red));
    item2->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
    scene->addItem(item3);
    scene->clearSelection();
}
