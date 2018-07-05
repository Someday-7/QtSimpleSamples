#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int m_FontSize = 24;
    int m_Interval = 100;
    bool m_IsEnable = true;
    QColor textColor = QColor(255, 255, 0);
    QColor backGroundColorColor = QColor(255, 0, 0);
    QString Tips = QStringLiteral("Test");
    ui->widget->SetUpdateInterval(m_Interval);
    ui->widget->SetShowTextName(Tips, m_FontSize, textColor);
    ui->widget->SetBackGroudColor(backGroundColorColor);
    ui->widget->SetEnableUpdate(m_IsEnable);
}
MainWindow::~MainWindow()
{
    delete ui;
}

