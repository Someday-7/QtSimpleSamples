#include "TableViewDelegate.h"
#include <QSqlRecord>
#include <QSqlField>
TableViewDelegate::TableViewDelegate(QObject *parent,QSqlDatabase db)
    :QSqlTableModel(parent,db)
{
    m_ColumnCheckable = 0;
}

void TableViewDelegate::setColumCheckable(int i)
{
    m_ColumnCheckable = i;
}

Qt::ItemFlags TableViewDelegate::flags(const QModelIndex &index) const
{
    if (!index.isValid())
          return QAbstractItemModel::flags(index);
      int col = index.column();
      if(col == m_ColumnCheckable)
      {
          return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
      }
      return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TableViewDelegate::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    int row = index.row();
    int colum = index.column();

    if (row < 0)
    {
        return QVariant();
    }
    switch (role)
    {

    case Qt::DecorationRole:
    {
        break;
    }
    case  Qt::TextColorRole:
    {
        break;
    }

    case Qt::ToolTipRole:
    {
        break;
    }
    case Qt::DisplayRole:
    {
        QSqlRecord getRecord = record();
        return getRecord.value(colum);
        break;
    }
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignVCenter);
    //关键--------------------------------------------------------------
    case Qt::CheckStateRole:
    {
        if (colum == m_ColumnCheckable)//在第n列显示QCheckBox
        {
            return Qt::Checked;
        }
        break;
    }
    //--------------------------------------------------------------
    default:
        return QVariant();
        break;
    }
    return QVariant();
}

bool TableViewDelegate::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (role == Qt::CheckStateRole && index.column() == m_ColumnCheckable)
    {
        if (value == Qt::Checked) //
        {
            m_rowCheckStateMap[index.row()] = Qt::Checked;
        }
        else
        {
            m_rowCheckStateMap[index.row()] = Qt::Unchecked;
        }
    }
    return true;
}
