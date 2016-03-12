/*
 * TableDataTabModel.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Model/TableDataTabModel.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
namespace UI {
namespace Explorer {
namespace Tabs {
namespace Model {

TableDataTabModel::TableDataTabModel(QObject * parent) : QSqlQueryModel(parent) {


}

void TableDataTabModel::setTable(QString table){
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
}

QList<QString> TableDataTabModel::getColumns(){
	return this->columns;
}

void TableDataTabModel::sort(int column, Qt::SortOrder order){
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
}

void TableDataTabModel::refreshWithFilter(QString filter)
{
	this->filter = filter;
	QString queryText = this->buildQuery();
	QSqlQuery currentQuery = this->query();
	this->setQuery(queryText);
	if (this->lastError().isValid()){
		qDebug() << this->lastError();
		QString lastError =  this->lastError().text();
		this->setQuery(currentQuery);
		emit queryError(queryText, lastError);
	}
}

QString TableDataTabModel::buildQuery()
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

TableDataTabModel::~TableDataTabModel() {
	// TODO Auto-generated destructor stub
}

} /* namespace Model */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
