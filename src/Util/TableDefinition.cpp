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
#include "TableDefinition.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

namespace Util {
TableDefinition::TableDefinition(QSqlDatabase connection, QString tableName):
    table(tableName)
{
    if (connection.isOpen() || connection.open()) {

        QSqlQuery query(connection);
        if (query.exec(QString("SHOW CREATE TABLE %1").arg(tableName)) && query.next()) {

            create = query.value(1).toString();

            QStringList createParts = create.split("\n");
            createParts.removeAt(0); // Removes the first line with CREATE TABLE
            foreach(QString part, createParts) {

                // Search column definition
                // e.g `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
                QRegExp line("^\\s*`(\\S+)` (\\w+)\\(?(\\d*)\\)?", Qt::CaseInsensitive);
                if (line.indexIn(part) != -1) { // Column definition
                    ColumnDefinition column;
                    column.name = line.cap(1);
                    column.type = line.cap(2);
                    column.length = -1;

                    if (line.captureCount() == 3) {
                        QString length = line.cap(3);
                        if (!length.isEmpty()) {
                            column.length = line.cap(3).toInt();
                        }
                    }

                    QRegExp unsignedRx("unsigned", Qt::CaseInsensitive);
                    if (unsignedRx.indexIn(part) != -1) {
                        column.unsignedCol = true;
                    } else {
                        column.unsignedCol = false;
                    }

                    QRegExp notNullRx("not null", Qt::CaseInsensitive);
                    if (notNullRx.indexIn(part) != -1) {
                        column.allowNull = false;
                    } else {
                        column.allowNull = true;
                    }

                    QRegExp defaultValueRx("default '(\\w)'", Qt::CaseInsensitive);
                    if (defaultValueRx.indexIn(part) != -1) {
                        column.defaultValue = defaultValueRx.cap(1);
                    } else {
                        column.defaultValue = "";
                    }

                    defaultValueRx = QRegExp("default null", Qt::CaseInsensitive);
                    if (defaultValueRx.indexIn(part) != -1) {
                        column.defaultValue = QVariant();
                    }

                    this->cols << column;
                }

                // Primary key definition
                QRegExp primaryKeyRx("^\\s*PRIMARY KEY \\((.*)\\)", Qt::CaseInsensitive);
                if (primaryKeyRx.indexIn(part) != -1) {
                    IndexDefinition index;
                    index.keyName = "PRIMARY KEY";
                    index.columns = primaryKeyRx.cap(1).replace("`", "").split(",");
                    index.type = "PRIMARY KEY";

                    this->idx << index;
                    this->pk = index.columns;
                }

                // Index definition
                QRegExp keyRx("^\\s*KEY `(.+)` \\((.+)\\)", Qt::CaseInsensitive);
                if (keyRx.indexIn(part) != -1) {

                    IndexDefinition index;
                    index.keyName = keyRx.cap(1);
                    index.columns = keyRx.cap(2).replace("`", "").split(",");
                    index.type = "KEY";

                    this->idx << index;
                }

                // Unique index definition
                QRegExp uniquekeyRx("^\\s*UNIQUE KEY `(.+)` \\((.+)\\)", Qt::CaseInsensitive);
                if (uniquekeyRx.indexIn(part) != -1) {

                    IndexDefinition index;
                    index.keyName = uniquekeyRx.cap(1);
                    index.columns = keyRx.cap(2).replace("`", "").split(",");
                    index.type = "UNIQUE";

                    this->idx << index;
                }

                // Foreign key definition
                QRegExp foreignKeyRx("^\\s*CONSTRAINT `(.+)` FOREIGN KEY \\((.+)\\) REFERENCES `(.+)` \\((.+)\\)", Qt::CaseInsensitive);
                if (foreignKeyRx.indexIn(part) != -1) { // Foreign key definition

                    ForeignKeyDefinition fk;
                    fk.keyName = foreignKeyRx.cap(1);
                    fk.column = foreignKeyRx.cap(2);
                    fk.foreignTable = foreignKeyRx.cap(3);
                    fk.foreignColumn = foreignKeyRx.cap(4);

                    QRegExp onDeleteActionRx("ON DELETE (RESTRICT|CASCADE|SET NULL|NO ACTION)", Qt::CaseInsensitive);
                    if (onDeleteActionRx.indexIn(part) != -1) {
                        fk.onDelete = onDeleteActionRx.cap(1);
                    } else {
                        fk.onDelete = "RESTRICT";
                    }

                    QRegExp onUpdateActionRx("ON UPDATE (RESTRICT|CASCADE|SET NULL|NO ACTION)", Qt::CaseInsensitive);
                    if (onUpdateActionRx.indexIn(part) != -1) {
                        fk.onUpdate = onUpdateActionRx.cap(1);
                    } else {
                        fk.onUpdate = "RESTRICT";
                    }

                    this->fks << fk;
                }
            }
        }


    } else {
        qDebug() << connection.lastError().text();
    }
}


QList<ColumnDefinition> TableDefinition::columns() const
{
    return this->cols;
}

QList<IndexDefinition> TableDefinition::indexes() const
{
    return this->idx;
}

QList<ForeignKeyDefinition> TableDefinition::foreignKeys() const
{
    return this->fks;
}

QStringList TableDefinition::primaryKey() const
{
    return this->pk;
}

QString TableDefinition::createTable() const
{
    return this->create;
}

QString TableDefinition::name() const
{
    return this->table;
}
}

