/*
 * QueryThread.h
 *
 *  Created on: Mar 20, 2016
 *      Author: stephane
 */

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
