#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
#define STREAM_TYPE_STM_DAT_MODE 0 //1:STM 0:DAT
/*
 *.stm : qt预定义编码格式文件，按照Qt预定义数据格式写入，预定义的标记符读出数据
 *.dat ：二进制原始内容写入和读出内容，每个字节都具有具体定义
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initTableData();
private slots:
    void slot_BtnClicked();
private:
    Ui::MainWindow *ui;
    QStandardItemModel* m_StandardItemModel;

};
#endif // MAINWINDOW_H
