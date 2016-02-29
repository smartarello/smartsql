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
#include <QSettings>
#include <QLabel>
#include <QDebug>
#include <QListView>
#include <QStringListModel>
#include <QStringList>
#include <QCoreApplication>
#include "SessionWindow.h"

namespace UI {

namespace Session {

SessionWindow::SessionWindow(QWidget *parent) : QWidget(parent){

	 QVBoxLayout *vlayout = new QVBoxLayout;
	 this->setLayout(vlayout);

	 QWidget *header = new QWidget;
	 vlayout->addWidget(header);

	 QHBoxLayout *hlayout = new QHBoxLayout;
	 header->setLayout(hlayout);

	 this->sessionList = new QListView;
	 this->sessionList->setFixedWidth(200);

	 // Create model
	 QStringListModel *model = new QStringListModel(this);

	 // Make data
	 QStringList sessionNames;
	 hlayout->addWidget(this->sessionList);

	 QSettings settings("smartarello", "mysqlclient");
	 int size = settings.beginReadArray("sessions");

	 for (int i = 0; i < size; ++i) {
	     settings.setArrayIndex(i);
	     sessionNames << settings.value("name").toString();
	 }
	 settings.endArray();

	 // Populate our model
	 model->setStringList(sessionNames);

	 // Glue model and view together
	 this->sessionList->setModel(model);


	 this->editSession = new EditSessionWindow(this);
	 hlayout->addWidget(this->editSession);

	 // Footer part
	 QWidget *buttonContainer = new QWidget;
	 QBoxLayout *buttonLayout = new QBoxLayout(QBoxLayout::LeftToRight);

	 buttonContainer->setLayout(buttonLayout);

	 QPushButton *newConnection = new QPushButton("New connection");
	 newConnection->setFixedWidth(150);
	 buttonLayout->addWidget(newConnection);

	 QPushButton *saveButton = new QPushButton("Save");
	 QPushButton *exitButton = new QPushButton("Exit");
	 QPushButton *deleteButton = new QPushButton("Delete");
	 saveButton->setFixedWidth(150);
	 exitButton->setFixedWidth(150);
	 deleteButton->setFixedWidth(150);
	 buttonLayout->addWidget(saveButton);
	 buttonLayout->addWidget(deleteButton);
	 buttonLayout->addWidget(exitButton);

	 vlayout->addWidget(buttonContainer);

	 // Connect button signal to appropriate slot
	 connect(newConnection, SIGNAL (released()), this, SLOT (handleNewConnection()));
	 connect(exitButton, SIGNAL (released()), this, SLOT (handleExit()));
	 connect(saveButton, SIGNAL (released()), this, SLOT (handleSaveConnection()));

	 connect(this->sessionList->selectionModel(),
	       SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
	       this, SLOT(handleSelectionChanged(QItemSelection)));
}


void SessionWindow::handleNewConnection()
{
//	this->editSession = new Session::EditSessionWindow(this);
//	this->setCentralWidget(this->editSession);
}

void SessionWindow::handleSelectionChanged(const QItemSelection& selection)
{
	if (selection.indexes().isEmpty()){
		return ;
	}

	QModelIndex m = selection.indexes().first();
	int r = m.row();

	qDebug() << r;
//	this->editSession = new Session::EditSessionWindow(this);
//	this->setCentralWidget(this->editSession);
}

void SessionWindow::handleExit()
{
	QCoreApplication::quit();
}

void SessionWindow::handleSaveConnection()
{
	QSettings settings("smartarello", "mysqlclient");

	int size = settings.beginReadArray("sessions");
	settings.endArray();

	settings.beginWriteArray("sessions");
	settings.setArrayIndex(size);
	settings.setValue("name", this->editSession->getName());
	settings.setValue("hostname", this->editSession->getHostName());
	settings.setValue("user", this->editSession->getUser());
	settings.setValue("password", this->editSession->getPassword());
	settings.setValue("port", this->editSession->getPort());
	settings.endArray();

	qDebug() << this->editSession->getName();
}

SessionWindow::~SessionWindow(){

}


}
}
