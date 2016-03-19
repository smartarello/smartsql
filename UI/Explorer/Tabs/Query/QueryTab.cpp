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

	this->queryTextEdit = new QueryTextEdit();
	topLayout->addWidget(this->queryTextEdit);

	QPushButton *execButton = new QPushButton(tr("Execute"));
	execButton->setToolTip(tr("Execute query (F5)"));
	execButton->setFixedWidth(100);
	topLayout->addWidget(execButton);

	this->queryTabs = new QTabWidget(this);

	this->addWidget(topPart);
	this->addWidget(this->queryTabs);

	QList<int> sizes;
	sizes << 100;
	sizes << 200;
	this->setSizes(sizes);

	connect(parent, SIGNAL (databaseChanged()), this->queryTextEdit, SLOT (databaseChanged()));
	connect(this->queryTextEdit, SIGNAL (queryChanged()), this, SLOT (queryChanged()));
	connect(execButton, SIGNAL (clicked(bool)), this, SLOT (queryChanged()));
}

void QueryTab::queryChanged()
{
	this->queryTabs->clear();

	QStringList queries = this->queryTextEdit->toPlainText().split(";");

	foreach (QString sql, queries) {

		if (sql.trimmed().isEmpty()) {
			continue;
		}

		QSqlQuery query;
		query.exec(sql);

		if (query.lastError().isValid()) {
			qDebug() << query.lastError();
			QMessageBox *message = new QMessageBox(this);
			message->setText(query.lastError().databaseText());
			message->setIcon(QMessageBox::Critical);
			message->show();
			return ;
		}

		if (query.isSelect()) {
			QTableView *tableData = new QTableView();
			tableData->setSortingEnabled(true);
			tableData->verticalHeader()->hide();
			QueryModel *model = new QueryModel();

			tableData->setModel(model);

			tableData->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
			model->setQuery(query);

			int rows = query.numRowsAffected();
			this->queryTabs->addTab(tableData, QString(tr("Result (%1 rows)")).arg(rows));
		}
		else {
			QTextEdit *resultText = new QTextEdit();
			resultText->setReadOnly(true);

			int rows = query.numRowsAffected();
			resultText->setPlainText(QString(tr("Affected rows: %1")).arg(rows));
			this->queryTabs->addTab(resultText, QString(tr("Result (%1 rows)")).arg(rows));
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
