/*
 * NewSessionWindow.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#include <UI/Session/NewSessionWindow.h>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
namespace UI {
namespace Session {

NewSessionWindow::NewSessionWindow(QWidget *parent) : QWidget(parent){


	QWidget *formWidget = new QWidget;

	this->nameLineEdit = new QLineEdit();
	this->hostLineEdit = new QLineEdit();
	this->userLineEdit = new QLineEdit();
	this->password = new QLineEdit();
	this->portLineEdit = new QLineEdit();
	// Asterisks will be shown when values are entered
	this->password->setEchoMode(QLineEdit::Password);


	QFormLayout *formLayout = new QFormLayout;
	formLayout->addRow(tr("&Connection name:"), this->nameLineEdit);
	formLayout->addRow(tr("&Hostname / IP:"), this->hostLineEdit);
	formLayout->addRow(tr("&User:"), this->userLineEdit);
	formLayout->addRow(tr("&Password:"), this->password);
	formLayout->addRow(tr("&Port:"), this->portLineEdit);
	formWidget->setLayout(formLayout);


	QWidget *footerWidget = new QWidget;
	QHBoxLayout *$boxLayout = new QHBoxLayout;


	footerWidget->setLayout($boxLayout);

	QPushButton *saveButton = new QPushButton("Save");
	QPushButton *cancelButton = new QPushButton("Cancel");
	saveButton->setFixedWidth(150);
	cancelButton->setFixedWidth(150);
	$boxLayout->addWidget(saveButton);
	$boxLayout->addWidget(cancelButton);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(formWidget);
	layout->addWidget(footerWidget);

	this->setLayout(layout);

	connect(cancelButton, SIGNAL (released()), this->parent(), SLOT (handleCancelNewConnection()));
	connect(saveButton, SIGNAL (released()), this->parent(), SLOT (handleSaveConnection()));
}

NewSessionWindow::~NewSessionWindow() {
	// TODO Auto-generated destructor stub
}

QString NewSessionWindow::getHost()
{
	return this->hostLineEdit->text();
}

} /* namespace Session */
} /* namespace UI */
