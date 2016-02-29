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
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include "SessionWindow.h"

namespace UI {

namespace Session {

SessionWindow::SessionWindow(QWidget *parent) : QWidget(parent), currentItem(0){

	 QVBoxLayout *vlayout = new QVBoxLayout;
	 this->setLayout(vlayout);

	 QWidget *header = new QWidget;
	 vlayout->addWidget(header);

	 QHBoxLayout *hlayout = new QHBoxLayout;
	 header->setLayout(hlayout);

	 this->sessionList = new QListView;
	 this->sessionList->setFixedWidth(250);

	 // Create model
	 QStringListModel *model = new QStringListModel(this);

	 // Make data
	 QStringList sessionNames;
	 hlayout->addWidget(this->sessionList);

	 QSettings settings("smartarello", "mysqlclient");
	 QString sessions = settings.value("sessions").toString();
	 if (!sessions.isNull() && !sessions.isEmpty()){
		 QJsonDocument doc = QJsonDocument::fromJson(sessions.toUtf8());
		 this->sessionStore = doc.array();
	 }

	 for (int i = 0; i < this->sessionStore.count(); i++){
		 QJsonObject session = this->sessionStore.at(i).toObject();
		 sessionNames << session.value("name").toString();
	 }

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
	 connect(deleteButton, SIGNAL (released()), this, SLOT (handleDelete()));

	 connect(this->sessionList->selectionModel(),
	       SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
	       this, SLOT(handleSelectionChanged(QItemSelection)));
}


void SessionWindow::handleNewConnection()
{
	QJsonObject newSession;
	newSession.insert("name", "Unamed");
	newSession.insert("user", "");
	newSession.insert("password", "");
	newSession.insert("port", 3306);
	newSession.insert("hostname", "localhost");

	this->sessionStore.append(newSession);

	this->persistSessionStore();

	this->currentItem = this->sessionStore.count() - 1;
	this->updateSelected();
}

void SessionWindow::handleDelete()
{
	QMessageBox *confirm = new QMessageBox();
	confirm->setText("Delete session");
	confirm->setIcon(QMessageBox::Question);
	confirm->setWindowTitle("Confirm");
	confirm->setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
	int ret = confirm->exec();

	if (ret == QMessageBox::Yes){
		this->sessionStore.removeAt(this->currentItem);
		this->persistSessionStore();
	}
}

void SessionWindow::handleSelectionChanged(const QItemSelection& selection)
{
	if (selection.indexes().isEmpty()){
		return ;
	}

	QModelIndex m = selection.indexes().first();
	this->currentItem = m.row();

	this->updateSelected();

}

void SessionWindow::updateSelected()
{
	if (this->currentItem < this->sessionStore.count()){
		QJsonObject session = this->sessionStore.at(this->currentItem).toObject();

		this->editSession->setName(session.value("name").toString());
		this->editSession->setHostName(session.value("hostname").toString());
		this->editSession->setUser(session.value("user").toString());
		this->editSession->setPassword(session.value("password").toString());
		this->editSession->setPort(session.value("port").toInt());
	}
}

void SessionWindow::handleExit()
{
	QCoreApplication::quit();
}

void SessionWindow::handleSaveConnection()
{
	QSettings settings("smartarello", "mysqlclient");


	QJsonObject session = this->sessionStore.at(this->currentItem).toObject();

	session.insert("name", this->editSession->getName());
	session.insert("hostname", this->editSession->getHostName());
	session.insert("user", this->editSession->getUser());
	session.insert("password", this->editSession->getPassword());
	session.insert("port", this->editSession->getPort());

	this->sessionStore.replace(this->currentItem, session);

	this->persistSessionStore();
}

void SessionWindow::persistSessionStore()
{
	QSettings settings("smartarello", "mysqlclient");

	QJsonDocument doc;
	doc.setArray(this->sessionStore);
	QString strJson(doc.toJson(QJsonDocument::Compact));
	settings.setValue("sessions", strJson);
}

SessionWindow::~SessionWindow(){

}


}
}
