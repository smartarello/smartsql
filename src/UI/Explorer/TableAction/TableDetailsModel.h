#ifndef TABLEDETAILSMODEL_H
#define TABLEDETAILSMODEL_H

#include <QStandardItemModel>
#include <QSqlDatabase>
class TableDetailsModel : public QAbstractTableModel
{

    Q_OBJECT

public:
    TableDetailsModel(QString createString, QObject *parent = 0);
    int rowCount(const QModelIndex & parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex & parent) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:

    struct ColumnDefinition
    {
        QString name;
        QString type;
        int length;
        bool unsignedCol;
        bool allowNull;
        QVariant defaultValue;
    };

    QList<ColumnDefinition> columns;
    QStringList headers;
    QStringList primaryKey;
};

#endif // TABLEDETAILSMODEL_H
