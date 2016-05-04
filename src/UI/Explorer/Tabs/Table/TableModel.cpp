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

#include <UI/Explorer/Tabs/Table/TableModel.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QFont>
#include <QSize>
#include <QDateTime>
#include <QDate>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Table {

TableModel::TableModel(QObject * parent) : QAbstractTableModel(parent) {


}

void TableModel::setTable(QSqlDatabase database, QString table, QString filter){

    this->database = database;
	this->table = table;
	this->sortOrder = "";
    this->filter = filter;
    this->columns = QList<QString>();
    this->primaryKey = QStringList();
    this->foreignKeys = QHash<QString, QStringList>();


    if (!this->database.isOpen() && !this->database.open()) {
        qWarning() << "TableModel::setTable - Unable to open the connection - " + this->database.lastError().text();
        return ;
    }

    QSqlQuery query(this->database);
	if (query.exec(QString("SHOW COLUMNS FROM %1").arg(this->table))) {

		while(query.next()){
			this->columns.append(query.value("Field").toString());
		}

        QSqlQuery queryIndex(this->database);
		if (queryIndex.exec(QString("SHOW INDEX FROM %1 WHERE Key_name LIKE 'PRIMARY'").arg(this->table))) {

			while(queryIndex.next()){
				this->primaryKey << queryIndex.value("Column_name").toString();
			}

			this->reload();
		} else {
			qDebug() << "TableModel::setTable - " + queryIndex.lastError().text();
		}

        // Finds the foreign keys
        QString queryString = QString("SELECT  COLUMN_NAME, REFERENCED_TABLE_NAME, REFERENCED_COLUMN_NAME FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE WHERE TABLE_SCHEMA = '%1'  AND TABLE_NAME = '%2' AND REFERENCED_TABLE_NAME IS NOT NULL").arg(this->database.databaseName()).arg(table);
        QSqlQuery queryFK(this->database);
        if (query.exec(queryString)) {
            while (query.next()) {
                QStringList fk;
                fk << query.value(1).toString(); // Referenced table
                fk << query.value(2).toString(); // Referenced column
                this->foreignKeys.insert(query.value(0).toString(), fk);
            }
        }

	} else {
		qDebug() << "TableModel::setTable - " + query.lastError().text();
	}
}

int TableModel::rowCount(const QModelIndex & parent) const
{
	return this->results.size();
}

int TableModel::columnCount(const QModelIndex & parent) const
{
	if (!this->columns.isEmpty()) {
		return this->columns.count();
	}

	return 0;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (section < this->columns.count() && role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		return QVariant(this->columns.at(section));
    } else if (role == Qt::FontRole) {
        QFont font;
        font.setBold(false);
        return font;
    }

    return QVariant();
}

QList<QString> TableModel::getColumns(){
	return this->columns;
}

QHash<QString,QStringList> TableModel::getForeignKeys()
{
    return this->foreignKeys;
}

void TableModel::sort(int column, Qt::SortOrder order){
	if (column >= this->columns.count()){
		return ;
	}

    QString sortOrder = QString("DESC");
	if (order == Qt::AscendingOrder){
        sortOrder = QString("ASC");
	}

	QString col = this->columns.at(column);

	this->sortOrder = col + " " + sortOrder;

	this->reload();
}

void TableModel::refreshWithFilter(QString filter)
{
	this->filter = filter;
	this->reload();
}

