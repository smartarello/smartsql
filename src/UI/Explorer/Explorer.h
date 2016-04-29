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

#ifndef UI_EXPLORER_EXPLORER_H_
#define UI_EXPLORER_EXPLORER_H_

#include <UI/Explorer/Tabs/Table/TableTab.h>
#include <QWidget>
#include <QLineEdit>
#include <QModelIndex>
#include <QStandardItem>
#include <QItemSelection>
#include <QTabWidget>
#include "DataBaseTree.h"
#include "Tabs/Database/DataBaseTab.h"
#include "Tabs/Server/ServerTab.h"
#include "Tabs/TableDetails/TableDetailsTab.h"

namespace UI {
namespace Explorer {

class Explorer: public QWidget {

	Q_OBJECT

public:
	Explorer(QWidget *parent, QJsonObject sessionConf);
	virtual ~Explorer();
	void addDatabase(QJsonObject sessionConfiguration);

signals:
	void databaseChanged();
	void closeExplorer();

public slots:
    void dataBaseTreeItemChanged();
	void dataBaseTreeDoubleClicked(QModelIndex index);
	void addQueryTab();
	void closeQueryTab(int index);
	void refreshDatabase();
	void handleCloseExplorer();
    void handleOpenTableInTab();
    void handleShowTable(QString tableName);
    void handleShowDatabase(QString databaseName);
    void handleCurrentTabChanged(int index);

private:
	DataBaseTree *dataBaseTree;
	QLineEdit *tableFilterLineEdit;
	QLineEdit *databaseFilterLineEdit;
	QTabWidget *explorerTabs;
	Tabs::Database::DataBaseTab *databaseTab;
	Tabs::Table::TableTab *tableTab;
    ServerTab *serverTab;
    TableDetailsTab *tableDetailsTab;

};

} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_EXPLORER_H_ */
