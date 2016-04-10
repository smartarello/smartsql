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

#include <UI/MainWindow.h>
#include <QSettings>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QApplication>



#include "Session/SessionWindow.h"
#include "Util/DataBase.h"

namespace UI {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), sessionManager(0), explorer(0) {

	// Init MySQL Driver
    this->setWindowTitle(tr("SmartSQL"));
	Session::SessionWindow *sessionList = new Session::SessionWindow(this);
	this->setCentralWidget(sessionList);

	connect(sessionList, SIGNAL(openConnection(QJsonObject)), this, SLOT(handleOpenConnection(QJsonObject)));
}

void MainWindow::handleOpenConnection(QJsonObject sessionConfiguration)
{
    if (Util::DataBase::open(sessionConfiguration)){

		if (this->explorer == 0) {

			qDebug() << "Open explorer";
			this->showMaximized();
            this->toolbar = new ToolBar(this);
            this->addToolBar(this->toolbar);

			this->explorer = new Explorer::Explorer(this, sessionConfiguration);
			this->setCentralWidget(explorer);

			connect(this->explorer, SIGNAL(closeExplorer()), SLOT(closeExplorer()));

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
		message->setText(tr("Unable to connect to the database"));
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

void MainWindow::closeExplorer()
{

	qDebug() << "Close Explorer";
    this->removeToolBar(this->toolbar);
	Session::SessionWindow *sessionList = new Session::SessionWindow(this);
	this->setCentralWidget(sessionList);
	this->showNormal();

    this->explorer = 0;

	connect(sessionList, SIGNAL(openConnection(QJsonObject)), this, SLOT(handleOpenConnection(QJsonObject)));
}

void MainWindow::exit()
{
	QApplication::quit();
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}
} /* namespace UI */
