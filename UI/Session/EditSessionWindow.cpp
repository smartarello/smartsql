/*
 * NewSessionWindow.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#include <UI/Session/EditSessionWindow.h>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
namespace UI {
namespace Session {

EditSessionWindow::EditSessionWindow(QWidget *parent) : QWidget(parent){

	QWidget *formWidget = new QWidget;

	this->nameLineEdit = new QLineEdit();
	this->hostLineEdit = new QLineEdit();
	this->userLineEdit = new QLineEdit();
	this->passwordLineEdit = new QLineEdit();
	this->portLineEdit = new QLineEdit();
	// Asterisks will be shown when values are entered
	this->passwordLineEdit->setEchoMode(QLineEdit::Password);


	QFormLayout *formLayout = new QFormLayout;
	formLayout->addRow(tr("&Connection name:"), this->nameLineEdit);
	formLayout->addRow(tr("&Hostname / IP:"), this->hostLineEdit);
	formLayout->addRow(tr("&User:"), this->userLineEdit);
	formLayout->addRow(tr("&Password:"), this->passwordLineEdit);
	formLayout->addRow(tr("&Port:"), this->portLineEdit);
	formWidget->setLayout(formLayout);



	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(formWidget);

	this->setLayout(layout);
}

EditSessionWindow::~EditSessionWindow() {
	// TODO Auto-generated destructor stub
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

QString EditSessionWindow::getPort()
{
	return this->portLineEdit->text();
}

} /* namespace Session */
} /* namespace UI */
