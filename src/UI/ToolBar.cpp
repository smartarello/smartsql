/**
 * Copyright (C) 2016  Stéphane Martarello
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

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
