#ifndef TABLEVIEWDELEGATE_H
#define TABLEVIEWDELEGATE_H
#include <QtSql/QSqlTableModel>
enum CheckableRole{COLUMN_CHECKABLE = 0};
class TableViewDelegate : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit TableViewDelegate(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());
    void setColumCheckable(int);
public:

    Qt::ItemFlags flags(const QModelIndex &index) const override;
protected:
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
private:
    int m_ColumnCheckable;
    QMap<int, Qt::CheckState> m_rowCheckStateMap;
};

#endif // TABLEVIEWDELEGATE_H
