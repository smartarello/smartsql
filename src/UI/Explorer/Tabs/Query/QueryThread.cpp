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

QueryThread::QueryThread(ConnectionConfiguration connection, QString query, QObject * parent ) :
    QThread(parent),
    connection(connection),
    query(query)
{

}

void QueryThread::run()
{
    QSqlDatabase database = Util::DataBase::createFromConfig(this->connection);

    QList<QueryExecutionResult>  results;
    if (database.open()) {

        QSqlQuery query(database);
        if (query.exec("SELECT CONNECTION_ID()")) {
            query.next();
            this->connectionId = query.value(0).toString();



            QSqlQuery query(database);

            QDateTime mStartTime = QDateTime::currentDateTime();
            if (query.exec(this->query)) {

                qint64 msec = mStartTime.msecsTo(QDateTime::currentDateTime());

                do {

                    QueryExecutionResult result;
                    result.msec = msec;
                    result.rows = query.size();
                    if (query.isSelect()) {
                        result.isSelect = true;
                        QList<QSqlRecord> data;
                        if (query.size() > 1000) {
                            result.limitedResult = true;
                        } else {
                            result.limitedResult = false;
                        }

                        int i = 0;
                        while (query.next() && i++ < 1000) {
                            data << query.record();
                        }

                        result.data = data;
                    } else {
                        result.query = query.executedQuery();
                        result.isSelect = false;
                        result.affectedRows = query.numRowsAffected();
                    }


                    results << result;
                } while (query.nextResult());
            } else {
                qDebug() << "QueryThread::run - " + query.lastError().text();
                QueryExecutionResult result;
                result.error = query.lastError().text();
                results << result;
            }

        }

        database.close();
        emit queryResultReady(results);
        results.clear();

	} else {
        qWarning() << database.lastError();
        QueryExecutionResult result;
        result.error = database.lastError().text();
        results << result;
        emit queryResultReady(results);
	}

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
