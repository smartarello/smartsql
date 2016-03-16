/*
 * TableTab.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Table/TableTab.h>
#include <QDebug>
#include <QSqlTableModel>
#include <QStringListModel>
#include <QStringList>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QHeaderView>
#include <QCompleter>
#include "TableModel.h"

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Table {

TableTab::TableTab(QWidget *parent) : QSplitter(parent) {

	this->setOrientation(Qt::Vertical);
	this->tableData = new QTableView();
	this->tableData->setSortingEnabled(true);
	this->tableData->verticalHeader()->hide();
	this->tableData->setEditTriggers(QAbstractItemView::DoubleClicked);

	this->whereConditionText = new TableFilterTextEdit();

	this->addWidget(this->tableData);

	QWidget *bottomPart = new QWidget(this);
	QVBoxLayout *bottomPartLayout = new QVBoxLayout();
	bottomPart->setLayout(bottomPartLayout);

	QLabel *filterLabel = new QLabel(this);
	filterLabel->setText(tr("Filter:"));

	bottomPartLayout->addWidget(filterLabel);
	bottomPartLayout->addWidget(this->whereConditionText);

	this->addWidget(bottomPart);

	this->setStretchFactor(0, 4);
	this->setStretchFactor(1, 1);

	TableModel *queryModel = new TableModel();
	this->tableData->setModel(queryModel);

	connect(this->whereConditionText, SIGNAL(filterChanged(QString)), queryModel, SLOT(refreshWithFilter(QString)));
	connect(queryModel, SIGNAL(queryError(QString, QString)), this, SLOT(queryError(QString, QString)));
}

void TableTab::setTable(QString tableName) {

	TableModel *queryModel = (TableModel *)this->tableData->model();
	queryModel->setTable(tableName);

	QCompleter *completer = this->whereConditionText->getAutocomplete();

	QStringListModel *model =  new QStringListModel(QStringList(queryModel->getColumns()), completer);
	completer->setModel(model);
}

void TableTab::queryError(QString query, QString error)
{
	QMessageBox *message = new QMessageBox(this);
	message->setText(error);
	message->setIcon(QMessageBox::Critical);
	message->setDetailedText(query);
	message->show();
}

TableTab::~TableTab() {

}

} /* namespace Table */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
