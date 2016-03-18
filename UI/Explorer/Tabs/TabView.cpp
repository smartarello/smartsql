/*
 * TabView.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/TabView.h>
#include "TabBar.h"

namespace UI {
namespace Explorer {
namespace Tabs {

TabView::TabView(QWidget * parent) : QTabWidget(parent) {

	this->setTabBar(new TabBar(this));
	this->setTabsClosable(true);
}

TabView::~TabView() {

}

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
