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

#include <UI/Explorer/Tabs/Query/QueryModel.h>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QFont>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryModel::QueryModel(QList<QSqlRecord> data, QObject * parent) : QAbstractTableModel(parent) {
    this->records = data;
}


QVariant QueryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < this->records.size()) {
        QSqlRecord record = this->records.at(index.row());
        QVariant value = record.value(index.column());

        if (value.isNull() && role == Qt::DisplayRole) {
                return QVariant("(NULL)");
        }
        else if (role == Qt::DisplayRole) {
            return value;
        }
        else if (role == Qt::FontRole && value.isNull()) {

            QFont font("DejaVue Sans Mono");
            font.setItalic(true);
            return QVariant(font);
        }
    }

    return QVariant();
}

int QueryModel::rowCount(const QModelIndex & parent) const
{
    return this->records.size();
}

int QueryModel::columnCount(const QModelIndex & parent) const
{
    if (this->records.isEmpty()) {
        return 0;
    } else {
        return this->records.at(0).count();
    }
}

QVariant QueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && !this->records.isEmpty()) {
        QSqlRecord r = this->records.at(0);
        return r.fieldName(section);
    }

    return QVariant();
}

QueryModel::~QueryModel() {

    this->records.clear();
}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
