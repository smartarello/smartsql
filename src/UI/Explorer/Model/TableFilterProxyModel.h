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
