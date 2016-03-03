/*
 * MainWindow.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#include <UI/MainWindow.h>
#include <QSettings>
#include <QSqlDatabase>
#include <QJsonObject>
#include <QMessageBox>
#include <QSqlError>
#include "Session/SessionWindow.h"
#include "Explorer/Explorer.h"

namespace UI {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setWindowTitle("MySQL explorer");
		 // Set size of the window
	setFixedSize(800, 600);

	Session::SessionWindow *sessionList = new Session::SessionWindow(this);
	this->setCentralWidget(sessionList);
}

void MainWindow::handleOpenConnection()
{
	Session::SessionWindow * session = (Session::SessionWindow *)this->centralWidget();
	QJsonObject conf = session->getSelectedSession();

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(conf.value("hostname").toString());
	db.setUserName(conf.value("user").toString());
	db.setPassword(conf.value("password").toString());
	db.setPort(conf.value("port").toInt());

	if (db.open()){
		db.close();
		this->setFixedSize(1024, 768);
		Explorer::Explorer *explorer = new Explorer::Explorer(this);
		this->setCentralWidget(explorer);
	}
	else{
		QSqlError err = db.lastError();

		QMessageBox *message = new QMessageBox();
		message->setWindowTitle("Connection error");
		message->setText("Unable to connect to the Data Base");
		message->setDetailedText(err.text());
		message->setIcon(QMessageBox::Critical);

		db = QSqlDatabase();
		db.removeDatabase("");

		message->exec();
	}
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}
} /* namespace UI */
