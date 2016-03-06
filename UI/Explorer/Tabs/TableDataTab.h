/*
 * TableDataTab.h
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_TABLEDATATAB_H_
#define UI_EXPLORER_TABS_TABLEDATATAB_H_

#include <QSplitter>
#include <QTableView>
#include <QTextEdit>
#include <QSqlDatabase>

namespace UI {
namespace Explorer {
namespace Tabs {

class TableDataTab: public QSplitter {

	Q_OBJECT

public:
	TableDataTab(QWidget *parent = 0);
	virtual ~TableDataTab();
	void setTable(QString tableName);

private:
	QTableView *tableData;
	QTextEdit *whereConditionText;
};

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_TABLEDATATAB_H_ */
