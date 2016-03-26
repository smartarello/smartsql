/*
 * ProcessListThread.h
 *
 *  Created on: Mar 24, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_PROCESSLISTTHREAD_H_
#define UI_EXPLORER_PROCESSLISTTHREAD_H_

#include <qthread.h>
#include <QSqlQuery>
#include <QSqlDatabase>

namespace UI {
namespace Explorer {

class ProcessListThread: public QThread {


	Q_OBJECT

public:
	ProcessListThread(QSqlDatabase database, QObject *parent = 0);
	virtual ~ProcessListThread();
	virtual void run();
	QSqlQuery getQuery();

private:
	QSqlQuery query;
	QSqlDatabase database;

signals:
	void processListReady();
};

} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_PROCESSLISTTHREAD_H_ */
