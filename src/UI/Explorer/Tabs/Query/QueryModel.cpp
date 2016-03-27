/*
 * QueryModel.cpp
 *
 *  Created on: Mar 14, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Query/QueryModel.h>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryModel::QueryModel(QObject * parent) : QSqlQueryModel(parent) {


}

void QueryModel::sort(int column, Qt::SortOrder order)
{
	if (!this->query().isValid()){
		return;
	}

	QSqlRecord record = this->query().record();
	if (!record.isEmpty()){
		QString field = record.fieldName(column);
		if (field.isEmpty()){
			return ;
		}

		QString sqlQuery = "SELECT * FROM (" + this->query().lastQuery() + ") as mysqlexplorer_tmp ORDER BY "+field;
		if (order == Qt::AscendingOrder){
			sqlQuery += " DESC";
		}

		QSqlQuery query;
		query.exec(sqlQuery);

		if (query.lastError().isValid()){
			qDebug() << "QueryModel::sort - " + query.lastError().text();
		} else {
			this->setQuery(query);
		}
	}
}

QueryModel::~QueryModel() {

}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
