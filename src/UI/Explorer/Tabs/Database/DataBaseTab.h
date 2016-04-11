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

#ifndef UI_EXPLORER_TABS_DATABASETAB_H_
#define UI_EXPLORER_TABS_DATABASETAB_H_

#include <QTableView>
#include <QSqlDatabase>
#include <QModelIndex>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Database {

class DataBaseTab: public QTableView {

	Q_OBJECT

public:
	DataBaseTab(QWidget * parent = 0);
	virtual ~DataBaseTab();
	void refresh();

public slots:
    void handleDoubleClicked(QModelIndex index);

signals:
    void showTable(QString tableName);
};

} /* namespace Database */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_DATABASETAB_H_ */
