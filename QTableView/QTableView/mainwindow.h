#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include "TableViewDelegate.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool SetupSqliteConnect();
    bool InsertIntoSqlite(int);
    bool seclectSql();
private:
    Ui::MainWindow *ui;
    QSqlDatabase SqliteDatabase;
    QMessageBox MsgBox;
    TableViewDelegate* modelDelegate;
};
#endif // MAINWINDOW_H
