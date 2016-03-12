/*
 * TableTab.h
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_TableTab_H_
#define UI_EXPLORER_TABS_TableTab_H_

#include <QSplitter>
#include <QTableView>
#include "TableFilterTextEdit.h"
#include <QSqlDatabase>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Table {

class TableTab: public QSplitter {

	Q_OBJECT

public:
	TableTab(QWidget *parent = 0);
	virtual ~TableTab();
	void setTable(QString tableName);

public slots:
	void queryError(QString query, QString error);

private:
	QTableView *tableData;
	TableFilterTextEdit *whereConditionText;
};

} /* namespace Table */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_TableTab_H_ */
