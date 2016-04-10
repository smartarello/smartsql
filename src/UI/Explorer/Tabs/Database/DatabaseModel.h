/**
 * Copyright (C) 2016  Stéphane Martarello
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
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
