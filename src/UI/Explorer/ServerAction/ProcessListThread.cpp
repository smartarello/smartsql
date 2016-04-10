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

        if (this->query.exec("SELECT * FROM `information_schema`.`PROCESSLIST`")) {
            emit processListReady();
        } else {
            qWarning() << "ProcessListThread::run - " + this->query.lastError().text();
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
