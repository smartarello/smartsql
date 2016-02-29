/*
 * NewSessionWindow.h
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#ifndef UI_SESSION_EDITSESSIONWINDOW_H_
#define UI_SESSION_EDITSESSIONWINDOW_H_

#include <QWidget>
#include <QLineEdit>

namespace UI {
namespace Session {

class EditSessionWindow : public QWidget
{
	Q_OBJECT


public:
	explicit EditSessionWindow(QWidget *parent);
	virtual ~EditSessionWindow();
	QString getName();
	QString getHostName();
	QString getUser();
	QString getPassword();
	QString getPort();

private:
	QLineEdit *nameLineEdit ;
	QLineEdit *hostLineEdit;
	QLineEdit *userLineEdit ;
	QLineEdit *passwordLineEdit ;
	QLineEdit *portLineEdit ;
};

}; /* namespace Session */
}; /* namespace UI */

#endif /* UI_SESSION_EDITSESSIONWINDOW_H_ */
