/*
 * QueryThread.cpp
 *
 *  Created on: Mar 20, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Query/QueryThread.h>
#include <QDebug>
#include <QSqlError>
#include <QSqlResult>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryThread::QueryThread(QSqlDatabase database, QStringList queryList, QObject * parent ) : QThread(parent) {
	this->database = database;
	this->queries = queryList;
}

void QueryThread::run()
{
	qDebug() << "Start thread execution";

	QList<QSqlQuery> results;

	if (this->database.open()) {
		foreach(QString sql, this->queries) {

			QSqlQuery query(sql, this->database);
			query.exec();

			this->queryResult << query;

			if (query.lastError().isValid()) {
				break;
			}
		}
	}

	this->database.close();

	qDebug() << "End of thread execution";
	emit queryResultReady();
}

QList<QSqlQuery> QueryThread::getQueryResult()
{
	return this->queryResult;
}


QueryThread::~QueryThread() {


}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
