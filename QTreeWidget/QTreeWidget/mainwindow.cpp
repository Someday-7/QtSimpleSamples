#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
enum    treeColNum{colItem=0, colItemType=1}; //目录树列的编号定义
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_TreeWidget = new QTreeWidget;
    this->setCentralWidget(m_TreeWidget);
    connect(m_TreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(ShowBasisTablesSlot(QTreeWidgetItem*, int)));

    m_TreeWidget->setHeaderHidden(false);//表头
    QStringList headerList;
    headerList<<"column1"<<"column2";
    m_TreeWidget->setHeaderLabels(headerList);
    int index = 0,childIndex = 0;
    for(index;index<3;index++)
    {
        QString rootName= QString("Root:")+QString::number(index);
        QTreeWidgetItem *TopItem = new QTreeWidgetItem;

        TopItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
        TopItem->setCheckState(colItem,Qt::Unchecked);//设置为选中状态

        TopItem->setText(0,rootName);
        m_TreeWidget->addTopLevelItem(TopItem);//添加根节点
        //TopItem->takeChildren();
        for(childIndex = 0;childIndex<3;childIndex++)
        {
            QString childName = QString("child:")+QString::number(childIndex);
            QTreeWidgetItem *childItem = new QTreeWidgetItem(TopItem); //添加子节点
            childItem->setText(colItem,childName);
            childItem->setText(colItemType,QString::number(childIndex));
            childItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
            childItem->setCheckState(colItem,Qt::Unchecked);//设置为选中状态

            TopItem->insertChild(childIndex,childItem);//添加孩子节点
        }
        m_TreeWidget->expandItem(TopItem);//展开root节点
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ShowBasisTablesSlot(QTreeWidgetItem* Item, int Colum)
{
    if(Item->parent()==NULL)
    {
        QString rootName =  Item->text(Colum);
        qDebug()<<"rootName:"<<rootName;
    }
    else
    {
        QString rootName =  Item->parent()->text(0);
        qDebug()<<"rootName:"<<rootName;
        QString childName = Item->text(Colum);
        qDebug()<<"childName:"<<childName;
    }
}
