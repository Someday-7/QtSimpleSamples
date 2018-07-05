/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "movementimage.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    MovementImage *label_VerticalGround;
    QLabel *label_ObliqueAngle;
    QLabel *label_HorizentalRight;
    MovementImage *label_VerticalTop;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(967, 635);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label_VerticalGround = new MovementImage(centralWidget);
        label_VerticalGround->setObjectName(QStringLiteral("label_VerticalGround"));
        label_VerticalGround->setGeometry(QRect(11, 11, 120, 551));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_VerticalGround->sizePolicy().hasHeightForWidth());
        label_VerticalGround->setSizePolicy(sizePolicy);
        label_ObliqueAngle = new QLabel(centralWidget);
        label_ObliqueAngle->setObjectName(QStringLiteral("label_ObliqueAngle"));
        label_ObliqueAngle->setGeometry(QRect(392, 11, 96, 251));
        sizePolicy.setHeightForWidth(label_ObliqueAngle->sizePolicy().hasHeightForWidth());
        label_ObliqueAngle->setSizePolicy(sizePolicy);
        label_HorizentalRight = new QLabel(centralWidget);
        label_HorizentalRight->setObjectName(QStringLiteral("label_HorizentalRight"));
        label_HorizentalRight->setGeometry(QRect(392, 289, 128, 281));
        sizePolicy.setHeightForWidth(label_HorizentalRight->sizePolicy().hasHeightForWidth());
        label_HorizentalRight->setSizePolicy(sizePolicy);
        label_VerticalTop = new MovementImage(centralWidget);
        label_VerticalTop->setObjectName(QStringLiteral("label_VerticalTop"));
        label_VerticalTop->setGeometry(QRect(773, 11, 96, 551));
        sizePolicy.setHeightForWidth(label_VerticalTop->sizePolicy().hasHeightForWidth());
        label_VerticalTop->setSizePolicy(sizePolicy);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 967, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label_VerticalGround->setText(QApplication::translate("MainWindow", "Vertical_Ground", Q_NULLPTR));
        label_ObliqueAngle->setText(QApplication::translate("MainWindow", "ObliqueAngle", Q_NULLPTR));
        label_HorizentalRight->setText(QApplication::translate("MainWindow", "Horizental_Right", Q_NULLPTR));
        label_VerticalTop->setText(QApplication::translate("MainWindow", "Vertical_Top", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
