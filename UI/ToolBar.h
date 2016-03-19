/*
 * ToolBar.h
 *
 *  Created on: Mar 19, 2016
 *      Author: stephane
 */

#ifndef UI_TOOLBAR_H_
#define UI_TOOLBAR_H_

#include <qtoolbar.h>

namespace UI {

class ToolBar: public QToolBar {

	Q_OBJECT

public:
	ToolBar(QWidget *parent = 0);
	virtual ~ToolBar();
};

} /* namespace UI */

#endif /* UI_TOOLBAR_H_ */
