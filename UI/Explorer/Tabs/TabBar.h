/*
 * TabBar.h
 *
 *  Created on: Mar 18, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_TABBAR_H_
#define UI_EXPLORER_TABS_TABBAR_H_

#include <qtabbar.h>
#include <QPushButton>

namespace UI {
namespace Explorer {
namespace Tabs {

class TabBar: public QTabBar {

	Q_OBJECT

public:
	TabBar(QWidget * parent = 0);
	virtual ~TabBar();
	QPushButton *plusButton;
	void tabLayoutChange();
	void resizeEvent(QResizeEvent *event);
	QSize sizeHint() const;

signals:
	void newTabRequested();

private:
	void movePlusButton();

private slots:
	void addTabClicked();

};

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_TABBAR_H_ */
