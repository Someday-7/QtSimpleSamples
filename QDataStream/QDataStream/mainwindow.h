#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
