/*
 * ShowProcessesWindow.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: stephane
 */

#include <UI/Explorer/ServerAction/ShowProcessesWindow.h>
#include <QVBoxLayout>
#include <QUuid>
#include <QSqlQueryModel>
#include <QHeaderView>

namespace UI {
namespace Explorer {
namespace ServerAction {

ShowProcessesWindow::ShowProcessesWindow(QJsonObject sessionConf, QWidget * parent) : QMainWindow(parent) {

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());

	db.setHostName(sessionConf.value("hostname").toString());
	db.setUserName(sessionConf.value("user").toString());
	db.setPassword(sessionConf.value("password").toString());
	db.setPort(sessionConf.value("port").toInt());

	this->setMinimumSize(900, 300);

	QWidget *container = new QWidget(this);
	QVBoxLayout *containerLayout = new QVBoxLayout(container);

	this->processListTable = new QTableView(this);
	this->processListTable->horizontalHeader()->stretchLastSection();

	this->processListTable->verticalHeader()->hide();
	QSqlQueryModel *model = new QSqlQueryModel();
	this->processListTable->setModel(model);

	containerLayout->addWidget(this->processListTable);

	this->setCentralWidget(container);

	this->worker = new ProcessListThread(db);
	connect(this->worker, SIGNAL(processListReady()), SLOT(processListReady()));
	this->worker->start();
}

void ShowProcessesWindow::processListReady()
{
	((QSqlQueryModel *)this->processListTable->model())->setQuery(this->worker->getQuery());
}

ShowProcessesWindow::~ShowProcessesWindow() {
	// TODO Auto-generated destructor stub
}

} /* namespace ServerAction */
} /* namespace Explorer */
} /* namespace UI */
