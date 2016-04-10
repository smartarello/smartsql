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
