/*
 * DataBaseTab.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/DataBaseTab.h>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QHeaderView>

namespace UI {
namespace Explorer {
namespace Tabs {

DataBaseTab::DataBaseTab(QSqlDatabase db, QWidget * parent) : QTableView(parent) {

	if (db.open()){
		QSqlQueryModel *model = new QSqlQueryModel();
		model->setQuery("SHOW TABLE STATUS", db);

		this->setModel(model);
		db.close();
	}

	this->resizeColumnsToContents();
	this->verticalHeader()->hide();
}

void DataBaseTab::setSqlDatabase(QSqlDatabase db)
{
	if (db.open()){
		QSqlQueryModel *model = new QSqlQueryModel();
		model->setQuery("SHOW TABLE STATUS", db);

		this->setModel(model);
		db.close();
	}
}

DataBaseTab::~DataBaseTab() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
