/*
 * TabView.h
 *
 *  Created on: Mar 18, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_TABVIEW_H_
#define UI_EXPLORER_TABS_TABVIEW_H_

#include <qtabwidget.h>

namespace UI {
namespace Explorer {
namespace Tabs {

class TabView: public QTabWidget {

	Q_OBJECT

public:
	TabView(QWidget * parent = 0);
	virtual ~TabView();
};

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_TABVIEW_H_ */
