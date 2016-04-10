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

#ifndef UI_EXPLORER_PROCESSLISTTHREAD_H_
#define UI_EXPLORER_PROCESSLISTTHREAD_H_

#include <qthread.h>
#include <QSqlQuery>
#include <QSqlDatabase>

namespace UI {
namespace Explorer {
namespace ServerAction {

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

} /* namespace ServerAction */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_PROCESSLISTTHREAD_H_ */
