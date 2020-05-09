#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    if(SetupSqliteConnect())
    {
//        int i = 0;
//        while(i<100)
//        {
//            InsertIntoSqlite(i++);
//        }
        seclectSql();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//创建数据库
bool MainWindow::SetupSqliteConnect()
{
    QString DatabaseDirName = QApplication::applicationDirPath()+QString("/Operation.db");
    SqliteDatabase = QSqlDatabase::addDatabase("QSQLITE","Local");
    SqliteDatabase.setDatabaseName(DatabaseDirName);
//    SqliteDatabase.setUserName("admin");
//    QByteArray PasswordDb("admin12345#");
//    QString PasswordMod5 = QCryptographicHash::hash(PasswordDb,QCryptographicHash::Md5);
//    SqliteDatabase.setPassword(PasswordMod5);
    if( !SqliteDatabase.open() )
    {
        MsgBox.about(NULL,"错误","连接本地数据库错误！");
        return false;
    }
    QSqlQuery SqlQueryObj(SqliteDatabase);
    SqlQueryObj.prepare( "CREATE TABLE IF NOT EXISTS OperationLog (CHECKABLE VARCHAR(1), Result VARCHAR(10), Reason VARCHAR(100))" );
    if( !SqlQueryObj.exec() )
    {
        MsgBox.about(NULL,"错误","本地数据库表创建错误！");
        return false;
    }
    modelDelegate = new TableViewDelegate(this,SqliteDatabase);
    modelDelegate->setColumCheckable(0);
    return true;
}
//数据插入
bool MainWindow::InsertIntoSqlite(int index)
{
    if(SqliteDatabase.isOpen())
    {
        QSqlQuery SqlQueryObj(SqliteDatabase);
        QString AddQuerryString = QObject::tr("INSERT INTO OperationLog (CHECKABLE, Result, Reason) VALUES ('%1', '%2', '%3')").arg("1").arg(QString::number(index)).arg("321");
        SqlQueryObj.prepare(AddQuerryString);
        if(!SqlQueryObj.exec())
        {
            MsgBox.about(NULL,"错误","数据库写入错误！");
        }
        return true;
    }
    return false;
}

bool MainWindow::seclectSql()
{
#if 0
    QSqlQuery SqlQueryObj(SqliteDatabase);
    QString AddQuerryString = QObject::tr("SELECT * FROM OperationLog");
    SqlQueryObj.prepare(AddQuerryString);
    if(SqlQueryObj.exec())
    {
        return true;
    }
    return false;
#endif
    modelDelegate->setTable("OperationLog");
    modelDelegate->select();
    modelDelegate->setHeaderData(0, Qt::Horizontal, QObject::tr("1")); //字段命名
    modelDelegate->setHeaderData(1, Qt::Horizontal, QObject::tr("2"));//字段命名
    modelDelegate->setHeaderData(2, Qt::Horizontal, QObject::tr("3"));//字段命名
    ui->tableView->setModel(modelDelegate);
    return true;
}


