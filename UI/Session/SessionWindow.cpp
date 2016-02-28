/*
 * SessionWindow.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QBoxLayout>
#include <QMainWindow>
#include "SessionWindow.h"

namespace UI {

namespace Session {

SessionWindow::SessionWindow(QWidget *parent) : QWidget(parent){

	 QVBoxLayout *vlayout = new QVBoxLayout;
	 this->setLayout(vlayout);

	 // Footer part
	 QWidget *buttonContainer = new QWidget;
	 QBoxLayout *buttonLayout = new QBoxLayout(QBoxLayout::LeftToRight);

	 buttonContainer->setLayout(buttonLayout);

	 QPushButton *newConnection = new QPushButton("New connection");
	 newConnection->setFixedWidth(150);
	 buttonLayout->addWidget(newConnection);

	 vlayout->addWidget(buttonContainer);

	 // Connect button signal to appropriate slot
	 connect(newConnection, SIGNAL (released()), this->parent(), SLOT (handleNewConnection()));
}

SessionWindow::~SessionWindow(){

}


}
}
