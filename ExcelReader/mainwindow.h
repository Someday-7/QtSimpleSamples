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
    QString m_SheetDescriptionName;
    QString m_SheetTopics;
    QVector<QString> m_vProperty;//每一列的名称
    QVector< QVector<QString> > m_vvRow;//每一行的值
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
    void InitStyleSheet();
    void ExcelReader(QString FilePath);
    bool GenerateXML_EventMsg();//生成参数列表xml 每一个参数的每个属性为不同节点
    bool GenerateXML_SensorFloat();//root节点具有topic属性
    bool GenerateXML_SensorInt(QDomDocument& document,QDomElement& rootElement,int& processValue);//对应生成相同数据类型的转化后目标整型数据(综导)
    bool GenerateXML_IntegratedOutput();
    void GenerateStructFile_TypeName();
    bool GenerateStructXML(QString TopicHead);//生成结构体xml文件（对应数据采集软件解析版本）
    void SetProcess(QString,int);
    void GenerateParaName();
    void GenerateStructFile();

protected:

private slots:
    void slot_OpenExcelFile();
    void slot_ChooseFileProgram(QString CurrentText);
private:
    Ui::MainWindow *ui;
    QHash<int,SheetInfo> m_hSheetInfor;
    QHash<int,SheetInfo> m_SensorFloatSheetInfor;
    int m_ProcessValue;
};

#endif // MAINWINDOW_H
