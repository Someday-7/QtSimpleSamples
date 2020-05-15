#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QDebug>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initComponent();
    initAppointMothCalendar(2020,1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initComponent()
{
    m_TableWidget = new QTableWidget(this);
    this->setCentralWidget(m_TableWidget);
    int iDays = QDate::currentDate().daysInMonth();
    qDebug()<<"iDays:"<<iDays;
    qDebug()<<"What day:"<<QDate::currentDate().dayOfWeek();
}

void MainWindow::initAppointMothCalendar(int iYear,int iMonth)
{
    QDate date;
    if(date.setDate(iYear,iMonth,1))
    {
        int startDayOfWeek = date.dayOfWeek();
        int iDaysOfMonth = date.daysInMonth();
        int iLastDays = iDaysOfMonth - (7 - startDayOfWeek + 1);
        int iRow = 1 + iLastDays/7 + ((iLastDays%7 != 0) ? 1 : 0);
        m_CalendarWidgetMap.clear();
        m_TableWidget->clear();
        m_TableWidget->setColumnCount(7);
        m_TableWidget->setRowCount(iRow);
        m_TableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        m_TableWidget->verticalHeader()->setVisible(false);
        m_TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_TableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_TableWidget->setFocusPolicy(Qt::NoFocus);
        //headerView->setFixedHeight(100);
        QStringList header;
        header<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7";
        m_TableWidget->setHorizontalHeaderLabels(header);
        int iOffset = startDayOfWeek-1;
        for(int i = 1; i <= iDaysOfMonth; ++i)
        {
            QWidget* widget = new QWidget;
            QVBoxLayout* vLayout = new QVBoxLayout;
            QLabel* label = new QLabel;
            label->setText(QString::number(i));
            QComboBox* box = new QComboBox;
            //box->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            box->setProperty("Day",QVariant(i));
            QStringList list;
            list<<"1"<<"2";
            box->addItems(list);
            vLayout->addWidget(label,1,Qt::AlignHCenter);
            vLayout->addWidget(box,1,Qt::AlignHCenter);
            widget->setLayout(vLayout);
            m_TableWidget->setCellWidget((iOffset+i-1)/7,(iOffset+i-1)%7,widget);
            m_CalendarWidgetMap.insert(i,box);
            connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_ComboboxValueChanged(int)));
        }
        qDebug()<<"add month of iDays:"<<iDaysOfMonth;
        qDebug()<<"What day:"<<startDayOfWeek;
        qDebug()<<"Date:"<<date.toString("yyyy-MM-dd");
    }
}

void MainWindow::getCalendarValue()
{
    QMap<int,QComboBox*>::iterator it = m_CalendarWidgetMap.begin();
    for(it;it != m_CalendarWidgetMap.end(); ++it)
    {
        qDebug()<<"iDay:"<<it.key()<<" Type:"<<it.value()->currentText();
    }
}

void MainWindow::slot_ComboboxValueChanged(int index)
{
    QComboBox* box = dynamic_cast<QComboBox*>(sender());
    int iDay = box->property("Day").toInt();
    qDebug()<<"iDay:"<<iDay<<" Type:"<<box->itemText(index);
    qDebug()<<"--------------------";
    getCalendarValue();
}
