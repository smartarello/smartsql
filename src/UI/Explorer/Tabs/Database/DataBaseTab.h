/*
 * DataBaseTab.h
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_DATABASETAB_H_
#define UI_EXPLORER_TABS_DATABASETAB_H_

#include <QTableView>
#include <QSqlDatabase>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Database {

class DataBaseTab: public QTableView {

	Q_OBJECT

public:
	DataBaseTab(QWidget * parent = 0);
	virtual ~DataBaseTab();
	void refresh();
};

} /* namespace Database */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_DATABASETAB_H_ */
