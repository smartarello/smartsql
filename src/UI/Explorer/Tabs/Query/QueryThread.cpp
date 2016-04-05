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

QueryThread::QueryThread(ConnectionConfiguration connection, QStringList queryList, QObject * parent ) :
    QThread(parent),
    connection(connection),
    queries(queryList)
{

}

void QueryThread::run()
{
    qDebug() << "QueryThread::run - Start thread execution";

    QSqlDatabase database = Util::DataBase::createFromConfig(this->connection);

    QList<QueryExecutionResult>  results;
    if (database.open()) {

        QSqlQuery query(database);
        if (query.exec("SELECT CONNECTION_ID()")) {
            query.next();
            this->connectionId = query.value(0).toString();

            foreach(QString sql, this->queries) {

                QSqlQuery query(database);

                QDateTime mStartTime = QDateTime::currentDateTime();
                if (query.exec(sql)) {

                    QueryExecutionResult result;
                    result.msec = mStartTime.msecsTo(QDateTime::currentDateTime());
                    result.rows = query.size();
                    if (query.isSelect()) {
                        result.isSelect = true;
                        QList<QSqlRecord> data;
                        if (query.size() > 100000) {
                            result.limitedResult = true;
                        } else {
                            result.limitedResult = false;
                        }

                        int i = 0;
                        while (query.next() && i++ < 100000) {
                            data << query.record();
                        }

                        result.data = data;
                    } else {
                        result.query = sql;
                        result.isSelect = false;
                        result.affectedRows = query.numRowsAffected();
                    }


                    results << result;
                } else {
                    qDebug() << "QueryThread::run - " + query.lastError().text();
                    QueryExecutionResult result;
                    result.error = query.lastError().text();
                    results << result;
                    break;
                }
            }
        }

        database.close();
        emit queryResultReady(results);

	} else {
        qWarning() << database.lastError();
	}

    qDebug() << "QueryThread::run - End of thread execution";
}

void QueryThread::killQuery()
{
    if (!this->connectionId.isEmpty()) {
        QSqlDatabase database = Util::DataBase::createFromConfig(this->connection);

        QSqlQuery killQuery(database);
        killQuery.exec("KILL "+this->connectionId);
        database.close();
    }
}


QueryThread::~QueryThread() {
}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
