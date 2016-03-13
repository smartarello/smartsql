/*
 * QueryTab.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Query/QueryTab.h>
#include <QHeaderView>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <qsqlerror.h>
#include <QMessageBox>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryTab::QueryTab(QWidget *parent) : QSplitter(parent) {

	this->setOrientation(Qt::Vertical);
	this->tableData = new QTableView(this);
	this->tableData->setSortingEnabled(true);
	this->tableData->verticalHeader()->hide();
	QSqlQueryModel *model = new QSqlQueryModel();
	this->tableData->setModel(model);

	this->queryTextEdit = new QueryTextEdit();

	this->addWidget(this->queryTextEdit);
	this->addWidget(this->tableData);

	this->setStretchFactor(0, 1);
	this->setStretchFactor(1, 4);

	connect(parent, SIGNAL (databaseChanged()), this->queryTextEdit, SLOT (databaseChanged()));
	connect(this->queryTextEdit, SIGNAL (queryChanged(QString)), this, SLOT (queryChanged(QString)));
}

void QueryTab::queryChanged(QString queryString)
{
	QSqlQueryModel *model = (QSqlQueryModel *) this->tableData->model();
	QSqlQuery query;
	query.exec(queryString);

	if (query.lastError().isValid()){
		qDebug() << query.lastError();
		QMessageBox *message = new QMessageBox(this);
		message->setText(query.lastError().databaseText());
		message->setIcon(QMessageBox::Critical);
		message->show();
		return ;
	}

	if (query.isSelect()){
		model->setQuery(query);
	}
}



QueryTab::~QueryTab() {

}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
