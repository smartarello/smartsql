/*
 * ToolBar.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: stephane
 */

#include <UI/ToolBar.h>
#include <QAction>

namespace UI {

ToolBar::ToolBar(QWidget *parent) : QToolBar(parent) {

	QAction *openSession = this->addAction(QIcon(":/resources/icons/connect.png"), tr("Display session manager"));

	this->addSeparator();
	QAction *exitAction = this->addAction(QIcon(":/resources/icons/exit.png"), tr("Exit"));

	connect(openSession, SIGNAL(triggered(bool)), parent, SLOT(openSessionManager()));
	connect(exitAction, SIGNAL(triggered(bool)), parent, SLOT(exit()));
}

ToolBar::~ToolBar() {

}

} /* namespace UI */
