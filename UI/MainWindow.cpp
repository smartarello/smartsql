/*
 * MainWindow.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#include <UI/MainWindow.h>
#include "Session/SessionWindow.h"

namespace UI {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setWindowTitle("MySQL explorer");
		 // Set size of the window
	setFixedSize(800, 600);

	Session::SessionWindow *sessionList = new Session::SessionWindow(this);
	this->setCentralWidget(sessionList);
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

void MainWindow::handleNewConnection()
{
	this->editSession = new Session::NewSessionWindow(this);
	this->setCentralWidget(this->editSession);
}

void MainWindow::handleCancelNewConnection()
{
	Session::SessionWindow *sessionList = new Session::SessionWindow(this);
	this->setCentralWidget(sessionList);
}

void MainWindow::handleSaveConnection()
{
	qDebug() << this->editSession->getHost();
}

} /* namespace UI */
