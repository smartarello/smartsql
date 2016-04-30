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
#include "TableIndexModel.h"

TableIndexModel::TableIndexModel(QString createString, QObject*parent) : QAbstractTableModel(parent)
{
    // Column headers
    this->headers << tr("Key name");
    this->headers << tr("Columns");
    this->headers << tr("Type");

    QStringList createParts = createString.split("\n");
    createParts.removeAt(0); // Removes the first line with CREATE TABLE
    foreach(QString part, createParts) {
        QRegExp keyRx("^\\s*KEY `(.+)` \\((.+)\\)", Qt::CaseInsensitive);
        if (keyRx.indexIn(part) != -1) {

            IndexDefinition index;
            index.keyName = keyRx.cap(1);
            index.columns = keyRx.cap(2).replace("`", "").split(",");
            index.type = "KEY";

            this->indexes << index;
        }

        QRegExp uniquekeyRx("^\\s*UNIQUE KEY `(.+)` \\((.+)\\)", Qt::CaseInsensitive);
        if (uniquekeyRx.indexIn(part) != -1) {

            IndexDefinition index;
            index.keyName = uniquekeyRx.cap(1);
            index.columns = keyRx.cap(2).replace("`", "").split(",");
            index.type = "UNIQUE";

            this->indexes << index;
        }

        QRegExp primaryKeyRx("^\\s*PRIMARY KEY \\((.*)\\)", Qt::CaseInsensitive);
        if (primaryKeyRx.indexIn(part) != -1) { // Primary key definition
            IndexDefinition index;
            index.keyName = "PRIMARY KEY";
            index.columns = primaryKeyRx.cap(1).replace("`", "").split(",");
            index.type = "PRIMARY KEY";

            this->indexes << index;
        }
    }
}


int TableIndexModel::rowCount(const QModelIndex & parent) const
{
    return this->indexes.size() ;
}

int TableIndexModel::columnCount(const QModelIndex & parent) const
{
    return this->headers.size() ;
}

QVariant TableIndexModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < this->headers.size() && role == Qt::DisplayRole && orientation == Qt::Horizontal) {

        return QVariant(this->headers.at(section));

    }

    return QVariant();
}


QVariant TableIndexModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < this->indexes.size()) {
        IndexDefinition fk = this->indexes.at(index.row());

        if (role == Qt::DisplayRole) {
            switch (index.column()) {

            case 0:
                return QVariant(fk.keyName);
            case 1:
                return QVariant(fk.columns.join(", "));
            case 2:
                return QVariant(fk.type);
            }
        }

    }

    return QVariant();
}
