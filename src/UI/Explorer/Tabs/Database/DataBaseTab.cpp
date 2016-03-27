/*
 * DataBaseTab.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Database/DataBaseTab.h>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
#include <QHeaderView>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Database {

DataBaseTab::DataBaseTab(QWidget * parent) : QTableView(parent) {

	QSqlQueryModel *model = new QSqlQueryModel();
	model->setQuery("SHOW TABLE STATUS");

	this->setModel(model);
	this->resizeColumnsToContents();
	this->verticalHeader()->hide();
}

void DataBaseTab::refresh()
{
	QSqlQueryModel *model = new QSqlQueryModel();
	model->setQuery("SHOW TABLE STATUS");

	this->setModel(model);
}

DataBaseTab::~DataBaseTab() {
	// TODO Auto-generated destructor stub
}

} /* namespace Database */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
