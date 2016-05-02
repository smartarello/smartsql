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
#include "ForeignKeyModel.h"

ForeignKeyModel::ForeignKeyModel(Util::TableDefinition table, QObject *parent) : QAbstractTableModel(parent)
{
    // Column headers
    this->headers << tr("Key name");
    this->headers << tr("Columns");
    this->headers << tr("Reference table");
    this->headers << tr("Foreign columns");
    this->headers << tr("On UPDATE");
    this->headers << tr("On DELETE");

    this->fks = table.foreignKeys();
}

int ForeignKeyModel::rowCount(const QModelIndex & parent) const
{
    return this->fks.size() ;
}

int ForeignKeyModel::columnCount(const QModelIndex & parent) const
{
    return this->headers.size() ;
}

QVariant ForeignKeyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < this->headers.size() && role == Qt::DisplayRole && orientation == Qt::Horizontal) {

        return QVariant(this->headers.at(section));

    }

    return QVariant();
}


QVariant ForeignKeyModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < this->fks.size()) {
        ForeignKeyDefinition fk = this->fks.at(index.row());

        if (role == Qt::DisplayRole) {
            switch (index.column()) {

            case 0:
                return QVariant(fk.keyName);
            case 1:
                return QVariant(fk.column);
            case 2:
                return QVariant(fk.foreignTable);
                break;
            case 3:
                return QVariant(fk.foreignColumn);

            case 4:
                return QVariant(fk.onUpdate);

            case 5:
                return QVariant(fk.onDelete);

            }
        }

    }

    return QVariant();
}
