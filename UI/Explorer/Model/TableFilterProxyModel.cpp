/*
 * TableFilterProxyModel.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Model/TableFilterProxyModel.h>
#include <QDebug>

namespace UI {
namespace Explorer {
namespace Model {

TableFilterProxyModel::TableFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent) {
	// TODO Auto-generated constructor stub

}

bool TableFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	// Always accept the server and the database, only filter the table name
	if (!sourceParent.isValid() || !sourceParent.parent().isValid()){
		return true;
	}

    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}

TableFilterProxyModel::~TableFilterProxyModel() {
	// TODO Auto-generated destructor stub
}

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */
