/*
 * NewSessionWindow.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#include <UI/Session/EditSessionWindow.h>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QString>
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QMessageBox>
namespace UI {
namespace Session {

EditSessionWindow::EditSessionWindow(QWidget *parent) : QWidget(parent){

	QWidget *formWidget = new QWidget;

	this->nameLineEdit = new QLineEdit();
	this->hostLineEdit = new QLineEdit();
	this->userLineEdit = new QLineEdit();
	this->passwordLineEdit = new QLineEdit();
	this->portLineEdit = new QSpinBox();
	this->portLineEdit->setMinimum(0);
	this->portLineEdit->setMaximum(1000000);
	this->portLineEdit->setFixedWidth(100);
	// Asterisks will be shown when values are entered
	this->passwordLineEdit->setEchoMode(QLineEdit::Password);

	QFormLayout *formLayout = new QFormLayout;
	formLayout->addRow(tr("&Connection name:"), this->nameLineEdit);
	formLayout->addRow(tr("&Hostname / IP:"), this->hostLineEdit);
	formLayout->addRow(tr("&User:"), this->userLineEdit);
	formLayout->addRow(tr("&Password:"), this->passwordLineEdit);
	formLayout->addRow(tr("&Port:"), this->portLineEdit);

	formWidget->setFixedHeight(200);
	formWidget->setLayout(formLayout);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);
	layout->addWidget(formWidget);

	QWidget *buttonWidget = new QWidget;
	QHBoxLayout *hboxlayout = new QHBoxLayout;
	hboxlayout->setAlignment(Qt::AlignRight);

	this->saveButton = new QPushButton("Save");
	this->saveButton->setFixedWidth(100);
	this->saveButton->setDisabled(true);

	QPushButton *testButton = new QPushButton("Test connection");
	testButton->setFixedWidth(150);

	hboxlayout->addWidget(testButton);
	hboxlayout->addWidget(this->saveButton);

	buttonWidget->setLayout(hboxlayout);
	layout->addWidget(buttonWidget);

	this->setLayout(layout);

	connect(testButton, SIGNAL (released()), this, SLOT (testConnection()));
	connect(this->saveButton, SIGNAL (released()), this->parent(), SLOT (handleSaveConnection()));
	connect(this->nameLineEdit, SIGNAL (textEdited(QString)), this, SLOT (edited()));
	connect(this->hostLineEdit, SIGNAL (textEdited(QString)), this, SLOT (edited()));
	connect(this->userLineEdit, SIGNAL (textEdited(QString)), this, SLOT (edited()));
	connect(this->passwordLineEdit, SIGNAL (textEdited(QString)), this, SLOT (edited()));
	connect(this->portLineEdit, SIGNAL (valueChanged(int)), this, SLOT (edited()));
}



EditSessionWindow::~EditSessionWindow() {

}

void EditSessionWindow::testConnection()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "test");
	db.setHostName(this->getHostName());
	db.setUserName(this->getUser());
	db.setPassword(this->getPassword());
	db.setPort(this->getPort());

	QMessageBox *message = new QMessageBox();
	message->setWindowTitle("Connection test");

	if (db.open()){

		db.close();
		db = QSqlDatabase();
		db.removeDatabase("test");
		message->setText("Connected successfully");
		message->setIcon(QMessageBox::Information);
	}
	else{
		QSqlError err = db.lastError();
		message->setText("Connection error");
		message->setDetailedText(err.text());
		message->setIcon(QMessageBox::Warning);

		db = QSqlDatabase();
		db.removeDatabase("test");
	}

	message->exec();

}

void EditSessionWindow::edited()
{
	this->saveButton->setDisabled(false);
}

QString EditSessionWindow::getHostName()
{
	return this->hostLineEdit->text();
}

QString EditSessionWindow::getName()
{
	return this->nameLineEdit->text();
}

QString EditSessionWindow::getUser()
{
	return this->userLineEdit->text();
}

QString EditSessionWindow::getPassword()
{
	return this->passwordLineEdit->text();
}

int EditSessionWindow::getPort()
{
	return this->portLineEdit->value();
}

void EditSessionWindow::setName(QString name)
{
	this->nameLineEdit->setText(name);
}
void EditSessionWindow::setHostName(QString hostname)
{
	this->hostLineEdit->setText(hostname);
}
void EditSessionWindow::setUser(QString user)
{
	this->userLineEdit->setText(user);
}
void EditSessionWindow::setPassword(QString password)
{
	this->passwordLineEdit->setText(password);
}
void EditSessionWindow::setPort(int port)
{
	this->portLineEdit->setValue(port);
}

} /* namespace Session */
} /* namespace UI */
