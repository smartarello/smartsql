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

#ifndef UI_EXPLORER_TABS_QUERY_QUERYTAB_H_
#define UI_EXPLORER_TABS_QUERY_QUERYTAB_H_

#include <qsplitter.h>
#include <QTabWidget>
#include <QSqlRecord>
#include <QPushButton>
#include "QueryTextEdit.h"
#include "QueryThread.h"


namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

class QueryTab: public QSplitter {

	Q_OBJECT
public:

	QueryTab(QWidget *parent = 0);
	virtual ~QueryTab();
	void focus();

public slots:
	void queryChanged();
	void stopQueries();
    void handleQueryResultReady(QList<QueryExecutionResult> results);

private:
	QueryTextEdit *queryTextEdit;
	QTabWidget *queryTabs;
	QueryThread *queryWorker;
	QPushButton *executeButton;
	QPushButton *stopButton;
};

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_QUERY_QUERYTAB_H_ */
