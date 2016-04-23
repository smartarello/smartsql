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

#ifndef UI_EXPLORER_DATABASETREE_H_
#define UI_EXPLORER_DATABASETREE_H_

#include <QTreeView>
#include <QJsonObject>
#include "Model/DataBaseModel.h"
#include "ServerAction/ShowProcessesWindow.h"
#include "Export/ExportWindow.h"

namespace UI {
namespace Explorer {

class DataBaseTree: public QTreeView {

	Q_OBJECT

public:
    DataBaseTree(QWidget *parent, QJsonObject sessionUuid);
	virtual ~DataBaseTree();
	Model::DataBaseModel * getDataBaseModel();

public slots:
	void filterTable(QString text);
	void filterDatabase(QString text);
	void customContextMenuRequested(QPoint point);
	void handleShowProcesses();
	void handleRefreshDatabase();
	void handleDrop();
	void handleTruncateTable();
	void handleDisconnect();
	void queryError(QString error);
    void handleCreateDatabase();
    void createDatabase(QString databaseName, QString collation);
    void handleShowDetailsTable();
    void handleOpenTableInTab();
    void handleExportTableAsSql();
    void exportWindowDestroyed();
    void processListWindowDestroyed();



signals:
	void closeExplorer();
    void openTableInNewTab();

private:
	Model::DataBaseModel *dataBaseModel;
	QModelIndex contextMenuIndex;
    bool exportWindowOpened = false;
    bool processListWindowOpened = false;
};

} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_DATABASETREE_H_ */
