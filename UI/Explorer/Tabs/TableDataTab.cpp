/*
 * TableDataTab.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/TableDataTab.h>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QHeaderView>

namespace UI {
namespace Explorer {
namespace Tabs {

TableDataTab::TableDataTab(QWidget *parent) : QSplitter(parent) {

	this->setOrientation(Qt::Vertical);
	this->tableData = new QTableView();
	this->tableData->setSortingEnabled(true);
	this->tableData->verticalHeader()->hide();
	this->whereConditionText = new QTextEdit();

	this->addWidget(this->tableData);
	this->addWidget(this->whereConditionText);

	this->setStretchFactor(0, 4);
	this->setStretchFactor(1, 1);
}

void TableDataTab::setTable(QString tableName, QSqlDatabase db) {

	if (db.open()){
		QSqlQueryModel *queryModel = new QSqlQueryModel();
		queryModel->setQuery(QString("SELECT * FROM %1 LIMIT 1000").arg(tableName));
		this->tableData->setModel(queryModel);

		db.close();
	}
}

TableDataTab::~TableDataTab() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
