#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QAxObject>
#include <QHash>
#include <QtXml/QDomDocument>
#include <QTextStream>
typedef struct _SheetInfo
{
    QString m_SheetName;
    QVector<QString> m_vProperty;
    QVector< QVector<QString> > m_vvRow;
    _SheetInfo()
    {
        m_SheetName = QStringLiteral("");
    }
}SheetInfo;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ExcelReader();
    bool GenerateXML();
private:
    Ui::MainWindow *ui;
    QHash<int,SheetInfo> m_hSheetInfor;
};

#endif // MAINWINDOW_H
