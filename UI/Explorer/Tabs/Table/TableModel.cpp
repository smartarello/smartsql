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

	this->setQuery(this->buildQuery());

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

	emit layoutChanged();
}

int TableModel::rowCount(const QModelIndex & parent) const
{
	return query.size();
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

	this->setQuery(this->buildQuery());
	emit layoutChanged();
}

void TableModel::refreshWithFilter(QString filter)
{
	QString oldFilter = this->filter;
	this->filter = filter;
	QString queryText = this->buildQuery();
	this->setQuery(queryText);
	if (this->query.lastError().isValid()){
		qDebug() << this->query.lastError();
		QString lastError =  this->query.lastError().text();
		this->filter = oldFilter;
		this->setQuery(this->buildQuery());
		emit queryError(queryText, lastError);
	}
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

	qDebug() << query;

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
	if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.isValid() && index.row() < query.size()) {
		QSqlRecord record = this->results.at(index.row());
		return record.value(index.column());
	}

	return QVariant();
}

bool TableModel::removeRows(int row, int count, const QModelIndex & parent)
{
	if (this->results.count() < row) {
		return false;
	}

	beginRemoveRows(parent, row, row);

	QSqlRecord record = this->results.at(row);

	QString deleteQuery = QString("DELETE FROM %1 WHERE ").arg(this->table);
	QStringList where;
	foreach(QString primaryKeyIndex, this->primaryKey) {
		where << QString("%1=:%2").arg(primaryKeyIndex).arg(primaryKeyIndex);
	}

	deleteQuery += where.join(" AND ");

	QSqlQuery query ;
	query.prepare(deleteQuery);

	foreach(QString primaryKeyIndex, this->primaryKey) {
		query.bindValue(":"+primaryKeyIndex, record.value(primaryKeyIndex));
	}

	query.exec();

	if (query.lastError().isValid()){
		qDebug() << query.lastError();
		emit queryError("", query.lastError().text());
		return false;
	}

	this->results.removeAt(row);
	endRemoveRows();
	return true;
}

void TableModel::setQuery(QString sql)
{
	this->query.exec(sql);
	this->results = QList<QSqlRecord>();
	while (this->query.next()) {
		this->results << this->query.record();
	}
}

TableModel::~TableModel() {

}

} /* namespace Model */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
