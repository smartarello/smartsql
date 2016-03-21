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
	if (!sourceParent.isValid()){
		return true;
	}

	QModelIndex index = this->sourceModel()->index(sourceRow, 0, sourceParent);
	QString nodeValue = this->sourceModel()->data(index).toString();

	// Database node
	if (!sourceParent.parent().isValid()) {
		return this->database.isEmpty() || nodeValue.contains(this->database, Qt::CaseInsensitive);
	} else { // Table node
		return this->table.isEmpty() || nodeValue.contains(this->table, Qt::CaseInsensitive);
	}

    return true;
}

void TableFilterProxyModel::setTableFilter(QString tableFilter)
{
	this->table = tableFilter;
	this->invalidateFilter();
}

void TableFilterProxyModel::setDatabaseFilter(QString databaseFilter)
{
	this->database = databaseFilter;
	this->invalidateFilter();
}

TableFilterProxyModel::~TableFilterProxyModel() {
	// TODO Auto-generated destructor stub
}

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */
