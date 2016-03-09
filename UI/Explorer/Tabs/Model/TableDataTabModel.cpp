/*
 * TableDataTabModel.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Model/TableDataTabModel.h>
#include <QSqlQuery>
#include <QDebug>
namespace UI {
namespace Explorer {
namespace Tabs {
namespace Model {

TableDataTabModel::TableDataTabModel(QObject * parent) : QSqlQueryModel(parent) {


}

void TableDataTabModel::setTable(QString table){
	this->table = table;
	this->setQuery(QString("SELECT * FROM %1 LIMIT 1000").arg(this->table));

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
	if (order == Qt::DescendingOrder){
		sortOrder = QString("DESC");
	}

	QString col = this->columns.at(column);
	QString query = QString("SELECT * FROM %1 ORDER BY %2 %3 LIMIT 1000").arg(this->table).arg(col).arg(sortOrder);
	qDebug() << query;
	this->setQuery(query);
}

TableDataTabModel::~TableDataTabModel() {
	// TODO Auto-generated destructor stub
}

} /* namespace Model */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
