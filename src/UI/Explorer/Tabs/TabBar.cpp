/*
 * TabBar.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: stephane
 */

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
	this->plusButton->setToolTip(tr("Open a blank query tab (Ctrl+N)"));

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
