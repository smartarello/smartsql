/*
 * Explorer.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Explorer.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QSqlDatabase>
#include <QSplitter>
#include <QDebug>
#include <QSqlError>
#include "Model/DataBaseModel.h"

namespace UI {
namespace Explorer {

Explorer::Explorer(QWidget *parent) : QWidget(parent) {

	this->dataBaseTree = new DataBaseTree(this);

	QSplitter *splitter = new QSplitter(Qt::Horizontal);

	QHBoxLayout *hboxlayout = new QHBoxLayout;
	this->setLayout(hboxlayout);

	QWidget *leftPartWidget = new QWidget;
	QVBoxLayout *leftPartlayout = new QVBoxLayout;
	leftPartWidget->setLayout(leftPartlayout);

	this->tableFilterLineEdit = new QLineEdit();
	leftPartlayout->addWidget(this->tableFilterLineEdit);
	leftPartlayout->addWidget(this->dataBaseTree);

	splitter->addWidget(leftPartWidget);

	this->explorerTabs = new QTabWidget(this);
	splitter->addWidget(this->explorerTabs);



	Model::DataBaseModel *model = this->dataBaseTree->getDataBaseModel();
	QStandardItem *rootItem = model->invisibleRootItem();
	QStandardItem *firstDataBase = rootItem->child(0)->child(0);

	QSqlDatabase db = QSqlDatabase::database();
	db.setDatabaseName(firstDataBase->text());

	this->databaseTab = new Tabs::DataBaseTab(db);
	this->explorerTabs->addTab(this->databaseTab, "Database");

	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 3);
	hboxlayout->addWidget(splitter);

	this->tableTab = new Tabs::TableDataTab();

	connect(this->tableFilterLineEdit, SIGNAL (textEdited(QString)), this->dataBaseTree, SLOT (filterTable(QString)));
	connect(this->dataBaseTree, SIGNAL (clicked(QModelIndex)), this, SLOT (dataBaseTreeClicked(QModelIndex)));
}

void Explorer::dataBaseTreeClicked(QModelIndex index)
{
	if (!index.parent().isValid()){
		// click on the server host
		// nothing todo
		return ;
	}

	QSortFilterProxyModel *model = (QSortFilterProxyModel *)this->dataBaseTree->model();
	index = model->mapToSource(index);
	QStandardItem *root = this->dataBaseTree->getDataBaseModel()->invisibleRootItem();

	QModelIndex dbIndex = index;
	if (index.parent().parent().isValid()){
		// click on a table
		dbIndex = index.parent();

		// Show table details
		QStandardItem *dbItem = root->child(0)->child(dbIndex.row());
		QStandardItem *tableItem = dbItem->child(index.row());

		QSqlDatabase db = QSqlDatabase::database();
		db.setDatabaseName(dbItem->text());

		this->tableTab->setTable(tableItem->text(), db);
		this->explorerTabs->insertTab(1, this->tableTab, tableItem->text());
		this->explorerTabs->setCurrentIndex(1);
		this->databaseTab->setSqlDatabase(db);
	}
	else{
		QStandardItem *item = root->child(0)->child(dbIndex.row());

		QSqlDatabase db = QSqlDatabase::database();
		db.setDatabaseName(item->text());
		this->databaseTab->setSqlDatabase(db);
	}
}

Explorer::~Explorer() {

}

} /* namespace Explorer */
} /* namespace UI */
