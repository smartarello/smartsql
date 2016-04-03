/*
 * ShowProcessesWindow.h
 *
 *  Created on: Mar 23, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_SHOWPROCESSESWINDOW_H_
#define UI_EXPLORER_SHOWPROCESSESWINDOW_H_

#include <qmainwindow.h>
#include <QTableView>
#include <QSqlDatabase>
#include <QJsonObject>
#include "ProcessListThread.h"

namespace UI {
namespace Explorer {
namespace ServerAction {

class ShowProcessesWindow: public QMainWindow {

	Q_OBJECT

public:
	ShowProcessesWindow(QJsonObject sessionConf, QWidget * parent = 0);
	virtual ~ShowProcessesWindow();

public slots:
	void processListReady();
    void killProcess();
    void refreshProcess();

private:
	QTableView *processListTable;
	ProcessListThread *worker;
    QSqlDatabase database;
};

} /* namespace ServerAction */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_SHOWPROCESSESWINDOW_H_ */
