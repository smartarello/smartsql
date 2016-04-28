/**
 * Copyright (C) 2016  Stéphane Martarello
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

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
#include <QUuid>
#include <QLocale>
#include <QJsonObject>
#include <QSqlRecord>
#include "QueryModel.h"
#include "ResultTableView.h"

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryTab::QueryTab(QWidget *parent) : QSplitter(parent) {

    // Defines a new type for the SIGNAL
    qRegisterMetaType< QList<QueryExecutionResult> >("QList<QueryExecutionResult>");

    // Horizontal split
	this->setOrientation(Qt::Vertical);

	QWidget *topPart = new QWidget(this);

    QVBoxLayout *topLayout = new QVBoxLayout(topPart);
	topPart->setLayout(topLayout);

    // Query editor
    this->queryTextEdit = new QueryTextEdit(this);
    topLayout->addWidget(this->queryTextEdit);

    // Actions
	QWidget *buttonContainer = new QWidget(this);
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonContainer->setLayout(buttonLayout);
	buttonLayout->setAlignment(Qt::AlignLeft);	

    this->executeButton = new QPushButton(this);
	this->executeButton->setIcon(QIcon(":/resources/icons/play.png"));
	this->executeButton->setIconSize(QSize(25, 25));
	this->executeButton->setToolTip(tr("Execute query (F5)"));
	this->executeButton->setFixedWidth(50);
	this->executeButton->setFixedHeight(30);
	buttonLayout->addWidget(this->executeButton);

    this->stopButton = new QPushButton(this);
	this->stopButton->setIcon(QIcon(":/resources/icons/stop.png"));
	this->stopButton->setIconSize(QSize(25, 25));
	this->stopButton->setToolTip(tr("Cancel running queries"));
	this->stopButton->setFixedWidth(50);
	this->stopButton->setFixedHeight(30);
	this->stopButton->setEnabled(false);
	buttonLayout->addWidget(this->stopButton);

	topLayout->addWidget(buttonContainer);

    // Tabs container to display the query results
    // Several queries can be played with the same editor
	this->queryTabs = new QTabWidget(this);

	this->addWidget(topPart);
	this->addWidget(this->queryTabs);

    // Proportion between the query editor and the tabs for the results
	QList<int> sizes;
	sizes << 80;
	sizes << 300;
	this->setSizes(sizes);

    // Events
    connect(this->parent(), SIGNAL (databaseChanged()), this->queryTextEdit, SLOT (databaseChanged()));
	connect(this->queryTextEdit, SIGNAL (queryChanged()), this, SLOT (queryChanged()));
	connect(this->executeButton, SIGNAL (clicked(bool)), this, SLOT (queryChanged()));
	connect(this->stopButton, SIGNAL (clicked(bool)), this, SLOT (stopQueries()));
}

void QueryTab::stopQueries()
{
    this->queryWorker->killQuery();
	this->executeButton->setEnabled(true);
	this->stopButton->setEnabled(false);
}

void QueryTab::queryChanged()
{
    QString query = this->queryTextEdit->toPlainText();

    if (!query.trimmed().isEmpty()) {

		this->executeButton->setEnabled(false);
		this->stopButton->setEnabled(true);

        // Creates the thread that will play the queries
        this->queryWorker = new QueryThread(Util::DataBase::dumpConfiguration(), query, this);
        // Event fire when the execution is terminated
        connect(this->queryWorker, SIGNAL(queryResultReady(QList<QueryExecutionResult>)), this, SLOT(handleQueryResultReady(QList<QueryExecutionResult>)));

        this->queryWorker->start();
	}

}

void QueryTab::handleQueryResultReady(QList<QueryExecutionResult> results)
{
    delete  this->queryWorker;

	this->executeButton->setEnabled(true);
    this->stopButton->setEnabled(false);

	this->queryTabs->clear();
    foreach(QueryExecutionResult result, results) {


        if (!result.error.isEmpty()) {
			QMessageBox *message = new QMessageBox(this);
            message->setText(result.error);
			message->setIcon(QMessageBox::Critical);
			message->show();
			return ;
		}

        double seconds = result.msec / 1000.0;

        if (result.isSelect) {
            ResultTableView *tableData = new ResultTableView(this->queryTabs);
            tableData->verticalHeader()->hide();

            QueryModel *model = new QueryModel(result.data, this);

            tableData->setModel(model);

            QString rowCount = QLocale(QLocale::English).toString(result.rows);

            QString headerText = QString(tr("Result (%1 rows, %2 sec)")).arg(rowCount).arg(seconds);
            if (result.limitedResult) {
                headerText += " " + tr("Limited to 1,000");
            }

            this->queryTabs->addTab(tableData, headerText);

            // Defines the initial column width
            int colCount = model->columnCount();
            for (int i = 0; i < colCount; i++) {
                QVariant header = model->headerData(i, Qt::Horizontal, Qt::DisplayRole);
                int size = header.toString().size() * 15;
                if (size < 100) {
                    size = 100;
                }
                tableData->setColumnWidth(i, size);
            }
		}
		else {
			QTextEdit *resultText = new QTextEdit();
            resultText->setFontFamily("DejaVue Sans Mono Oblique");
			resultText->setReadOnly(true);

            QString rowCount = QLocale(QLocale::English).toString(result.affectedRows);
            QString headerText = QString(tr("Result (%1 rows, %2 sec)")).arg(rowCount).arg(seconds);
            QString affectedRows = QString(tr("Affected rows: %1")).arg(rowCount);

            resultText->setPlainText(affectedRows + "\n\n" + result.query);
			this->queryTabs->addTab(resultText, headerText);
		}
	}


    results.clear();

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
