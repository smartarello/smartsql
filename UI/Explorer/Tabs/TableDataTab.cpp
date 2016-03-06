/*
 * TableDataTab.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/TableDataTab.h>
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QHeaderView>
#include "Model/TableDataTabModel.h"

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

void TableDataTab::setTable(QString tableName) {

	Model::TableDataTabModel *queryModel = new Model::TableDataTabModel();
	queryModel->setTable(tableName);
	queryModel->select();
	this->tableData->setModel(queryModel);

}

TableDataTab::~TableDataTab() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
