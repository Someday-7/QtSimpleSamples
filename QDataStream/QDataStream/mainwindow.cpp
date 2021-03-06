#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QDataStream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_StandardItemModel = new QStandardItemModel;
    connect(ui->btnSave,SIGNAL(clicked()),this,SLOT(slot_BtnClicked()));
    connect(ui->btnLoad,SIGNAL(clicked()),this,SLOT(slot_BtnClicked()));
    initTableData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTableData()
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            m_StandardItemModel->setItem(i,j,new QStandardItem(QString::number(i*j)));
        }
    }
    ui->tableView_Save->setModel(m_StandardItemModel);

}

void MainWindow::slot_BtnClicked()
{
    QPushButton* btn = dynamic_cast<QPushButton*>(sender());
    if(btn == ui->btnSave)
    {
#if STREAM_TYPE_STM_DAT_MODE
        QString strFileName = QFileDialog::getOpenFileName(this,"打开一个文件",QDir::currentPath(),"Qt 预定义编码数据文件(*.stm)");
        if(strFileName.isEmpty())
        {
            return;
        }
        QFile file(strFileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return;
        }
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_9);
        qint16 rowCount = m_StandardItemModel->rowCount();
        qint16 columCount = m_StandardItemModel->columnCount();
        stream << rowCount;//写入文件流
        stream << columCount;//写入文件流
        for(int i = 0; i < rowCount; ++i)
        {
            for(int j = 0; j < columCount; ++j)
            {
                QStandardItem* item = m_StandardItemModel->item(i,j);
                stream << item->data(Qt::DisplayRole).toString();//字符串写入文件流
            }
        }
        file.close();
#else
        QString strFileName = QFileDialog::getOpenFileName(this,"打开一个文件",QDir::currentPath(),"Qt 预定义编码数据文件(*.dat)");
        if(strFileName.isEmpty())
        {
            return;
        }
        QFile file(strFileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return;
        }
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_9);
        stream.setByteOrder(QDataStream::LittleEndian);
        qint16 rowCount = m_StandardItemModel->rowCount();
        qint16 columCount = m_StandardItemModel->columnCount();
        stream.writeRawData((char*)&rowCount,sizeof(qint16));//写入文件流
        stream.writeRawData((char*)&columCount,sizeof(qint16));//写入文件流
        for(int i = 0; i < rowCount; ++i)
        {
            for(int j = 0; j < columCount; ++j)
            {
                QStandardItem* item = m_StandardItemModel->item(i,j);
                QByteArray itemByte = item->data(Qt::DisplayRole).toByteArray();
                stream.writeBytes(itemByte.data(),itemByte.length());//字符串写入文件流
            }
        }
        file.close();
#endif
    }
    else if(btn == ui->btnLoad)
    {
        m_StandardItemModel->clear();
#if STREAM_TYPE_STM_DAT_MODE
        QString strFileName = QFileDialog::getOpenFileName(this,"打开一个文件",QDir::currentPath(),"Qt 预定义编码数据文件(*.stm)");
        if(strFileName.isEmpty())
        {
            return;
        }
        QFile file(strFileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            return;
        }
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_9);
        qint16 rowCount = m_StandardItemModel->rowCount();
        qint16 columCount = m_StandardItemModel->columnCount();
        stream >> rowCount;
        stream >> columCount;
        for(int i = 0; i < rowCount; ++i)
        {
            for(int j = 0; j < columCount; ++j)
            {
                QString strTemp;
                stream >> strTemp;
//                QModelIndex index = m_StandardItemModel->index(i,j);
//                QStandardItem* item =  m_StandardItemModel->itemFromIndex(index);
//                item->setData(strTemp,Qt::DisplayRole);
                m_StandardItemModel->setItem(i,j,new QStandardItem(strTemp));

            }
        }
        ui->tableView_Read->setModel(m_StandardItemModel);
        file.close();
#else
        QString strFileName = QFileDialog::getOpenFileName(this,"打开一个文件",QDir::currentPath(),"Qt 预定义编码数据文件(*.dat)");
        if(strFileName.isEmpty())
        {
            return;
        }
        QFile file(strFileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            return;
        }
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_9);
        stream.setByteOrder(QDataStream::LittleEndian);
        qint16 rowCount = m_StandardItemModel->rowCount();
        qint16 columCount = m_StandardItemModel->columnCount();
        stream.readRawData((char*)&rowCount,sizeof(qint16));
        stream.readRawData((char*)&columCount,sizeof(qint16));
        char* buf = NULL;
        uint iLen = 0;
        for(int i = 0; i < rowCount; ++i)
        {
            for(int j = 0; j < columCount; ++j)
            {
                stream.readBytes(buf,iLen);
                m_StandardItemModel->setItem(i,j,new QStandardItem(QString::fromLocal8Bit(buf,iLen)));
            }
        }
        ui->tableView_Read->setModel(m_StandardItemModel);
        file.close();
#endif
    }
}

