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
#include <UI/Explorer/Model/TableFilterProxyModel.h>

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
	connect(refreshShortcut, SIGNAL(activated()), SLOT(handleRefreshDatabase()));

	QShortcut* deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
	connect(deleteShortcut, SIGNAL(activated()), SLOT(handleDrop()));
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
		menu->addAction(showProcessesAction);

		QAction *createDatabaseAction = new QAction(tr("Create database"), this);
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
	}

	menu->popup(this->viewport()->mapToGlobal(point));
}

void DataBaseTree::handleShowProcesses()
{
	QStandardItem *serverItem = this->dataBaseModel->invisibleRootItem()->child(this->currentIndex().row(), 0);
	QJsonObject sessionConf = serverItem->data().toJsonObject();

	qDebug() << "Open process list window";
	qDebug() << sessionConf;

	ShowProcessesWindow *showProcesses = new ShowProcessesWindow(sessionConf, this);
	showProcesses->show();
}

void DataBaseTree::handleRefreshDatabase()
{
	QModelIndex index = this->currentIndex();
	this->dataBaseModel->refresh(index);
}

void DataBaseTree::handleDrop()
{
	if (this->currentIndex().isValid() && this->currentIndex().parent().isValid() && !this->currentIndex().parent().parent().isValid()) {

		// Drop database
		QMessageBox *confirm = new QMessageBox();
		confirm->setText("Drop database ?");
		confirm->setIcon(QMessageBox::Question);
		confirm->setWindowTitle("Confirm");
		confirm->setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
		int ret = confirm->exec();

		if (ret == QMessageBox::Yes){
			if (!this->dataBaseModel->dropDatabase(this->currentIndex())) {
				QMessageBox *message = new QMessageBox();
				message->setWindowTitle(tr("Error"));
				message->setText(tr("Unable to drop the database"));
				message->setIcon(QMessageBox::Critical);
				message->exec();
			}
		}
	}

}

DataBaseTree::~DataBaseTree() {

}

} /* namespace Explorer */
} /* namespace UI */
