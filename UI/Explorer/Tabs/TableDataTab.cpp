/*
 * TableDataTab.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/TableDataTab.h>
#include <QDebug>
#include <QSqlTableModel>
#include <QStringListModel>
#include <QStringList>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QCompleter>
#include "Model/TableDataTabModel.h"

namespace UI {
namespace Explorer {
namespace Tabs {

TableDataTab::TableDataTab(QWidget *parent) : QSplitter(parent) {

	this->setOrientation(Qt::Vertical);
	this->tableData = new QTableView();
	this->tableData->setSortingEnabled(true);
	this->tableData->verticalHeader()->hide();

	this->whereConditionText = new TableFilterTextEdit();

	this->addWidget(this->tableData);

	QWidget *bottomPart = new QWidget(this);
	QVBoxLayout *bottomPartLayout = new QVBoxLayout();
	bottomPart->setLayout(bottomPartLayout);

	QLabel *filterLabel = new QLabel();
	filterLabel->setText(tr("Filter:"));

	bottomPartLayout->addWidget(filterLabel);
	bottomPartLayout->addWidget(this->whereConditionText);

	this->addWidget(bottomPart);

	this->setStretchFactor(0, 4);
	this->setStretchFactor(1, 1);
}

void TableDataTab::setTable(QString tableName) {

	Model::TableDataTabModel *queryModel = new Model::TableDataTabModel();
	queryModel->setTable(tableName);
	this->tableData->setModel(queryModel);
	QCompleter *completer = this->whereConditionText->getAutocomplete();

	QStringListModel *model =  new QStringListModel(QStringList(queryModel->getColumns()), completer);
	completer->setModel(model);
}

TableDataTab::~TableDataTab() {

}

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
