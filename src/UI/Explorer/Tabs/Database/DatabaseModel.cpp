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
#include "DatabaseModel.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFont>


namespace UI {
namespace Explorer {
namespace Tabs {
namespace Database {

DatabaseModel::DatabaseModel(QObject * parent) : QAbstractTableModel(parent)
{

}

int DatabaseModel::rowCount(const QModelIndex & parent) const
{
    return this->tableList.size();
}

int DatabaseModel::columnCount(const QModelIndex & parent) const
{
    return 6;
}

QVariant DatabaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch(section) {
        case 0: return "Name";
        case 1: return "Size";
        case 2: return "Rows";
        case 3: return "Engine";
        case 4: return "Collation";
        case 5: return "Row format";
        }

    } else if (role == Qt::FontRole) {
        QFont font;
        font.setBold(false);
        return font;
    }

    return QVariant();
}

QVariant DatabaseModel::data(const QModelIndex &index, int role) const
{
    // Role used to sort column
    int sortRole = Qt::UserRole+1;

    if ((role == Qt::DisplayRole || (role == sortRole && index.column() != 1)) && index.row() < this->tableList.size()) {
        TableDescription table = this->tableList.at(index.row());
        switch(index.column()) {
        case 0: return table.name;
        case 1: return table.size;
        case 2: return table.rowCount;
        case 3: return table.engine;
        case 4: return table.collation;
        case 5: return table.rowFormat;
        }
    } else if (role == sortRole && index.column() == 1) {
        TableDescription table = this->tableList.at(index.row());
        return table.rawSize;
    }

    return QVariant();
}


void DatabaseModel::reload()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.open()) {
        qWarning() << "DatabaseModel::reload - Database not open";
        return;
    }

    this->tableList = QList<TableDescription>();

    QSqlQuery query;
    if (query.exec("SHOW TABLE STATUS")) {
        while (query.next()) {
            TableDescription table;
            table.name = query.value("Name").toString();
            table.collation = query.value("Collation").toString();
            table.engine = query.value("Engine").toString();
            table.rowFormat = query.value("Row_format").toString();
            table.rowCount = query.value("Rows").toInt();

            double dataLength = query.value("Data_length").toDouble();
            double indexLength = query.value("Index_length").toDouble();

            table.rawSize = (dataLength + indexLength) / 1024;
            table.size = this->getSizeString(table.rawSize);

            this->tableList << table;
        }

        emit layoutChanged();
    } else {
        qWarning() << "DatabaseModel::reload - " + query.lastError().text();
    }
}

QString DatabaseModel::getSizeString(double size) {
    if (size < 1000){
        return QString("%1 Kb").arg(QString::number(size, 'f', 0));
    }
    else{
        size = size / 1024;
        if (size < 1000){
            return QString("%1 Mb").arg(QString::number(size, 'f', 0));
        }
        else{
            size = size / 1024;
            return QString("%1 Gb").arg(QString::number(size, 'f', 2));
        }
    }
}

} /* namespace Database */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
