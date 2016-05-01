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
#ifndef TABLE_H
#define TABLE_H

#include <QVariant>
#include <QSqlDatabase>


struct ColumnDefinition
{
    QString name;
    QString type;
    int length;
    bool unsignedCol;
    bool allowNull;
    QVariant defaultValue;
};

struct IndexDefinition
{
    QString keyName;
    QStringList columns;
    QString type;
};

struct ForeignKeyDefinition
{
    QString keyName;
    QString column;
    QString foreignTable;
    QString foreignColumn;
    QString onUpdate;
    QString onDelete;
};

namespace Util {
class TableDefinition
{
public:
    TableDefinition(QSqlDatabase connection, QString tableName);
    QList<ColumnDefinition> columns() const;
    QList<IndexDefinition> indexes() const;
    QList<ForeignKeyDefinition> foreignKeys() const;
    QStringList primaryKey() const;
    QString createTable() const;
    QString name() const;

private:
    QString table;
    QList<ColumnDefinition> cols;
    QList<IndexDefinition> idx;
    QList<ForeignKeyDefinition> fks;
    QList<QString> pk;
    QString create;
};
}


#endif // TABLE_H
