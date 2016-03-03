/*
 * Explorer.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Explorer.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeView>

namespace UI {
namespace Explorer {

Explorer::Explorer(QWidget *parent) : QWidget(parent) {

	this->dataBaseTree = new DataBaseTree(this);
	QHBoxLayout *hboxlayout = new QHBoxLayout;
	this->setLayout(hboxlayout);

	QWidget *leftPartWidget = new QWidget;
	leftPartWidget->setFixedWidth(350);
	QVBoxLayout *leftPartlayout = new QVBoxLayout;
	leftPartWidget->setLayout(leftPartlayout);

	this->tableFilterLineEdit = new QLineEdit();
	leftPartlayout->addWidget(this->tableFilterLineEdit);
	leftPartlayout->addWidget(this->dataBaseTree);

	hboxlayout->addWidget(leftPartWidget);


	connect(this->tableFilterLineEdit, SIGNAL (textEdited(QString)), this->dataBaseTree, SLOT (filterTable(QString)));
}


Explorer::~Explorer() {
	// TODO Auto-generated destructor stub
}

} /* namespace Explorer */
} /* namespace UI */
