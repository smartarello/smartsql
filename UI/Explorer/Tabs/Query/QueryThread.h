/*
 * QueryThread.h
 *
 *  Created on: Mar 20, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_QUERY_QUERYTHREAD_H_
#define UI_EXPLORER_TABS_QUERY_QUERYTHREAD_H_

#include <qthread.h>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlResult>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

class QueryThread: public QThread {

	Q_OBJECT

public:
	QueryThread(QSqlDatabase database, QStringList queryList, QObject * parent = 0);
	virtual ~QueryThread();
	virtual void run();
	QList<QSqlQuery> getQueryResult();

private:
	QStringList queries;
	QList<QSqlQuery> queryResult;
	QSqlDatabase database;

signals:
	void queryResultReady();
};

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_QUERY_QUERYTHREAD_H_ */
