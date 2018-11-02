#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitStyleSheet();
    m_ProcessValue = 0;
    ui->textBrowser->setReadOnly(true);
    ui->progressBar->setValue(0);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slot_OpenExcelFile()));
    QStringList list;
    list<<QStringLiteral("无操作")
        <<QStringLiteral("生成参数配置文件_事件型消息")
        <<QStringLiteral("生成参数配置文件_传感器数据")
        <<QStringLiteral("生成参数配置文件_综导输出数据")
          <<QStringLiteral("生成参数配置文件_结构体综导")
        <<QStringLiteral("生成参数结构体文件")
        <<QStringLiteral("生成参数名称列表文件")
        <<QStringLiteral("生成结构体文件");
    ui->comboBox->addItems(list);
    connect(ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_ChooseFileProgram(QString)));
    ui->comboBox->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::InitStyleSheet()
{
    this->setWindowTitle(QStringLiteral("EXCEL转数据结构文件转换器"));
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor(66,65,71));
    this->setPalette(palette);

    this->setStyleSheet("\
                        QPushButton{color:#ffffff; border: 1px solid; border-radius:10px;background-color:transparent;}\
                        QPushButton::hover{background-color:#468552;}\
                        QListView{border: 1px solid #191970; border-radius:10px;background-color: transparent;color:#191970;}\
                        QTextBrowser{border: 1px solid #ffffff; border-radius:10px;background-color: transparent;color:#ffffff;}\
                        QComboBox QAbstractItemView {border: 1px solid white; color:green; selection-color: white; selection-background-color:#ACACFF; background-color:white;}\
                        QComboBox{text-align:center; color:#ffffff; min-width: 4em; background-color:#468552;}\
                        QLabel{color:#ffffff;}\
                        QScrollBar::vertical { background: rgba(ff,ff,88,0%); padding-top:20px; padding-bottom:20px; margin:0px,0px,0px,0px;}\
                        QScrollBar::handle:vertical{background: rgba(0,0,0,15%); width: 10px; border-radius:4px; min-height:10}\
                        QTableView{color:#ffffff;text-align: center;border: 0px solid #191970; border-radius:10px;background-color: transparent;}\
                        QTableView::item:hover{color:#ffffff;background-color:transparent;}\
                        QTableView::item:selected{color:#00ff00;background-color:transparent;}\
                        QTabBar::tab{color:#191970;text-align: center;}\
                        QToolButton{background-color:transparent;border:0px solid;color:#ffffff;}\
                        QDockWidget{background-color:#333333;color:#ffffff}\
                        QDockWidget>QWidget{border: 1px solid #000000;}\
                        QProgressBar{background-color: transparent;color:#ffffff;}\
                        ");
}
void MainWindow::SetProcess(QString Log,int Value)
{
    ui->textBrowser->append(Log);
    if(Value>100)
        Value = 100;
    ui->progressBar->setValue(Value);
}
void MainWindow::slot_ChooseFileProgram(QString CurrentText)
{
    if(CurrentText == QStringLiteral("生成参数配置文件_事件型消息"))
    {
        GenerateXML_EventMsg();
    }
    else if(CurrentText == QStringLiteral("生成参数配置文件_传感器数据"))
    {
        GenerateXML_SensorFloat();
    }
    else if(CurrentText == QStringLiteral("生成参数配置文件_综导输出数据"))
    {
        GenerateXML_IntegratedOutput();
    }
    else if(CurrentText == QStringLiteral("生成参数配置文件_结构体综导"))
    {
        GenerateStructFile_TypeName();
    }
    else if(CurrentText == QStringLiteral("生成参数结构体文件"))
    {
        GenerateStructXML(QStringLiteral("DDS_EVENT_MSG_"));
    }
    else if(CurrentText == QStringLiteral("生成参数名称列表文件"))
    {
         GenerateParaName();
    }
    else if(CurrentText == QStringLiteral("生成结构体文件"))
    {
        GenerateStructFile();
    }
    else
    {

    }
}
void MainWindow::slot_OpenExcelFile()
{
    m_ProcessValue = 0;
    QString FilePath = QFileDialog::getOpenFileName(this,"Open Excel",".","excel Files(*.xlsx)");
    if(FilePath.isEmpty())
    {
        QString log = QString("[Error] Open File:%1").arg(FilePath);
        SetProcess(log,m_ProcessValue++);
        return;
    }
    ui->comboBox->setEnabled(true);
    ui->label_FileName->setText(FilePath.split("/").last());
    ExcelReader(FilePath);
}
void MainWindow::ExcelReader(QString FilePath)
{
    QString log = QString("[OK] Open File:%1").arg(FilePath);
    SetProcess(log,m_ProcessValue++);
    QAxObject *excel = NULL;    //本例中，excel设定为Excel文件的操作对象
    QAxObject *workbooks = NULL;
    QAxObject *workbook = NULL;  //Excel操作对象
    excel = new QAxObject("Excel.Application");
    excel->dynamicCall("SetVisible(bool)", true); //true 表示操作文件时可见，false表示为不可见//设置false的时候出bug
    workbooks = excel->querySubObject("WorkBooks");
    if(!workbooks)
    {
        QString log = QString("[Error] Excel Query WorkBooks");
        SetProcess(log,m_ProcessValue++);
        return;
    }
    log = QString("[OK] Excel Query WorkBooks");
    SetProcess(log,m_ProcessValue++);
    //————————————————按文件路径打开文件————————————————————
    workbook = workbooks->querySubObject("Open(QString&)", FilePath);
    if(!workbook)
    {
        QString log = QString("[Error] Open Excel");
        SetProcess(log,m_ProcessValue++);
        return;
    }
    log = QString("[OK] Open Excel");
    SetProcess(log,m_ProcessValue++);
    // 获取打开的excel文件中所有的工作sheet
    QAxObject * worksheets = workbook->querySubObject("WorkSheets");
    if(!worksheets)
    {
        log = QString("[OK] Query WorkSheets");
        SetProcess(log,m_ProcessValue++);
        return;
    }
    m_hSheetInfor.clear();
    log = QString("[OK] Query WorkSheets");
    SetProcess(log,m_ProcessValue++);
    //—————————————————Excel文件中表的个数:——————————————————
    int iWorkSheet = worksheets->property("Count").toInt();
    log = QString("[OK] ")+QString::fromLocal8Bit("Excel文件中表的个数: %1").arg(QString::number(iWorkSheet));
    SetProcess(log,m_ProcessValue++);
    for(int sheetIndex=1;sheetIndex<=iWorkSheet;sheetIndex++)
    {
        // ————————————————获取第n个工作表 querySubObject("Item(int)", n);——————————
        QAxObject * worksheet = worksheets->querySubObject("Item(int)", sheetIndex);//本例获取第一个，最后参数填1
        log = QString("[OK] ")+QString::fromLocal8Bit("Excel Parser sheet_%1").arg(QString::number(sheetIndex));
        SetProcess(log,(m_ProcessValue += 5));
        if(worksheet)
        {
            //—————————获取该sheet的数据范围（可以理解为有数据的矩形区域）————
            QAxObject * usedrange = worksheet->querySubObject("UsedRange");
            //———————————————————获取行数———————————————
            QAxObject * rows = usedrange->querySubObject("Rows");
            int iRows = rows->property("Count").toInt();
            //qDebug() << QString::fromLocal8Bit("行数为: %1").arg(QString::number(iRows));
            //————————————获取列数—————————
            QAxObject * columns = usedrange->querySubObject("Columns");
            int iColumns = columns->property("Count").toInt();
            //qDebug() << QString::fromLocal8Bit("列数为: %1").arg(QString::number(iColumns));
            //————————数据的起始行———
            int iStartRow = rows->property("Row").toInt();
            //qDebug() << QString::fromLocal8Bit("起始行为: %1").arg(QString::number(iStartRow));
            //————————数据的起始列————————————
            int iColumn = columns->property("Column").toInt();
            //qDebug() << QString::fromLocal8Bit("起始列为: %1").arg(QString::number(iColumn));
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
                    if(row == iStartRow && column == iColumn)//第一行，标志标志数据类型
                    {
                        QStringList titleList = strValue.split("-");
                        //qDebug()<<"titleList:"<<titleList;
                        SheetInfoTemp.m_SheetName = titleList.at(0);
                        SheetInfoTemp.m_SheetDescriptionName = titleList.at(1);
                        if(titleList.count()>2)
                            SheetInfoTemp.m_SheetTopics = titleList.at(2);
                    }
                    else
                    {
                        vTemp.push_back(strValue);
                    }
                }
                //qDebug() << QString::fromLocal8Bit("第%1行的数据为：").arg(QString::number(row))<<RowList;
                if(row == iStartRow+1)//参数标题栏
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
        log = QString("[Display] ")+"Sheet:"+QString::number(it.key())+" Type:"+tempSheetInfo.m_SheetName+" Description:"+tempSheetInfo.m_SheetDescriptionName;
        SetProcess(log,m_ProcessValue);
        QVector<QString>::iterator itProperty = tempSheetInfo.m_vProperty.begin();
        QStringList tempList;
        QString logList;
        for(itProperty;itProperty != tempSheetInfo.m_vProperty.end();itProperty++)
        {
            tempList<<(*itProperty);
            logList.append(*itProperty).append(" ");
        }
        //qDebug()<<"Property:"<<tempList;
        log = QString("[Property] ")+logList;
        logList.clear();
        SetProcess(log,m_ProcessValue);
        log = QString("[Parameter] Count:")+QString::number(tempSheetInfo.m_vvRow.size());
        SetProcess(log,m_ProcessValue);
        for(int index = 0;index < tempSheetInfo.m_vvRow.size();index++)
        {
            tempList.clear();
            QVector<QString> vRow = tempSheetInfo.m_vvRow.at(index);

            QVector<QString>::iterator itOneRow = vRow.begin();
            for(itOneRow;itOneRow != vRow.end();itOneRow++)
            {
                tempList<<(*itOneRow);
                logList.append(*itOneRow).append(" ");
            }
            //qDebug()<<"Row:"<<tempList;
            log = QString("[Row] ")+logList;
            logList.clear();
            SetProcess(log,m_ProcessValue);
        }
        m_ProcessValue++;
    }
    log = QString("[Finish] Excel Loading");
    SetProcess(log,100);
}
bool MainWindow::GenerateXML_EventMsg()
{
    m_ProcessValue = 0;
    QString Path = QCoreApplication::applicationDirPath()+QString(QStringLiteral("/EventProtocol.xml"));
    QString OriginPath = QCoreApplication::applicationDirPath()+QString(QStringLiteral("/EventOrigin"));

    //qDebug()<<"Path:"<<Path;
    QString log = QString("[OK] Parameter Xml:")+Path;
    SetProcess(log,m_ProcessValue++);
    QFile file(Path);
    if (!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        //qDebug()<<"File Open Failed!";
        log = QString("[Error] Open Xml");
        SetProcess(log,m_ProcessValue++);
        return false;
    }
    log = QString("[OK] Open Xml");
    SetProcess(log,m_ProcessValue++);

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
        SensorElement.setAttribute("Description",tempSheetInfo.m_SheetDescriptionName);
        SensorElement.setAttribute("Topics",tempSheetInfo.m_SheetTopics);
        rootElement.appendChild(SensorElement);
        QStringList propertyList;
        for(int i=0;i <tempSheetInfo.m_vProperty.size();i++)
        {
            propertyList<<tempSheetInfo.m_vProperty.at(i);

        }

        for(int index = 0;index < tempSheetInfo.m_vvRow.size();index++)
        {
            QVector<QString> vRow = tempSheetInfo.m_vvRow.at(index);//每一行的值列表
            QDomElement item = document.createElement( "Row" );
            QDomElement scriptItem = document.createElement( "Script" );
            for(int i=0;i<vRow.size() && i<propertyList.size();i++)
            {
                item.setAttribute( propertyList.at(i), vRow[i] );
            }
            item.appendChild(scriptItem);
            SensorElement.appendChild( item );
        }
    }

    QTextStream out( &file );
    document.save( out, 4 );
    file.close();
    //qDebug()<<"--FINISH--";
    QFile::remove(OriginPath);
    if(QFile::copy(Path,OriginPath))
    {
        log = QString("[OK] Generate Oring File Success!");
        SetProcess(log,m_ProcessValue++);
    }
    else
    {
        log = QString("[ERROR] Generate Oring File Failed!");
        SetProcess(log,m_ProcessValue++);
    }
    log = QString("[OK] FINISH Parameter Xml Generation");
    SetProcess(log,100);

    return 1;
}
bool MainWindow::GenerateXML_SensorFloat()
{
    m_ProcessValue = 0;
    QString Path = QCoreApplication::applicationDirPath()+QString(QStringLiteral("/SensorProtocol.xml"));
    QString OriginPath = QCoreApplication::applicationDirPath()+QString(QStringLiteral("/SensorOrigin"));
    //qDebug()<<"Path:"<<Path;
    QString log = QString("[OK] Parameter Xml:")+Path;
    SetProcess(log,m_ProcessValue++);
    QFile file(Path);
    if (!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        //qDebug()<<"File Open Failed!";
        log = QString("[Error] Open Xml");
        SetProcess(log,m_ProcessValue++);
        return false;
    }
    log = QString("[OK] Open Xml");
    SetProcess(log,m_ProcessValue++);

    QDomDocument document;
    QString strHeader( "version=\"1.0\" encoding=\"UTF-8\"" );
    document.appendChild( document.createProcessingInstruction("xml", strHeader) );
    QDomElement	rootElement = document.createElement( "ProtocolList" );
    QDomElement	typeElement = document.createElement( "SrcProtocolList" );
    rootElement.appendChild(typeElement);
    document.appendChild(rootElement);

    QHash<int,SheetInfo>::iterator it = m_hSheetInfor.begin();
    for(it;it != m_hSheetInfor.end();it++)
    {
        SheetInfo tempSheetInfo = it.value();
        QDomElement SensorElement = document.createElement( tempSheetInfo.m_SheetName );
        SensorElement.setAttribute("Description",tempSheetInfo.m_SheetDescriptionName);
        SensorElement.setAttribute("Topics",tempSheetInfo.m_SheetTopics);
        typeElement.appendChild(SensorElement);
        QStringList propertyList;
        for(int i=0;i <tempSheetInfo.m_vProperty.size();i++)
        {
            propertyList<<tempSheetInfo.m_vProperty.at(i);

        }

        for(int index = 0;index < tempSheetInfo.m_vvRow.size();index++)
        {
            QVector<QString> vRow = tempSheetInfo.m_vvRow.at(index);//每一行的值列表
            QDomElement item = document.createElement( "Row" );
            for(int i=0;i<vRow.size() && i<propertyList.size();i++)
            {
                item.setAttribute( propertyList.at(i), vRow[i] );
            }
            SensorElement.appendChild( item );
        }
    }
    m_SensorFloatSheetInfor.swap(m_hSheetInfor);
    slot_OpenExcelFile();
    GenerateXML_SensorInt(document,rootElement,m_ProcessValue);
    QTextStream out( &file );
    document.save( out, 4 );
    file.flush();
    file.close();
    //qDebug()<<"--FINISH--";
    QFile::remove(OriginPath);
    if(QFile::copy(Path,OriginPath))
    {
        log = QString("[OK] Generate Oring File Success!");
    }
    else
    {
        log = QString("[ERROR] Generate Oring File Failed!");
    }
    SetProcess(log,m_ProcessValue++);
    log = QString("[OK] FINISH Parameter Xml Generation");
    SetProcess(log,100);

    return 1;
}
bool MainWindow::GenerateXML_SensorInt(QDomDocument& document,QDomElement& rootElement,int& processValue )
{
    QDomElement	typeElement = document.createElement( "DestProtocolList" );
    rootElement.appendChild(typeElement);
    document.appendChild(rootElement);
    QString log = QString("[OK] Start Dest Parameter Xml Generation");
    SetProcess(log,processValue++);

    QHash<int,SheetInfo>::iterator it = m_hSheetInfor.begin();
    for(it;it != m_hSheetInfor.end();it++)
    {
        SheetInfo tempSheetInfo = it.value();
        //查找传感器数据源
        auto srcIt = m_SensorFloatSheetInfor.begin();
        for(srcIt;srcIt != m_SensorFloatSheetInfor.end();srcIt++)
        {
            SheetInfo srcSheetInfo = srcIt.value();
            if(srcSheetInfo.m_SheetName == tempSheetInfo.m_SheetName)
            {
                //qDebug()<<"SheetName:"<<srcSheetInfo.m_SheetName;
                QDomElement SensorElement = document.createElement( tempSheetInfo.m_SheetName );
                SensorElement.setAttribute("Description",tempSheetInfo.m_SheetDescriptionName);
                typeElement.appendChild(SensorElement);
                QStringList propertyList;
                for(int i=0;i <tempSheetInfo.m_vProperty.size();i++)
                {
                    propertyList<<tempSheetInfo.m_vProperty.at(i);

                }
                QStringList srcPropertyList;
                for(int i=0;i <srcSheetInfo.m_vProperty.size();i++)
                {
                    srcPropertyList<<srcSheetInfo.m_vProperty.at(i);

                }
        #if 1
                for(int index = 0;index < tempSheetInfo.m_vvRow.size();index++)
                {
                    QVector<QString> vRow = tempSheetInfo.m_vvRow.at(index);//每一行的值列表
                    QDomElement item = document.createElement( "Row" );
                    for(int i=0;i<vRow.size() && i<propertyList.size();i++)
                    {
                        if(propertyList.at(i) == QStringLiteral("Index"))
                        {
                            item.setAttribute( propertyList.at(i), vRow[i] );
                        }
                        else if(propertyList.at(i) == QStringLiteral("Name"))
                        {
                            item.setAttribute( propertyList.at(i), vRow[i] );
                            bool isFind = false;
                            for(int j = 0;j < srcSheetInfo.m_vvRow.size();j++)
                            {
                                QVector<QString> srcRow = srcSheetInfo.m_vvRow.at(j);//每一行的值列表
                                for(int k=0;k<srcRow.size() && k<srcPropertyList.size();k++)
                                {
                                    if(srcPropertyList.at(k) == QStringLiteral("Name"))
                                    {
                                        if(srcRow.at(k).compare(vRow.at(i)) ==0)//找到对应名称
                                        {
                                            isFind = true;
                                            for(int l=0;l<srcRow.size() && l<srcPropertyList.size();l++)
                                            {
                                                if(srcPropertyList.at(l) == QStringLiteral("Index"))
                                                {
                                                    item.setAttribute( QStringLiteral("SrcIndex"), srcRow[l] );
                                                }
                                                else if(srcPropertyList.at(l) == QStringLiteral("Type"))
                                                {
                                                    if(srcRow[l].compare(QStringLiteral("double"))==0 || srcRow[l].compare(QStringLiteral("float"))==0)
                                                    {
                                                        item.setAttribute(QStringLiteral("Type"), QStringLiteral("int") );
                                                    }
                                                    else
                                                    {
                                                        item.setAttribute(QStringLiteral("Type"), srcRow[l] );
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }//end for
                            if(!isFind)
                            {
                                for(int i=0;i<vRow.size() && i<propertyList.size();i++)
                                {
                                    if(propertyList.at(i) == QStringLiteral("SrcIndex") || propertyList.at(i) == QStringLiteral("Type"))
                                    {
                                        item.setAttribute( propertyList.at(i), vRow[i] );
                                    }
                                }
                            }

                        }
                    }
                    SensorElement.appendChild( item );
                }
        #else
                QDomElement item = document.createElement( "Row" );
                item.setAttribute("Index","1");
                item.setAttribute("SrcIndex","NULL");
                item.setAttribute("Name","NULL");
                item.setAttribute("Type","int");
                SensorElement.appendChild( item );
        #endif
            }
        }

    }

    log = QString("[OK] FINISH Dest Parameter Xml Generation");
    SetProcess(log,processValue++);
    return true;
}
bool MainWindow::GenerateXML_IntegratedOutput()
{
    m_ProcessValue = 0;
    QString Path = QCoreApplication::applicationDirPath()+QString(QStringLiteral("/IntegratedProtocol.xml"));
    QString OriginPath = QCoreApplication::applicationDirPath()+QString(QStringLiteral("/IntegratedOrigin"));

    //qDebug()<<"Path:"<<Path;
    QString log = QString("[OK] Parameter Xml:")+Path;
    SetProcess(log,m_ProcessValue++);
    QFile file(Path);
    if (!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        //qDebug()<<"File Open Failed!";
        log = QString("[Error] Open Xml");
        SetProcess(log,m_ProcessValue++);
        return false;
    }
    log = QString("[OK] Open Xml");
    SetProcess(log,m_ProcessValue++);

    QDomDocument document;
    QString strHeader( "version=\"1.0\" encoding=\"UTF-8\"" );
    document.appendChild( document.createProcessingInstruction("xml", strHeader) );
    QDomElement	rootElement = document.createElement( "ProtocolList" );
    QDomElement	typeElement = document.createElement( "SrcProtocolList" );
    rootElement.appendChild(typeElement);
    document.appendChild(rootElement);

    QHash<int,SheetInfo>::iterator it = m_hSheetInfor.begin();
    for(it;it != m_hSheetInfor.end();it++)
    {
        SheetInfo tempSheetInfo = it.value();
        QDomElement SensorElement = document.createElement( tempSheetInfo.m_SheetName );
        SensorElement.setAttribute("Description",tempSheetInfo.m_SheetDescriptionName);
        SensorElement.setAttribute("Topics",tempSheetInfo.m_SheetTopics);
        typeElement.appendChild(SensorElement);
        QStringList propertyList;
        for(int i=0;i <tempSheetInfo.m_vProperty.size();i++)
        {
            propertyList<<tempSheetInfo.m_vProperty.at(i);

        }

        for(int index = 0;index < tempSheetInfo.m_vvRow.size();index++)
        {
            QVector<QString> vRow = tempSheetInfo.m_vvRow.at(index);//每一行的值列表
            QDomElement item = document.createElement( "Row" );
            for(int i=0;i<vRow.size() && i<propertyList.size();i++)
            {
                item.setAttribute( propertyList.at(i), vRow[i] );
            }
            SensorElement.appendChild( item );
        }
    }
    typeElement = document.createElement( "DestProtocolList" );
    rootElement.appendChild(typeElement);
    document.appendChild(rootElement);
    it = m_hSheetInfor.begin();
    for(it;it != m_hSheetInfor.end();it++)
    {
        SheetInfo tempSheetInfo = it.value();
        QDomElement SensorElement = document.createElement( tempSheetInfo.m_SheetName );
        SensorElement.setAttribute("Description",tempSheetInfo.m_SheetDescriptionName);
        typeElement.appendChild(SensorElement);
        QStringList propertyList;
        for(int i=0;i <tempSheetInfo.m_vProperty.size();i++)
        {
            propertyList<<tempSheetInfo.m_vProperty.at(i);

        }

        for(int index = 0;index < tempSheetInfo.m_vvRow.size();index++)
        {
            QVector<QString> vRow = tempSheetInfo.m_vvRow.at(index);//每一行的值列表
            QDomElement item = document.createElement( "Row" );
            for(int i=0;i<vRow.size() && i<propertyList.size();i++)
            {
                if(QStringLiteral("ConversionFactors") == propertyList.at(i) || QStringLiteral("LSB") == propertyList.at(i) )
                {
                    continue;
                }
                item.setAttribute( propertyList.at(i), vRow[i] );
            }
            SensorElement.appendChild( item );
        }
    }

    QTextStream out( &file );
    document.save( out, 4 );
    file.close();
    //qDebug()<<"--FINISH--";
    QFile::remove(OriginPath);
    if(QFile::copy(Path,OriginPath))
    {
        log = QString("[OK] Generate Oring File Success!");
        SetProcess(log,m_ProcessValue++);
    }
    else
    {
        log = QString("[ERROR] Generate Oring File Failed!");
        SetProcess(log,m_ProcessValue++);
    }
    log = QString("[OK] FINISH Parameter Xml Generation");
    SetProcess(log,100);

    return true;
}
void MainWindow::GenerateStructFile_TypeName()
{

    m_ProcessValue = 0;
    QHash<int,SheetInfo>::iterator it = m_hSheetInfor.begin();
    for(it;it != m_hSheetInfor.end();it++)
    {
        SheetInfo tempSheetInfo = it.value();

        QString FileName = tempSheetInfo.m_SheetName + QStringLiteral(".txt");
        QString Path = QCoreApplication::applicationDirPath()+ QStringLiteral("/structs/") + FileName;
        //qDebug()<<"Path:"<<Path;
        QString log = QString("[OK] Struct txt:")+Path;
        SetProcess(log,m_ProcessValue++);
        QFile file(Path);
        if (!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
        {
            //qDebug()<<QString("File:%1 Create Failed!").arg(FileName);
            log = QString("[Error] Open Struct txt");
            SetProcess(log,m_ProcessValue++);
            continue;
        }
        int typeIndex = 0;
        int nameIndex = 0;
        for(int i=0;i <tempSheetInfo.m_vProperty.size();i++)
        {
            if(tempSheetInfo.m_vProperty.at(i) == QStringLiteral("Type") )
            {
                typeIndex = i;
            }
            else if(tempSheetInfo.m_vProperty.at(i) == QStringLiteral("Name") )
            {
                nameIndex = i;
            }
        }


        log = QString("[OK] Open Struct txt");
        SetProcess(log,m_ProcessValue++);
        QByteArray head = QString("typedef struct _%1//%2\n{\n").arg(tempSheetInfo.m_SheetName).arg(tempSheetInfo.m_SheetDescriptionName).toLocal8Bit();
        file.write(head);
        QByteArray paraList;
        QString Name = QStringLiteral("para_");
        for(int index = 0;index < tempSheetInfo.m_vvRow.size();index++)
        {

            QVector<QString> vRow = tempSheetInfo.m_vvRow.at(index);
            paraList += QString("   %1 %2;//%3\n").arg(vRow.at(typeIndex)).arg(Name+QString::number(index)).arg(vRow.at(nameIndex)).toLocal8Bit();
        }
        file.write(paraList);
        QByteArray last = QString("}%1,*%2;").arg(tempSheetInfo.m_SheetName).arg(tempSheetInfo.m_SheetName).toLocal8Bit();
        file.write(last);
        file.close();
    }
    //qDebug()<<"--FINISH--";
    QString log = QString("[OK] FINISH Struct txt Generation");
    SetProcess(log,100);
    return;
}
bool MainWindow::GenerateStructXML(QString TopicHead)
{
    m_ProcessValue = 0;
    QHash<int,SheetInfo>::iterator it = m_hSheetInfor.begin();
    for(it;it != m_hSheetInfor.end();it++)
    {
        SheetInfo tempSheetInfo = it.value();

        QString FileName = tempSheetInfo.m_SheetName + QStringLiteral(".xml");
        QString Path = QCoreApplication::applicationDirPath()+ QStringLiteral("/configs/") + FileName;
        //qDebug()<<"Path:"<<Path;
        QString log = QString("[OK] Struct Xml:")+Path;
        SetProcess(log,m_ProcessValue++);
        QFile file(Path);
        if (!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
        {
            //qDebug()<<QString("File:%1 Create Failed!").arg(FileName);
            log = QString("[Error] Open Struct Xml");
            SetProcess(log,m_ProcessValue++);
            continue;
        }
        log = QString("[OK] Open Struct Xml");
        SetProcess(log,m_ProcessValue++);

        QDomDocument document;
        QString strHeader( "version=\"1.0\" encoding=\"UTF-8\"" );
        document.appendChild( document.createProcessingInstruction("xml", strHeader) );

        QDomElement RootElement = document.createElement(QStringLiteral("topicname"));
        QString topicName = TopicHead + tempSheetInfo.m_SheetName;
        RootElement.setAttribute("name",topicName);
        document.appendChild(RootElement);

        //根据excel中类型顺序 Index	Name	Type	ValueDescription	Form
        //xml需要构建的类型顺序 order type name pos len varname
        int pos = 0;
        for(int index = 0;index < tempSheetInfo.m_vvRow.size();index++)
        {
            QDomElement VarElement = document.createElement(QStringLiteral("var"));
            VarElement.setAttribute("id",QString::number(index));
            RootElement.appendChild(VarElement);

            QVector<QString> vRow = tempSheetInfo.m_vvRow.at(index);

            QDomElement ParaElement = document.createElement(QStringLiteral("order"));
            QDomNode nodeValue = document.createTextNode(QString::number(index));
            ParaElement.appendChild(nodeValue);
            VarElement.appendChild(ParaElement);

            QString valueType;
            int valueLength = 0;
            if(vRow.at(2) == QStringLiteral("长整型"))
            {
                valueLength = 8;
                valueType = QStringLiteral("double");
            }
            else if(vRow.at(2) == QStringLiteral("整型"))
            {
                valueLength = 4;
                valueType = QStringLiteral("int");
            }
            else if(vRow.at(2) == QStringLiteral("枚举型"))
            {
                valueLength = 1;
                valueType = QStringLiteral("char");
            }
            else if(vRow.at(2) == QStringLiteral("布尔型"))
            {
                valueLength = 1;
                valueType = QStringLiteral("bool");
            }
            else if(vRow.at(2) == QStringLiteral("双浮点型"))
            {
                valueLength = 8;
                valueType = QStringLiteral("double");
            }
            else if(vRow.at(2) == QStringLiteral("浮点型"))
            {
                valueLength = 4;
                valueType = QStringLiteral("float");
            }

            ParaElement = document.createElement(QStringLiteral("type"));
            nodeValue = document.createTextNode(valueType);
            ParaElement.appendChild(nodeValue);
            VarElement.appendChild(ParaElement);

            ParaElement = document.createElement(QStringLiteral("name"));
            nodeValue = document.createTextNode(vRow.at(1));
            ParaElement.appendChild(nodeValue);
            VarElement.appendChild(ParaElement);

            ParaElement = document.createElement(QStringLiteral("pos"));
            nodeValue = document.createTextNode(QString::number(pos));
            ParaElement.appendChild(nodeValue);
            VarElement.appendChild(ParaElement);

            ParaElement = document.createElement(QStringLiteral("len"));
            nodeValue = document.createTextNode(QString::number(valueLength));
            ParaElement.appendChild(nodeValue);
            VarElement.appendChild(ParaElement);

            ParaElement = document.createElement(QStringLiteral("varname"));
            //nodeValue = document.createTextNode(QStringLiteral("para_") + QString::number(index));
            nodeValue = document.createTextNode(vRow.at(1));
            ParaElement.appendChild(nodeValue);
            VarElement.appendChild(ParaElement);

            pos += valueLength;
        }
        QTextStream out( &file );
        document.save( out, 4 );
        file.close();
    }
    //qDebug()<<"--FINISH--";
    QString log = QString("[OK] FINISH Struct Xml Generation");
    SetProcess(log,100);
    return 1;

}
void MainWindow::GenerateParaName()
{
    m_ProcessValue = 0;
    QHash<int,SheetInfo>::iterator it = m_hSheetInfor.begin();
    for(it;it != m_hSheetInfor.end();it++)
    {
        SheetInfo tempSheetInfo = it.value();

        QString FileName = tempSheetInfo.m_SheetName + QStringLiteral(".txt");
        QString Path = QCoreApplication::applicationDirPath()+ QStringLiteral("/names/") + FileName;
        //qDebug()<<"Path:"<<Path;
        QString log = QString("[OK] Para txt:")+Path;
        SetProcess(log,m_ProcessValue++);
        QFile file(Path);
        if (!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
        {
            //qDebug()<<QString("File:%1 Create Failed!").arg(FileName);
            log = QString("[Error] Open Para txt");
            SetProcess(log,m_ProcessValue++);
            continue;
        }
        log = QString("[OK] Open Para txt");
        SetProcess(log,m_ProcessValue++);

        QByteArray paraList;
        for(int index = 0;index < tempSheetInfo.m_vvRow.size();index++)
        {
            QVector<QString> vRow = tempSheetInfo.m_vvRow.at(index);
            paraList += QString("<<QStringLiteral(\"%1\")").arg(vRow.at(1)).toLocal8Bit();
        }
        file.write(paraList);
        file.close();
    }
    //qDebug()<<"--FINISH--";
    QString log = QString("[OK] FINISH Para txt Generation");
    SetProcess(log,100);
    return;
}
void MainWindow::GenerateStructFile()
{

    m_ProcessValue = 0;
    QHash<int,SheetInfo>::iterator it = m_hSheetInfor.begin();
    for(it;it != m_hSheetInfor.end();it++)
    {
        SheetInfo tempSheetInfo = it.value();

        QString FileName = tempSheetInfo.m_SheetName + QStringLiteral(".txt");
        QString Path = QCoreApplication::applicationDirPath()+ QStringLiteral("/structs/") + FileName;
        //qDebug()<<"Path:"<<Path;
        QString log = QString("[OK] Struct txt:")+Path;
        SetProcess(log,m_ProcessValue++);
        QFile file(Path);
        if (!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
        {
            //qDebug()<<QString("File:%1 Create Failed!").arg(FileName);
            log = QString("[Error] Open Struct txt");
            SetProcess(log,m_ProcessValue++);
            continue;
        }
        log = QString("[OK] Open Struct txt");
        SetProcess(log,m_ProcessValue++);
        QByteArray head = QString("typedef struct _%1//%2\n{\n").arg(tempSheetInfo.m_SheetName).arg(tempSheetInfo.m_SheetDescriptionName).toLocal8Bit();
        file.write(head);
        QByteArray paraList;
        for(int index = 0;index < tempSheetInfo.m_vvRow.size();index++)
        {
            QVector<QString> vRow = tempSheetInfo.m_vvRow.at(index);
            paraList += QString("   %1 %2;//%3\n").arg(vRow.at(2)).arg(vRow.at(1)).arg(vRow.at(3)).toLocal8Bit();
        }
        file.write(paraList);
        QByteArray last = QString("}%1,*%2;").arg(tempSheetInfo.m_SheetName).arg(tempSheetInfo.m_SheetName).toLocal8Bit();
        file.write(last);
        file.close();
    }
    //qDebug()<<"--FINISH--";
    QString log = QString("[OK] FINISH Struct txt Generation");
    SetProcess(log,100);
    return;
}