QString TableModel::buildQuery()
{
	QString query = QString("SELECT * FROM %1 ").arg(this->table);
	if (!this->filter.isEmpty()){
		query += QString(" WHERE %1").arg(this->filter);
	}

	if (!this->sortOrder.isEmpty()){
		query += QString(" ORDER BY %1").arg(this->sortOrder);
	}

	query += " LIMIT 1000";

	return query;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()){
		return Qt::ItemIsEnabled;
	}

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole && index.row() < this->results.count()) {

    	QSqlRecord record = this->results.at(index.row());

        // The value is not changed
        if (value == record.value(index.column())) {
            return false;
        }

    	QString columnName = record.fieldName(index.column());

        QString updateQuery = QString("UPDATE %1 SET %2=:new_%3 WHERE ").arg(this->table).arg(columnName).arg(columnName);
    	QStringList where;

        if (!this->primaryKey.isEmpty()) {
            foreach(QString primaryKeyIndex, this->primaryKey) {
                where << QString("%1=:%2").arg(primaryKeyIndex).arg(primaryKeyIndex);
            }
        } else {
            // There is no primary key for this table
            for (int i = 0; i < record.count(); i++) {
                where << QString("%1=:%2").arg(record.fieldName(i)).arg(record.fieldName(i));
            }
        }

    	updateQuery += where.join(" AND ");

        if (!this->database.isOpen() && !this->database.open()) {
            return false;
        }

        QSqlQuery query(this->database) ;
        query.prepare(updateQuery);
        query.bindValue(":new_"+columnName, value);

        if (!this->primaryKey.isEmpty()) {
            foreach(QString primaryKeyIndex, this->primaryKey) {
                query.bindValue(":"+primaryKeyIndex, record.value(primaryKeyIndex));
            }
        } else {
            // There is no primary key for this table
            for (int i = 0; i < record.count(); i++) {
                query.bindValue(":"+record.fieldName(i), record.value(i));
            }
        }

        if (query.exec()){
    		record.setValue(index.column(), value);
    		this->results.replace(index.row(), record);
    		emit dataChanged(index, index);
    		return true;
    	} else {
    		qDebug() << "TableModel::setData - " + query.lastError().text();
    		emit queryError("", query.lastError().text());
    		return false;
    	}
    }

    return false;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::FontRole) && index.isValid() && index.row() < this->results.size()) {
		QSqlRecord record = this->results.at(index.row());
        QVariant value = record.value(index.column());

        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            if (value.isNull() && role == Qt::DisplayRole) {
                value = QVariant("(NULL)");
            } else if (value.type() == QVariant::DateTime) {
                return value.toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            } else if (value.type() == QVariant::Date) {
                return value.toDate().toString("yyyy-MM-dd");
            } else {
                return value;
            }

        } else if (value.isNull()) { // Qt::FontRole
            QFont font("DejaVue Sans Mono");
            font.setItalic(true);
            return QVariant(font);
        }
    }

	return QVariant();
}

bool TableModel::removeRows(int row, int count, const QModelIndex & parent)
{
	int lastRow = row + count -1;
	if (this->results.count() < row || this->results.count() <= lastRow) {
		return false;
	}

	// Compute the DELETE statement
	QString deleteQuery = QString("DELETE FROM %1 WHERE ").arg(this->table);
	QStringList orStatement;

	QList<QSqlRecord> recordToRemove;

	for (int i = row; i <= lastRow; i++) {

		QSqlRecord record = this->results.at(i);
		recordToRemove << record;

		// Adds a WHERE condition with the primary key for the current record
		QStringList where;
		foreach(QString primaryKeyIndex, this->primaryKey) {
			where << QString("%1='%2'").arg(primaryKeyIndex).arg(record.value(primaryKeyIndex).toString());
		}

		orStatement << where.join(" AND ");
	}


	deleteQuery += "(" + orStatement.join(") OR (") + ")";

    if (!this->database.isOpen() && !this->database.open()) {
        return false;
    }

    QSqlQuery query(this->database) ;
	query.exec(deleteQuery);

	if (query.lastError().isValid()){
		qDebug() << query.lastError();
		emit queryError("", query.lastError().text());
		return false;
	}

	beginRemoveRows(parent, row, lastRow);

	foreach (QSqlRecord record, recordToRemove) {
		this->results.removeOne(record);
	}

	endRemoveRows();
	return true;
}

void TableModel::reload()
{
    if (!this->database.isOpen() && !this->database.open()) {
        return;
    }

	QString sql = this->buildQuery();
    QSqlQuery query(this->database);
    query.exec(sql);

    if (query.lastError().isValid()){
		this->filter = "";
        qDebug() << "TableModel::reload - " + query.lastError().text();
        QString lastError =  query.lastError().text();
		emit queryError("", lastError);
	} else {
		this->results = QList<QSqlRecord>();
        while (query.next()) {
            this->results << query.record();
		}

		emit layoutChanged();
	}
}

TableModel::~TableModel() {
    this->database.close();
}

} /* namespace Model */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
