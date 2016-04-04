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
#include <QLabel>
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
    void setTable(QSqlDatabase database, QString tableName);

public slots:
	void queryError(QString query, QString error);
	void applyFilterClicked(bool checked);
	void customContextMenuRequested(QPoint point);
	void handleSetNullAction();
	void handleCopyAction();
	void handlePastAction();
	void handleDeleteAction();
	void handleFilterColumnLikeAction();
	void handleFilterColumnEqualAction();
    void refreshData();

private:
	QTableView *tableData;
	QLabel *tableInfoLabel;
	TableFilterTextEdit *whereConditionText;
	QModelIndex contextMenuIndex;
};

} /* namespace Table */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_TableTab_H_ */
