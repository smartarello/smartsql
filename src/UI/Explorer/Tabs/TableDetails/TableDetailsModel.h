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
    QStringList fks;
    QStringList headers;
    QStringList primaryKey;
};

#endif // TABLEDETAILSMODEL_H
