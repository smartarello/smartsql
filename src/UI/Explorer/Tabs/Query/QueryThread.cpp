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
#include <QDateTime>
#include <QUuid>


namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryThread::QueryThread(QStringList queryList, QObject * parent ) : QThread(parent) {

	this->queries = queryList;
}

void QueryThread::run()
{
	qDebug() << "Start thread execution";

    QSqlDatabase db = QSqlDatabase::database();
    this->database = QSqlDatabase::cloneDatabase(db, QUuid::createUuid().toString());

	QList<QSqlQuery> results;

	if (this->database.open()) {

        QSqlQuery query(this->database);
        if (query.exec("SELECT CONNECTION_ID()")) {
            query.next();
            this->connectionId = query.value(0).toString();
        }


		foreach(QString sql, this->queries) {

			QSqlQuery query(this->database);

			QDateTime mStartTime = QDateTime::currentDateTime();
			if (query.exec(sql)) {

				QueryExecutionResult result;
				result.msec = mStartTime.msecsTo(QDateTime::currentDateTime());
				result.query = query;

				this->queryResult << result;
			} else {
				qDebug() << "QueryThread::run - " + query.lastError().text();
				QueryExecutionResult result;
				result.msec = 0;
				result.query = query;
				this->queryResult << result;
				break;
			}
		}

		emit queryResultReady();
	} else {
		qWarning() << this->database.lastError();
	}

	qDebug() << "End of thread execution";
}

void QueryThread::killQuery()
{
    if (!this->connectionId.isEmpty()) {
        QSqlQuery killQuery;
        killQuery.exec("KILL "+this->connectionId);
    }
}

QList<QueryExecutionResult> QueryThread::getQueryResult()
{
	return this->queryResult;
}


QueryThread::~QueryThread() {


}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
