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

#ifndef UI_EXPLORER_TABS_QUERY_QUERYTHREAD_H_
#define UI_EXPLORER_TABS_QUERY_QUERYTHREAD_H_

#include <QThread>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlResult>
#include <QJsonObject>
#include <QSqlRecord>
#include "Util/DataBase.h"

struct QueryExecutionResult {
    QList<QSqlRecord> data;
    qint64 msec; // millisecond
    bool isSelect;
    int affectedRows;
    int rows;
    QString error;
    QString query;
    bool limitedResult;
} ;

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

class QueryThread: public QThread {

	Q_OBJECT

public:


    QueryThread(ConnectionConfiguration connection, QStringList queryList, QObject * parent = 0);
	virtual ~QueryThread();
	virtual void run();
    void killQuery();

private:
	QStringList queries;
    QString connectionId;
    ConnectionConfiguration connection;

signals:
    void queryResultReady(QList<QueryExecutionResult>);
};

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_QUERY_QUERYTHREAD_H_ */
