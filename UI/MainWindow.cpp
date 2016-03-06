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

	this->setWindowTitle(tr("MySQL explorer"));
	Session::SessionWindow *sessionList = new Session::SessionWindow(this);
	this->setCentralWidget(sessionList);
}

void MainWindow::handleOpenConnection()
{
	qInfo() << "Opening connection...";
	Session::SessionWindow * session = (Session::SessionWindow *)this->centralWidget();
	QJsonObject conf = session->getSelectedSession();

	qDebug() << conf;

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(conf.value("hostname").toString());
	db.setUserName(conf.value("user").toString());
	db.setPassword(conf.value("password").toString());
	db.setPort(conf.value("port").toInt());

	if (db.open()){
		db.close();
		this->showMaximized();
		Explorer::Explorer *explorer = new Explorer::Explorer(this, conf);
		this->setCentralWidget(explorer);
	}
	else{
		qInfo() << "Connection failed";

		QSqlError err = db.lastError();

		QMessageBox *message = new QMessageBox();
		message->setWindowTitle(tr("Connection error"));
		message->setText(tr("Unable to connect to the Data Base"));
		message->setDetailedText(err.text());
		message->setIcon(QMessageBox::Critical);

		qDebug() << err.text();

		db = QSqlDatabase();
		db.removeDatabase("");

		message->exec();
	}
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}
} /* namespace UI */
