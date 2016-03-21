/*
 * MainWindow.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#include <UI/MainWindow.h>
#include <QSettings>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QApplication>



#include "Session/SessionWindow.h"
#include "ToolBar.h"
#include "Util/DataBase.h"

namespace UI {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), sessionManager(0), explorer(0) {

	// Init MySQL Driver
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	this->setWindowTitle(tr("MySQL explorer"));
	Session::SessionWindow *sessionList = new Session::SessionWindow(this);
	this->setCentralWidget(sessionList);

	connect(sessionList, SIGNAL(openConnection(QJsonObject)), this, SLOT(handleOpenConnection(QJsonObject)));
}

void MainWindow::handleOpenConnection(QJsonObject sessionConfiguration)
{
	qInfo() << "Opening connection...";

	qDebug() << sessionConfiguration;

	if (Util::DataBase::open(sessionConfiguration)){
		if (this->explorer == 0) {

			qDebug() << "Open explorer";
			this->showMaximized();
			this->addToolBar(new ToolBar(this));

			this->explorer = new Explorer::Explorer(this, sessionConfiguration);
			this->setCentralWidget(explorer);
		} else {

			if (this->sessionManager != 0) {
				qDebug() << "Close session manager";
				this->sessionManager->close();
				this->sessionManager = 0;
			}

			this->explorer->addDatabase(sessionConfiguration);
		}
	}
	else{
		qInfo() << "Connection failed";
		QSqlDatabase db = QSqlDatabase::database();
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

void MainWindow::openSessionManager()
{
	qDebug() << "Open session manager";
	if (this->sessionManager == 0) {
		this->sessionManager = new QMainWindow(this);
		Session::SessionWindow *sessionList = new Session::SessionWindow(this->sessionManager);
		sessionList->disableExitButton();
		this->sessionManager->setCentralWidget(sessionList);

		connect(sessionList, SIGNAL(openConnection(QJsonObject)), this, SLOT(handleOpenConnection(QJsonObject)));
	}

	this->sessionManager->show();

}

void MainWindow::exit()
{
	QApplication::quit();
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}
} /* namespace UI */
