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
#include "TableDetailsModel.h"
#include <QDebug>
#include <QPixmap>

TableDetailsModel::TableDetailsModel(Util::TableDefinition table, QObject *parent) :
    QAbstractTableModel(parent),
    table(table)
{
    // Column headers
    this->headers << tr("Name");
    this->headers << tr("Data type");
    this->headers << tr("Length/Set");
    this->headers << tr("Unsigned");
    this->headers << tr("Allow NULL");
    this->headers << tr("Default");
}


int TableDetailsModel::rowCount(const QModelIndex & parent) const
{
    return this->table.columns().size() ;
}

int TableDetailsModel::columnCount(const QModelIndex & parent) const
{
    return this->headers.size() ;
}

QVariant TableDetailsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < this->headers.size() && role == Qt::DisplayRole && orientation == Qt::Horizontal) {

        return QVariant(this->headers.at(section));

    } else if (orientation == Qt::Vertical && section < this->table.columns().size() && role == Qt::DecorationRole) {
        ColumnDefinition col = this->table.columns().at(section);
        if (this->table.primaryKey().contains(col.name, Qt::CaseInsensitive)) {
            return QPixmap(":/resources/icons/key-icon.png").scaledToWidth(20);
        }
    }

    return QVariant();
}

QVariant TableDetailsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < this->table.columns().size()) {
        ColumnDefinition col = this->table.columns().at(index.row());

        if (role == Qt::DisplayRole) {
            switch (index.column()) {

            case 0:
                return QVariant(col.name);
            case 1:
                return QVariant(col.type.toUpper());
            case 2:
                if (col.length != -1) {
                    return col.length;
                }
                break;
            case 3:
                if (col.unsignedCol) {
                    return tr("Yes");
                }
                 break;

            case 4:
                if (col.allowNull) {
                    return tr("Yes");
                }
                 break;

            case 5:
                if (col.defaultValue.isNull()) {
                    return "NULL";
                } else {
                    return col.defaultValue;
                }

            }
        } else if (role == Qt::FontRole && index.column() == 5 && col.defaultValue.isNull()) {
            QFont font("DejaVu Sans Mono");
            font.setItalic(true);
            return QVariant(font);
        }

    }

    return QVariant();
}

Qt::ItemFlags TableDetailsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
