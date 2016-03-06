/*
 * TableDataTabModel.h
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_MODEL_TABLEDATATABMODEL_H_
#define UI_EXPLORER_TABS_MODEL_TABLEDATATABMODEL_H_

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QObject>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Model {

class TableDataTabModel: public QSqlTableModel {

	Q_OBJECT

public:
	TableDataTabModel(QObject * parent = 0);
	virtual ~TableDataTabModel();
//	void sort(int column, Qt::SortOrder order);
};

} /* namespace Model */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_MODEL_TABLEDATATABMODEL_H_ */
