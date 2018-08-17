#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ExcelReader()
{
    QString FilePath = QFileDialog::getOpenFileName(this,"Open Excel",".","excel Files(*.xlsx)");
    if(FilePath.isEmpty())
    {
        return;
    }
    qDebug()<<"file:"<<FilePath;
    QAxObject *excel = NULL;    //本例中，excel设定为Excel文件的操作对象
    QAxObject *workbooks = NULL;
    QAxObject *workbook = NULL;  //Excel操作对象
    excel = new QAxObject("Excel.Application");
    excel->dynamicCall("SetVisible(bool)", true); //true 表示操作文件时可见，false表示为不可见//设置false的时候出bug
    workbooks = excel->querySubObject("WorkBooks");
    if(!workbooks)
    {
        qDebug()<<"[Error] Get WorkBooks Failed!";
        return;
    }
    //————————————————按文件路径打开文件————————————————————
    workbook = workbooks->querySubObject("Open(QString&)", FilePath);
    if(!workbook)
    {
        qDebug()<<"[Error] Open Excel File Failed!";
        return;
    }
    // 获取打开的excel文件中所有的工作sheet
    QAxObject * worksheets = workbook->querySubObject("WorkSheets");
    if(!worksheets)
    {
        qDebug()<<"[Error] Query WorkSheets Failed!";
        return;
    }
    //—————————————————Excel文件中表的个数:——————————————————
    int iWorkSheet = worksheets->property("Count").toInt();
    qDebug() << QString::fromLocal8Bit("Excel文件中表的个数: %1").arg(QString::number(iWorkSheet));

    for(int sheetIndex=1;sheetIndex<=iWorkSheet;sheetIndex++)
    {
        // ————————————————获取第n个工作表 querySubObject("Item(int)", n);——————————
        QAxObject * worksheet = worksheets->querySubObject("Item(int)", sheetIndex);//本例获取第一个，最后参数填1
        if(worksheet)
        {
            //—————————获取该sheet的数据范围（可以理解为有数据的矩形区域）————
            QAxObject * usedrange = worksheet->querySubObject("UsedRange");
            //———————————————————获取行数———————————————
            QAxObject * rows = usedrange->querySubObject("Rows");
            int iRows = rows->property("Count").toInt();
            qDebug() << QString::fromLocal8Bit("行数为: %1").arg(QString::number(iRows));
            //————————————获取列数—————————
            QAxObject * columns = usedrange->querySubObject("Columns");
            int iColumns = columns->property("Count").toInt();
            qDebug() << QString::fromLocal8Bit("列数为: %1").arg(QString::number(iColumns));
            //————————数据的起始行———
            int iStartRow = rows->property("Row").toInt();
            qDebug() << QString::fromLocal8Bit("起始行为: %1").arg(QString::number(iStartRow));
            //————————数据的起始列————————————
            int iColumn = columns->property("Column").toInt();
            qDebug() << QString::fromLocal8Bit("起始列为: %1").arg(QString::number(iColumn));
            //——————————————读出数据—————————————
            //获取第i行第j列的数据
            //假如是第6行，第6列 对应表中F列6行，即F6
            SheetInfo SheetInfoTemp;
            for(int row=iStartRow;row <= iRows;row++)
            {
                QStringList RowList;
                QVector<QString> vTemp;
                for(int column=iColumn;column <= iColumns;column++)
                {
                    QAxObject *cell = worksheet->querySubObject("Cells(int, int)", row, column);
                    QString strValue = cell->dynamicCall("Value2()").toByteArray();
                    RowList << strValue;
                    if(row == iStartRow && column == iColumn)
                    {
                        SheetInfoTemp.m_SheetName = strValue;
                    }
                    else
                    {
                        vTemp.push_back(strValue);
                    }
                }
                //qDebug() << QString::fromLocal8Bit("第%1行的数据为：").arg(QString::number(row))<<RowList;
                if(row == iStartRow+1)
                {
                    SheetInfoTemp.m_vProperty = vTemp;
                }
                else if(row != iStartRow)
                {
                    SheetInfoTemp.m_vvRow.push_back(vTemp);
                }
            }
            m_hSheetInfor.insert(sheetIndex,SheetInfoTemp);
        }
    }
    //!!!!!!!一定要记得close，不然系统进程里会出现n个EXCEL.EXE进程
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    if (excel)
    {
        delete excel;
        excel = NULL;
    }


    QHash<int,SheetInfo>::iterator it = m_hSheetInfor.begin();
    for(it;it != m_hSheetInfor.end();it++)
    {

        SheetInfo tempSheetInfo = it.value();
        qDebug()<<"Sheet:"<<it.key()<<" Type:"<<tempSheetInfo.m_SheetName;
        QVector<QString>::iterator itProperty = tempSheetInfo.m_vProperty.begin();
        QStringList tempList;
        for(itProperty;itProperty != tempSheetInfo.m_vProperty.end();itProperty++)
        {
            tempList<<(*itProperty);
        }
        qDebug()<<"Property:"<<tempList;

        QVector< QVector<QString> >::iterator itRow = tempSheetInfo.m_vvRow.begin();
        qDebug()<<"Row Count:"<<tempSheetInfo.m_vvRow.size();
        for(itRow;itRow != tempSheetInfo.m_vvRow.end();itRow++)
        {
            tempList.clear();
            QVector<QString> vRow = (*itRow);

            QVector<QString>::iterator itOneRow = vRow.begin();
            for(itOneRow;itOneRow != vRow.end();itOneRow++)
            {
                tempList<<(*itOneRow);
            }
            qDebug()<<"Row:"<<tempList;
        }
    }
    GenerateXML();
}
bool MainWindow::GenerateXML()
{
    QString Path = QCoreApplication::applicationDirPath()+QString(QStringLiteral("/ProtocolTest.xml"));
    qDebug()<<"Path:"<<Path;
    QFile file(Path);
    if (!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        qDebug()<<"File Open Failed!";
        return -1;
    }

    QDomDocument document;
    QString strHeader( "version=\"1.0\" encoding=\"UTF-8\"" );
    document.appendChild( document.createProcessingInstruction("xml", strHeader) );
    QDomElement	rootElement = document.createElement( "ProtocolList" );
    document.appendChild(rootElement);

    QHash<int,SheetInfo>::iterator it = m_hSheetInfor.begin();
    for(it;it != m_hSheetInfor.end();it++)
    {

        SheetInfo tempSheetInfo = it.value();
        QDomElement SensorElement = document.createElement( tempSheetInfo.m_SheetName );
        rootElement.appendChild(SensorElement);
        QVector<QString>::iterator itProperty = tempSheetInfo.m_vProperty.begin();
        QStringList propertyList;
        for(itProperty;itProperty != tempSheetInfo.m_vProperty.end();itProperty++)
        {
            propertyList<<(*itProperty);
        }

        QVector< QVector<QString> >::iterator itRow = tempSheetInfo.m_vvRow.begin();
        for(itRow;itRow != tempSheetInfo.m_vvRow.end();itRow++)
        {
            QVector<QString> vRow = (*itRow);
            QDomElement item = document.createElement( "Row" );
            for(int i=0;i<vRow.size() && i<propertyList.size();i++)
            {
                item.setAttribute( propertyList.at(i), vRow[i] );
                SensorElement.appendChild( item );
            }

        }
    }

    QTextStream out( &file );
    document.save( out, 4 );
    file.close();
    qDebug()<<"--FINISH--";
    return 1;
}
