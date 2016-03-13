/*
 * QueryTab.h
 *
 *  Created on: Mar 12, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_QUERY_QUERYTAB_H_
#define UI_EXPLORER_TABS_QUERY_QUERYTAB_H_

#include <qsplitter.h>
#include <QTableView>
#include "QueryTextEdit.h"

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

class QueryTab: public QSplitter {

	Q_OBJECT
public:
	QueryTab(QWidget *parent = 0);
	virtual ~QueryTab();

public slots:
	void queryChanged(QString query);

private:
	QTableView *tableData;
	QueryTextEdit *queryTextEdit;
};

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_QUERY_QUERYTAB_H_ */
