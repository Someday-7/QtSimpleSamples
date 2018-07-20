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
private slots:
    void ShowBasisTablesSlot(QTreeWidgetItem*, int);
private:
    Ui::MainWindow *ui;
    QTreeWidget* m_TreeWidget;
};

#endif // MAINWINDOW_H
