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
#include <QToolButton>
#include <QSqlDatabase>
#include <QSplitter>
#include <QItemSelectionModel>
#include <QDebug>
#include <QKeySequence>
#include <QShortcut>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlError>
#include <QPixmap>
#include "Model/DataBaseModel.h"
#include "Tabs/Query/QueryTab.h"
#include "Tabs/TabView.h"
#include "../../Util/DataBase.h"


namespace UI {
namespace Explorer {

Explorer::Explorer(QWidget *parent, QJsonObject sessionConf) : QWidget(parent) {


	this->dataBaseTree = new DataBaseTree(this, sessionConf);

	QSplitter *splitter = new QSplitter(Qt::Horizontal);

	QHBoxLayout *hboxlayout = new QHBoxLayout;
	this->setLayout(hboxlayout);

	QWidget *leftPartWidget = new QWidget(this);
	QVBoxLayout *leftPartlayout = new QVBoxLayout;
	leftPartlayout->setContentsMargins(0, 0, 0, 0);
	leftPartWidget->setLayout(leftPartlayout);

	QWidget *databaseFilterContainer = new QWidget(this);
	QHBoxLayout *databaseFilterLayout = new QHBoxLayout();
	databaseFilterLayout->setContentsMargins(0, 0, 0, 0);
	databaseFilterContainer->setLayout(databaseFilterLayout);

	QLabel *databaseIconLabel = new QLabel();
	databaseIconLabel->setToolTip(tr("Database filter"));
	QPixmap iconDatabase(":/resources/icons/database-icon-24.png");
	databaseIconLabel->setPixmap(iconDatabase);

	databaseFilterLayout->addWidget(databaseIconLabel);
	this->databaseFilterLineEdit = new QLineEdit();
	this->databaseFilterLineEdit->setPlaceholderText(tr("Database filter"));
	databaseFilterLayout->addWidget(this->databaseFilterLineEdit);


	QWidget *tableFilterContainer = new QWidget(this);
	QHBoxLayout *tableFilterLayout = new QHBoxLayout();
	tableFilterLayout->setContentsMargins(0, 0, 0, 0);
	tableFilterContainer->setLayout(tableFilterLayout);

	QLabel *tableIconLabel = new QLabel();
	tableIconLabel->setToolTip(tr("Table filter"));
	QPixmap icon(":/resources/icons/database-table-icon-24.png");
	tableIconLabel->setPixmap(icon);

	tableFilterLayout->addWidget(tableIconLabel);
	this->tableFilterLineEdit = new QLineEdit();
	this->tableFilterLineEdit->setPlaceholderText(tr("Table filter"));
	tableFilterLayout->addWidget(this->tableFilterLineEdit);


	leftPartlayout->addWidget(databaseFilterContainer);
	leftPartlayout->addWidget(tableFilterContainer);
	leftPartlayout->addWidget(this->dataBaseTree);

	splitter->addWidget(leftPartWidget);

	this->explorerTabs = new Tabs::TabView(splitter);

	QShortcut * addTabShortCut = new QShortcut(QKeySequence("Ctrl+N"), this->explorerTabs);
	splitter->addWidget(this->explorerTabs);

	QSortFilterProxyModel *filterModel = (QSortFilterProxyModel *)this->dataBaseTree->model();
    QModelIndex	index = filterModel->index(0, 0);

	this->dataBaseTree->selectionModel()->setCurrentIndex(index, (QItemSelectionModel::Select | QItemSelectionModel::Rows));

	this->databaseTab = new Tabs::Database::DataBaseTab();
    this->serverTab = new ServerTab(this);
    this->explorerTabs->addTab(this->serverTab, tr("Host"));

	Tabs::Query::QueryTab *queryTab = new Tabs::Query::QueryTab(this);
	this->explorerTabs->addTab(queryTab, tr("Query"));

	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 3);
	hboxlayout->addWidget(splitter);

	this->tableTab = new Tabs::Table::TableTab(this);
	this->tableTab->hide();

	// The first tabs are not closable
	this->explorerTabs->tabBar()->tabButton(0, QTabBar::RightSide)->hide();
	this->explorerTabs->tabBar()->tabButton(1, QTabBar::RightSide)->hide();

	connect(this->tableFilterLineEdit, SIGNAL (textEdited(QString)), this->dataBaseTree, SLOT (filterTable(QString)));
	connect(this->databaseFilterLineEdit, SIGNAL (textEdited(QString)), this->dataBaseTree, SLOT (filterDatabase(QString)));

	connect(this->dataBaseTree, SIGNAL (closeExplorer()), this, SLOT (handleCloseExplorer()));
	connect(this->dataBaseTree, SIGNAL (doubleClicked(QModelIndex)), this, SLOT (dataBaseTreeDoubleClicked(QModelIndex)));
	connect(addTabShortCut, SIGNAL(activated()), this, SLOT(addQueryTab()));
	connect(this->explorerTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeQueryTab(int)));
	connect(this->explorerTabs->tabBar(), SIGNAL(newTabRequested()), this, SLOT(addQueryTab()));
    connect(this->dataBaseTree->getDataBaseModel(), SIGNAL(databaseChanged()), this, SLOT(refreshDatabase()));


