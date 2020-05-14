#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QHBoxLayout>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getItemCkeckedList(QVector<QStringList>&);
    void addTopLevelItem(QString strTopItem);
    void addChildItem(QString,QStringList);
private slots:
    void slotItemDoubleClicked(QTreeWidgetItem*, int);
private:
    Ui::MainWindow *ui;
    QTreeWidget* m_TreeWidget;
};

#endif // MAINWINDOW_H
