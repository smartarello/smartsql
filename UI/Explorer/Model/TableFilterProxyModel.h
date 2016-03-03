/*
 * TableFilterProxyModel.h
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_MODEL_TABLEFILTERPROXYMODEL_H_
#define UI_EXPLORER_MODEL_TABLEFILTERPROXYMODEL_H_

#include <qsortfilterproxymodel.h>

namespace UI {
namespace Explorer {
namespace Model {

class TableFilterProxyModel: public QSortFilterProxyModel {

	Q_OBJECT

public:
	TableFilterProxyModel();
	virtual ~TableFilterProxyModel();
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_MODEL_TABLEFILTERPROXYMODEL_H_ */
