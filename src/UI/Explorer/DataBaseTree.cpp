/*
 * DataBaseTree.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#include <UI/Explorer/DataBaseTree.h>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QHeaderView>
#include <QModelIndexList>
#include <QKeySequence>
#include <QShortcut>
#include <QMessageBox>
#include <QMenu>
#include "Explorer.h"
#include <UI/Explorer/Model/TableFilterProxyModel.h>
#include "ServerAction/NewDatabaseWindow.h"

namespace UI {
namespace Explorer {

DataBaseTree::DataBaseTree(QWidget *parent, QJsonObject sessionConf) : QTreeView(parent) {

	this->dataBaseModel = new Model::DataBaseModel(sessionConf);

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customContextMenuRequested(QPoint)));

	Model::TableFilterProxyModel *filter = new Model::TableFilterProxyModel();
	filter->setSourceModel(this->dataBaseModel);

	this->setModel(filter);
	this->setHeaderHidden(true);
	this->expandToDepth(0);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// The first column with the table name takes all the space.
	this->header()->setStretchLastSection(false);
	this->header()->setSectionResizeMode(0, QHeaderView::Stretch);

	QShortcut* refreshShortcut = new QShortcut(QKeySequence(Qt::Key_F5), this);
	refreshShortcut->setContext(Qt::WidgetShortcut);
	connect(refreshShortcut, SIGNAL(activated()), SLOT(handleRefreshDatabase()));

	QShortcut* deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
	deleteShortcut->setContext(Qt::WidgetShortcut);
	connect(deleteShortcut, SIGNAL(activated()), SLOT(handleDrop()));

	connect(this->dataBaseModel, SIGNAL(queryError(QString)), SLOT(queryError(QString)));
}

Model::DataBaseModel * DataBaseTree::getDataBaseModel()
{
	return this->dataBaseModel;
}

void DataBaseTree::filterTable(QString text)
{
	((Model::TableFilterProxyModel *)this->model())->setTableFilter(text);
}

void DataBaseTree::filterDatabase(QString text)
{
	((Model::TableFilterProxyModel *)this->model())->setDatabaseFilter(text);
}

void DataBaseTree::customContextMenuRequested(QPoint point)
{
	this->contextMenuIndex = ((Model::TableFilterProxyModel *)this->model())->mapToSource(this->indexAt(point));

	if (!this->contextMenuIndex.isValid()) {
		return ;
	}

	QModelIndexList list = this->selectionModel()->selectedRows(0);
	QMenu *menu = new QMenu(this);

	if (!this->contextMenuIndex.parent().isValid()){
		// Server node
		QAction *showProcessesAction = new QAction(tr("Processes"), this);
        showProcessesAction->setIcon(QIcon(":/resources/icons/database-process-icon.png"));
		menu->addAction(showProcessesAction);

		QAction *createDatabaseAction = new QAction(tr("Create database"), this);
        createDatabaseAction->setIcon(QIcon(":/resources/icons/database-add-icon.png"));
		menu->addAction(createDatabaseAction);

		QAction *disconnectAction = new QAction(tr("Disconnect"), this);
		disconnectAction->setIcon(QIcon(":/resources/icons/disconnect.png"));
		menu->addAction(disconnectAction);

		menu->addSeparator();

		QAction *refreshAction = new QAction(tr("Refresh"), this);
		refreshAction->setIcon(QIcon(":/resources/icons/refresh-icon.png"));
		refreshAction->setShortcut(QKeySequence(Qt::Key_F5));
		menu->addAction(refreshAction);

		connect(showProcessesAction, SIGNAL(triggered(bool)), SLOT(handleShowProcesses()));
		connect(refreshAction, SIGNAL(triggered(bool)), SLOT(handleRefreshDatabase()));
		connect(disconnectAction, SIGNAL (triggered(bool)), SLOT (handleDisconnect()));
        connect(createDatabaseAction, SIGNAL (triggered(bool)), SLOT (handleCreateDatabase()));
	} else if (!this->contextMenuIndex.parent().parent().isValid()) {
		// Database node

		QAction *dropDatabaseAction = new QAction(tr("Drop..."), this);
		dropDatabaseAction->setShortcut(QKeySequence(Qt::Key_Delete));
		dropDatabaseAction->setIcon(QIcon(":/resources/icons/delete-icon.png"));
		menu->addAction(dropDatabaseAction);

		menu->addSeparator();

		QAction *refreshAction = new QAction(tr("Refresh"), this);
		refreshAction->setShortcut(QKeySequence(Qt::Key_F5));
		refreshAction->setIcon(QIcon(":/resources/icons/refresh-icon.png"));
		connect(refreshAction, SIGNAL(triggered(bool)), SLOT(handleRefreshDatabase()));
		connect(dropDatabaseAction, SIGNAL(triggered(bool)), SLOT(handleDrop()));

		menu->addAction(refreshAction);
	} else {
		QAction *dropDatabaseAction = new QAction(tr("Drop..."), this);
		dropDatabaseAction->setShortcut(QKeySequence(Qt::Key_Delete));
		dropDatabaseAction->setIcon(QIcon(":/resources/icons/delete-icon.png"));
		connect(dropDatabaseAction, SIGNAL(triggered(bool)), SLOT(handleDrop()));
		menu->addAction(dropDatabaseAction);

		QAction *truncateAction = new QAction(tr("Empty table..."), this);
		truncateAction->setIcon(QIcon(":/resources/icons/empty-document-icon.png"));
		connect(truncateAction, SIGNAL(triggered(bool)), SLOT(handleTruncateTable()));
		menu->addAction(truncateAction);

		menu->addSeparator();

		QAction *refreshAction = new QAction(tr("Refresh"), this);
		refreshAction->setShortcut(QKeySequence(Qt::Key_F5));
		refreshAction->setIcon(QIcon(":/resources/icons/refresh-icon.png"));
		connect(refreshAction, SIGNAL(triggered(bool)), SLOT(handleRefreshDatabase()));
		menu->addAction(refreshAction);
	}

	menu->popup(this->viewport()->mapToGlobal(point));
}

void DataBaseTree::handleCreateDatabase()
{
    ServerAction::NewDatabaseWindow *newDatabaseWindow = new ServerAction::NewDatabaseWindow();

    connect(newDatabaseWindow, SIGNAL(createDatabase(QString, QString)), SLOT(createDatabase(QString, QString)));
    newDatabaseWindow->show();
}

void DataBaseTree::createDatabase(QString databaseName, QString collation)
{
    this->dataBaseModel->addDatabase(databaseName, collation);
    this->handleRefreshDatabase();
}

void DataBaseTree::handleShowProcesses()
{
	QStandardItem *serverItem = this->dataBaseModel->invisibleRootItem()->child(this->contextMenuIndex.row(), 0);
	QJsonObject sessionConf = serverItem->data().toJsonObject();

	qDebug() << "Open process list window";
	qDebug() << sessionConf;

	ServerAction::ShowProcessesWindow *showProcesses = new ServerAction::ShowProcessesWindow(sessionConf, this);
	showProcesses->show();
}

void DataBaseTree::handleRefreshDatabase()
{
	QModelIndex index = ((Model::TableFilterProxyModel *)this->model())->mapToSource(this->currentIndex());
	this->dataBaseModel->refresh(index);
}

void DataBaseTree::handleDrop()
{
	QModelIndex index = ((Model::TableFilterProxyModel *)this->model())->mapToSource(this->currentIndex());

	if (!index.isValid() || !index.parent().isValid()) {
		// Server node
		return;
	}
	else if (index.isValid() && index.parent().isValid() && !index.parent().parent().isValid()) {
		// Database node

		QStandardItem *dbItem  = this->dataBaseModel->invisibleRootItem()->child(index.parent().row())->child(index.row());
		// Drop database
		QMessageBox *confirm = new QMessageBox();
		confirm->setText(QString(tr("Drop database `%1` ?")).arg(dbItem->text()));
		confirm->setIcon(QMessageBox::Question);
		confirm->setWindowTitle("Confirm");
		confirm->setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
		int ret = confirm->exec();

		if (ret == QMessageBox::Yes){
			this->dataBaseModel->dropDatabase(index);
		}
	} else {
		// Table node
		QStandardItem *dbItem  = this->dataBaseModel->invisibleRootItem()->child(index.parent().parent().row())->child(index.parent().row());
		QStandardItem *tableItem = dbItem->child(index.row());

		// Drop table
		QMessageBox *confirm = new QMessageBox();
		confirm->setText(QString(tr("Drop table `%1` ?")).arg(tableItem->text()));
		confirm->setIcon(QMessageBox::Question);
		confirm->setWindowTitle("Confirm");
		confirm->setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
		int ret = confirm->exec();

		if (ret == QMessageBox::Yes){
			this->dataBaseModel->dropTable(index);
		}
	}

}

void DataBaseTree::handleTruncateTable()
{
	QModelIndex index = ((Model::TableFilterProxyModel *)this->model())->mapToSource(this->currentIndex());

	if (!index.isValid() || !index.parent().isValid() || !index.parent().parent().isValid()) {
		// Server node, database node
		return;
	}
	else {
		// Table node

		QStandardItem *dbItem  = this->dataBaseModel->invisibleRootItem()->child(index.parent().parent().row())->child(index.parent().row());
		QStandardItem *tableItem = dbItem->child(index.row());

		// Drop database
		QMessageBox *confirm = new QMessageBox();
		confirm->setText(QString(tr("Empty table `%1` ?")).arg(tableItem->text()));
		confirm->setIcon(QMessageBox::Question);
		confirm->setWindowTitle("Confirm");
		confirm->setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
		int ret = confirm->exec();

		if (ret == QMessageBox::Yes){
			this->dataBaseModel->truncateTable(index);
		}
	}
}

void DataBaseTree::queryError(QString error)
{
	QMessageBox *message = new QMessageBox();
	message->setWindowTitle(tr("Error"));
	message->setText(error);
	message->setIcon(QMessageBox::Critical);
	message->exec();
}

void DataBaseTree::handleDisconnect()
{
	QStandardItem *rootItem = this->dataBaseModel->invisibleRootItem();
	if (rootItem->rowCount() > 1) {
		QModelIndex serverIndex = this->currentIndex();
		rootItem->removeRow(serverIndex.row());
	} else {
		emit closeExplorer();
	}
}

DataBaseTree::~DataBaseTree() {

}

} /* namespace Explorer */
} /* namespace UI */
