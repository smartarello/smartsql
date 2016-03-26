/*
 * DataBaseTree.h
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_DATABASETREE_H_
#define UI_EXPLORER_DATABASETREE_H_

#include <QTreeView>
#include <QJsonObject>
#include "Model/DataBaseModel.h"
#include "ShowProcessesWindow.h"

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


private:
	Model::DataBaseModel *dataBaseModel;
	QModelIndex contextMenuIndex;
};

} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_DATABASETREE_H_ */
