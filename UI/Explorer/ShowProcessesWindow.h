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

namespace UI {
namespace Explorer {

class ShowProcessesWindow: public QMainWindow {

	Q_OBJECT

public:
	ShowProcessesWindow(QWidget * parent = 0);
	virtual ~ShowProcessesWindow();

private:
	QTableView *processListTable;
};

} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_SHOWPROCESSESWINDOW_H_ */
