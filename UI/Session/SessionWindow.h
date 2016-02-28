/*
 * SessionWindow.h
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#ifndef UI_SESSION_SESSIONWINDOW_H_
#define UI_SESSION_SESSIONWINDOW_H_
#include <QWidget>

namespace UI {
namespace Session {

class SessionWindow : public QWidget
{
	Q_OBJECT


public:
	explicit SessionWindow(QWidget *parent);
	~SessionWindow();

};
};
};

#endif /* UI_SESSION_SESSIONWINDOW_H_ */
