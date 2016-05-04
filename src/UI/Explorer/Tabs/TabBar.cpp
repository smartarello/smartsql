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

#include <UI/Explorer/Tabs/TabBar.h>
#include <QIcon>

namespace UI {
namespace Explorer {
namespace Tabs {

TabBar::TabBar(QWidget * parent) : QTabBar(parent) {

	QIcon icon(":/resources/icons/plus.png");
	this->plusButton = new QPushButton();
	this->plusButton->setIcon(icon);
	this->plusButton->setIconSize(QSize(18, 18));
	this->plusButton->setMaximumSize(40, 25) ;
	this->plusButton->setMinimumSize(40, 25) ;
    this->plusButton->setToolTip(tr("Open a blank query tab (Ctrl+T)"));

	this->plusButton->setParent(this);
	this->movePlusButton();

	connect(this->plusButton, SIGNAL (clicked(bool)), this, SLOT (addTabClicked()));
}

void TabBar::movePlusButton()
{
	 int size = 0;
	 int i = 0;
	 while ( i < this->count()){
		 size += this->tabRect(i).width();
		 i++;
	 }

	 int h = this->geometry().bottom() - 25;
	 int w = this->width();
	 if ( size > w ) {
		 this->plusButton->move(w-54, h);
	 } else {
		 this->plusButton->move(size, h);
	 }
}

void TabBar::tabLayoutChange()
{
	QTabBar::tabLayoutChange();
	this->movePlusButton();
}

QSize TabBar::sizeHint() const
{
	QSize sizeHint = QTabBar::sizeHint();
	int width = sizeHint.width();
	int height = sizeHint.height();
	return QSize(width+70, height);
}

void TabBar::resizeEvent(QResizeEvent *event)
{
	QTabBar::resizeEvent(event);
	this->movePlusButton();
}

void TabBar::addTabClicked()
{
	emit newTabRequested();
}

TabBar::~TabBar() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
