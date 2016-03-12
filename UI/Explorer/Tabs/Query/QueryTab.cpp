/*
 * QueryTab.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Query/QueryTab.h>
#include <QHeaderView>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryTab::QueryTab(QWidget *parent) : QSplitter(parent) {

	this->setOrientation(Qt::Vertical);
	this->tableData = new QTableView(this);
	this->tableData->setSortingEnabled(true);
	this->tableData->verticalHeader()->hide();

	this->queryTextEdit = new QueryTextEdit();

	this->addWidget(this->queryTextEdit);
	this->addWidget(this->tableData);

	this->setStretchFactor(0, 1);
	this->setStretchFactor(1, 4);
}

QueryTab::~QueryTab() {
	// TODO Auto-generated destructor stub
}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
