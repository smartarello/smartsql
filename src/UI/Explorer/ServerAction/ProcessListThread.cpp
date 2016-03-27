/*
 * ProcessListThread.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: stephane
 */

#include <UI/Explorer/ServerAction/ProcessListThread.h>
#include <QSqlError>
#include <QDebug>

namespace UI {
namespace Explorer {
namespace ServerAction {

ProcessListThread::ProcessListThread(QSqlDatabase database, QObject *parent) : QThread(parent), database(database) {



}

void ProcessListThread::run()
{
	if (this->database.open()) {
		this->query = QSqlQuery(this->database);

		while(true) {
			if (this->query.exec("SELECT * FROM `information_schema`.`PROCESSLIST`")) {
				emit processListReady();
			} else {
				qWarning() << "ProcessListThread::run - " + this->query.lastError().text();
				return; // Stop the thread execution
			}

			this->sleep(3);
		}
	} else {
		qDebug() << "Unable to open the database for the process list thread";
	}
}

QSqlQuery ProcessListThread::getQuery()
{
	return this->query;
}

ProcessListThread::~ProcessListThread() {

}

} /* namespace ServerAction */
} /* namespace Explorer */
} /* namespace UI */
