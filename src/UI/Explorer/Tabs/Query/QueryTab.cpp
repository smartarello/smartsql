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
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <qsqlerror.h>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QUuid>
#include "QueryModel.h"

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryTab::QueryTab(QWidget *parent) : QSplitter(parent) {

	this->setOrientation(Qt::Vertical);


	QWidget *topPart = new QWidget(this);

	QVBoxLayout *topLayout = new QVBoxLayout();
	topPart->setLayout(topLayout);

	QWidget *buttonContainer = new QWidget(this);
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonContainer->setLayout(buttonLayout);
	buttonLayout->setAlignment(Qt::AlignLeft);

	this->queryTextEdit = new QueryTextEdit();
	topLayout->addWidget(this->queryTextEdit);

	this->executeButton = new QPushButton();
	this->executeButton->setIcon(QIcon(":/resources/icons/play.png"));
	this->executeButton->setIconSize(QSize(25, 25));
	this->executeButton->setToolTip(tr("Execute query (F5)"));
	this->executeButton->setFixedWidth(50);
	this->executeButton->setFixedHeight(30);
	buttonLayout->addWidget(this->executeButton);

	this->stopButton = new QPushButton();
	this->stopButton->setIcon(QIcon(":/resources/icons/stop.png"));
	this->stopButton->setIconSize(QSize(25, 25));
	this->stopButton->setToolTip(tr("Cancel running queries"));
	this->stopButton->setFixedWidth(50);
	this->stopButton->setFixedHeight(30);
	this->stopButton->setEnabled(false);
	buttonLayout->addWidget(this->stopButton);

	topLayout->addWidget(buttonContainer);

	this->queryTabs = new QTabWidget(this);

	this->addWidget(topPart);
	this->addWidget(this->queryTabs);

	QList<int> sizes;
	sizes << 80;
	sizes << 300;
	this->setSizes(sizes);

	connect(parent, SIGNAL (databaseChanged()), this->queryTextEdit, SLOT (databaseChanged()));
	connect(this->queryTextEdit, SIGNAL (queryChanged()), this, SLOT (queryChanged()));
	connect(this->executeButton, SIGNAL (clicked(bool)), this, SLOT (queryChanged()));
	connect(this->stopButton, SIGNAL (clicked(bool)), this, SLOT (stopQueries()));
}

void QueryTab::stopQueries()
{
	this->queryWorker->terminate();
	this->queryWorker->deleteLater();
	this->executeButton->setEnabled(true);
	this->stopButton->setEnabled(false);
}

void QueryTab::queryChanged()
{
	QStringList queries = this->queryTextEdit->toPlainText().split(";");
	QStringList filteredQueries;

	foreach (QString sql, queries) {

		if (sql.trimmed().isEmpty()) {
			continue;
		}

		filteredQueries << sql;
	}

	if (!filteredQueries.isEmpty()) {
		this->executeButton->setEnabled(false);
		this->stopButton->setEnabled(true);
		QSqlDatabase db = QSqlDatabase::database();
		QSqlDatabase threadDB = QSqlDatabase::cloneDatabase(db, QUuid::createUuid().toString());
		this->queryWorker = new QueryThread(threadDB, filteredQueries);
		connect(this->queryWorker, SIGNAL(queryResultReady()), this, SLOT(handleQueryResultReady()));
		this->queryWorker->start();
	}

}

void QueryTab::handleQueryResultReady()
{
	qDebug() << "Queries result ready";

	this->executeButton->setEnabled(true);
	this->stopButton->setEnabled(false);
	QList<QueryExecutionResult> results = this->queryWorker->getQueryResult();


	this->queryTabs->clear();
	foreach(QueryExecutionResult result, results) {

		double seconds = result.msec / 1000.0;
		if (result.query.lastError().isValid()) {
			QMessageBox *message = new QMessageBox(this);
			message->setText(result.query.lastError().databaseText());
			message->setIcon(QMessageBox::Critical);
			message->show();
			return ;
		}

		if (result.query.isSelect()) {
			QTableView *tableData = new QTableView();
			tableData->verticalHeader()->hide();
			QueryModel *model = new QueryModel();

			tableData->setModel(model);

			model->setQuery(result.query);
			QString headerText = QString(tr("Result (%1 rows, %2 sec)")).arg(result.query.size()).arg(seconds);
			this->queryTabs->addTab(tableData, headerText);
		}
		else {
			QTextEdit *resultText = new QTextEdit();
			QFont font = QFont("Courier");
			font.setPointSize(12);
			font.setItalic(true);
			resultText->setFont(font);
			resultText->setReadOnly(true);


			QString headerText = QString(tr("Result (%1 rows, %2 sec)")).arg(result.query.numRowsAffected()).arg(seconds);
			QString affectedRows = QString(tr("Affected rows: %1")).arg(result.query.numRowsAffected());

			resultText->setPlainText(affectedRows + "\n\n" + result.query.lastQuery());
			this->queryTabs->addTab(resultText, headerText);
		}
	}
}

void QueryTab::focus()
{
	this->queryTextEdit->setFocus();
}



QueryTab::~QueryTab() {

}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
