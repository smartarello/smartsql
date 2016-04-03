#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QModelIndex>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Database {
class DatabaseModel : public QAbstractTableModel
{

    Q_OBJECT
public:
    DatabaseModel(QObject * parent = 0);
    QVariant data(const QModelIndex & item, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex & parent) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    void reload();

private:

    struct TableDescription {
        QString name;
        QString engine;
        QString rowFormat;
        int rowCount;
        double rawSize;
        QString collation;
        QString size;

    };


    QString getSizeString(double size) ;
    int sortCol;
    Qt::SortOrder order;
    bool tableLessThan(TableDescription table1, TableDescription table2);



    QList<TableDescription> tableList;
};
} /* namespace Database */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */


#endif // DATABASEMODEL_H