    connect(this->dataBaseTree->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(dataBaseTreeItemChanged()));
}

void Explorer::addQueryTab(){
	Tabs::Query::QueryTab *queryTab = new Tabs::Query::QueryTab(this);
	this->explorerTabs->addTab(queryTab, tr("Query"));
	this->explorerTabs->setCurrentWidget(queryTab);
	queryTab->focus();
}

void Explorer::closeQueryTab(int index)
{
	if (index > 1){
		this->explorerTabs->removeTab(index);
	}
}

void Explorer::dataBaseTreeDoubleClicked(QModelIndex index)
{
	int tableTabIndex = this->explorerTabs->indexOf(this->tableTab) ;

	if (tableTabIndex != -1){
		QSortFilterProxyModel *model = (QSortFilterProxyModel *)this->dataBaseTree->model();
		index = model->mapToSource(index);

		// Click on a table
		if (index.parent().parent().isValid()){
			this->explorerTabs->setCurrentWidget(this->tableTab);
		}
	}
}

void Explorer::dataBaseTreeItemChanged()
{
	int tableTabIndex = this->explorerTabs->indexOf(this->tableTab) ;
    int dataBaseTabIndex = this->explorerTabs->indexOf(this->databaseTab) ;
    int serverTabIndex = this->explorerTabs->indexOf(this->serverTab) ;
    QModelIndex index = this->dataBaseTree->currentIndex();
	if (!index.parent().isValid()){
		// click on the server host
		if (tableTabIndex != -1){
			this->explorerTabs->removeTab(tableTabIndex);
		}

        if (dataBaseTabIndex != -1) {
            this->explorerTabs->removeTab(dataBaseTabIndex);
        }

        if (serverTabIndex == -1) {
            this->explorerTabs->insertTab(0, this->serverTab, tr("Host"));
             this->explorerTabs->tabBar()->tabButton(0, QTabBar::RightSide)->hide();
        }
		return ;
    } else if (serverTabIndex != -1) {
        this->explorerTabs->removeTab(serverTabIndex);
        this->explorerTabs->insertTab(0, this->databaseTab, "");
        this->explorerTabs->tabBar()->tabButton(0, QTabBar::RightSide)->hide();
    }

	QSortFilterProxyModel *model = (QSortFilterProxyModel *)this->dataBaseTree->model();
    index = model->mapToSource(index);
	QStandardItem *root = this->dataBaseTree->getDataBaseModel()->invisibleRootItem();


	QString dataBaseName;
	QString tableName;
	QStandardItem *dbItem ;

	QModelIndex dbIndex = index;
	if (index.parent().parent().isValid()){
		// click on a table
		dbIndex = index.parent();

		// Show table details
		dbItem = root->child(dbIndex.parent().row())->child(dbIndex.row());
		QStandardItem *tableItem = dbItem->child(index.row());

		dataBaseName = dbItem->text();
		tableName = tableItem->text();
	}
	else{
        // Click on a database
		dbItem = root->child(dbIndex.parent().row())->child(dbIndex.row());
		dataBaseName = dbItem->text();
	}

	QJsonObject sessionConf = dbItem->parent()->data().toJsonObject();
	if (!Util::DataBase::open(sessionConf, dataBaseName)){


		QMessageBox *message = new QMessageBox();
		message->setWindowTitle(tr("Connection error"));
		message->setText(tr("Unable to connect to the Data Base"));

		message->setIcon(QMessageBox::Critical);
		message->exec();

		return;
	}


	if (!tableName.isNull()){
		this->tableTab->setTable(tableName);
		if (this->explorerTabs->indexOf(this->tableTab) == -1){
			this->tableTab->show();
			this->explorerTabs->insertTab(1, this->tableTab, tr("Data"));
			this->explorerTabs->tabBar()->tabButton(1, QTabBar::RightSide)->hide();
		}
	}
	else if (tableTabIndex != -1){
		this->explorerTabs->removeTab(tableTabIndex);
	}

    this->databaseTab->refresh();
    this->explorerTabs->setTabText(0, QString(tr("Database: %1")).arg(dataBaseName));


	emit databaseChanged();
}

void Explorer::refreshDatabase()
{
    int tableTabIndex = this->explorerTabs->indexOf(this->tableTab) ;
    if (tableTabIndex != -1){
        this->explorerTabs->removeTab(tableTabIndex);
    }

	QSqlDatabase db = QSqlDatabase::database();
	this->databaseTab->refresh();
	this->explorerTabs->setTabText(0, QString(tr("Database: %1")).arg(db.databaseName()));

    this->explorerTabs->setCurrentWidget(this->databaseTab);
}

void Explorer::addDatabase(QJsonObject sessionConfiguration)
{
    this->dataBaseTree->getDataBaseModel()->addServer(sessionConfiguration);
}

void Explorer::handleCloseExplorer()
{
	// We are closing the last open connection, we have to close the explorer and show the session window
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isOpen()) {
		db.close();
	}

	emit closeExplorer();
}

Explorer::~Explorer() {

}

} /* namespace Explorer */
} /* namespace UI */
