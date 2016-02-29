/*
 * MainWindow.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#include <UI/MainWindow.h>
#include <QSettings>
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
} /* namespace UI */
