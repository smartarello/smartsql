/*
 * TableFilterProxyModel.h
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_MODEL_TABLEFILTERPROXYMODEL_H_
#define UI_EXPLORER_MODEL_TABLEFILTERPROXYMODEL_H_

#include <qsortfilterproxymodel.h>
#include <QObject>
#include <QModelIndex>

namespace UI {
namespace Explorer {
namespace Model {

class TableFilterProxyModel: public QSortFilterProxyModel {

	Q_OBJECT

public:
	TableFilterProxyModel(QObject * parent = 0);
	virtual ~TableFilterProxyModel();
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
	void setTableFilter(QString tableFilter);
	void setDatabaseFilter(QString databaseFilter);

private:
	QString database;
	QString table;
};

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_MODEL_TABLEFILTERPROXYMODEL_H_ */
