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
	QAction *disconnectSession = this->addAction(QIcon(":/resources/icons/disconnect.png"), tr("Close selected database connection"));
	this->addSeparator();

	connect(openSession, SIGNAL(triggered(bool)), parent, SLOT(openSessionManager()));
}

ToolBar::~ToolBar() {

}

} /* namespace UI */
