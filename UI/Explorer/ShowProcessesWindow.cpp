/*
 * ShowProcessesWindow.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: stephane
 */

#include <UI/Explorer/ShowProcessesWindow.h>
#include <QVBoxLayout>
#include <QSqlQueryModel>
#include <QHeaderView>

namespace UI {
namespace Explorer {

ShowProcessesWindow::ShowProcessesWindow(QWidget * parent) : QMainWindow(parent) {

	this->setMinimumSize(900, 300);

	QWidget *container = new QWidget(this);
	QVBoxLayout *containerLayout = new QVBoxLayout(container);

	this->processListTable = new QTableView(this);
	this->processListTable->horizontalHeader()->stretchLastSection();

	this->processListTable->verticalHeader()->hide();
	QSqlQueryModel *model = new QSqlQueryModel();
	model->setQuery("SELECT * FROM `information_schema`.`PROCESSLIST`");
	this->processListTable->setModel(model);

	containerLayout->addWidget(this->processListTable);

	this->setCentralWidget(container);
}

ShowProcessesWindow::~ShowProcessesWindow() {
	// TODO Auto-generated destructor stub
}

} /* namespace Explorer */
} /* namespace UI */
