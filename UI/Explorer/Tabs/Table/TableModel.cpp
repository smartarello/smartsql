/*
 * TableDataTabModel.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Table/TableModel.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
namespace UI {
namespace Explorer {
namespace Tabs {
namespace Table {

TableModel::TableModel(QObject * parent) : QAbstractTableModel(parent) {


}

void TableModel::setTable(QString table){
	this->table = table;
	this->sortOrder = "";
	this->filter = "";

	QSqlQuery query;
	query.exec(QString("SHOW COLUMNS FROM %1").arg(this->table));
	this->columns = QList<QString>();
	while(query.next()){
		this->columns.append(query.value("Field").toString());
	}

	QSqlQuery queryIndex;
	queryIndex.exec(QString("SHOW INDEX FROM %1 WHERE Key_name LIKE 'PRIMARY'").arg(this->table));
	this->primaryKey = QStringList();
	while(queryIndex.next()){
		this->primaryKey << queryIndex.value("Column_name").toString();
	}

	this->reload();
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
	}

	return QVariant();
}

QList<QString> TableModel::getColumns(){
	return this->columns;
}

void TableModel::sort(int column, Qt::SortOrder order){
	if (column >= this->columns.count()){
		return ;
	}

	QString sortOrder = QString("ASC");
	if (order == Qt::AscendingOrder){
		sortOrder = QString("DESC");
	}

	QString col = this->columns.at(column);

	this->sortOrder = col + " " + sortOrder;

	this->reload();
}

void TableModel::refreshWithFilter(QString filter)
{
	QString oldFilter = this->filter;
	this->filter = filter;
	QString queryText = this->buildQuery();
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

    	QString columnName = record.fieldName(index.column());

    	QString updateQuery = QString("UPDATE %1 SET %2=:%3 WHERE ").arg(this->table).arg(columnName).arg(columnName);
    	QStringList where;
    	foreach(QString primaryKeyIndex, this->primaryKey) {
    		where << QString("%1=:%2").arg(primaryKeyIndex).arg(primaryKeyIndex);
    	}

    	updateQuery += where.join(" AND ");

    	QSqlQuery query ;
    	query.prepare(updateQuery);
    	query.bindValue(":"+columnName, value);

    	foreach(QString primaryKeyIndex, this->primaryKey) {
    		query.bindValue(":"+primaryKeyIndex, record.value(primaryKeyIndex));
    	}

    	query.exec();

    	if (query.lastError().isValid()){
    		qDebug() << query.lastError();
    		emit queryError(updateQuery, query.lastError().text());
    		return false;
    	} else {
    		record.setValue(index.column(), value);
    		this->results.replace(index.row(), record);
    		emit dataChanged(index, index);
    		return true;
    	}
    }

    return false;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
	if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.isValid() && index.row() < this->results.size()) {
		QSqlRecord record = this->results.at(index.row());
		return record.value(index.column());
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

	qDebug() << deleteQuery;

	QSqlQuery query ;
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
	QString sql = this->buildQuery();
	qDebug() << sql;
	this->query.exec(sql);

	if (this->query.lastError().isValid()){
		this->filter = "";
		qDebug() << this->query.lastError();
		QString lastError =  this->query.lastError().text();
		emit queryError("", lastError);
	} else {
		this->results = QList<QSqlRecord>();
		while (this->query.next()) {
			this->results << this->query.record();
		}

		emit layoutChanged();
	}
}

TableModel::~TableModel() {

}

} /* namespace Model */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
