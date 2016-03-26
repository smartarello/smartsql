/*
 * QueryModel.h
 *
 *  Created on: Mar 14, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_QUERY_QUERYMODEL_H_
#define UI_EXPLORER_TABS_QUERY_QUERYMODEL_H_

#include <qsqlquerymodel.h>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

class QueryModel: public QSqlQueryModel {

	Q_OBJECT

public:
	QueryModel(QObject * parent = 0);
	virtual ~QueryModel();

private:
	void sort(int column, Qt::SortOrder order);
};

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_QUERY_QUERYMODEL_H_ */
