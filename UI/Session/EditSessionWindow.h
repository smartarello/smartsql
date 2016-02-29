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
#include <QSpinBox>

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
	int getPort();
	void setName(QString name);
	void setHostName(QString hostname);
	void setUser(QString user);
	void setPassword(QString password);
	void setPort(int port);

private:
	QLineEdit *nameLineEdit ;
	QLineEdit *hostLineEdit;
	QLineEdit *userLineEdit ;
	QLineEdit *passwordLineEdit ;
	QSpinBox *portLineEdit ;
};

}; /* namespace Session */
}; /* namespace UI */

#endif /* UI_SESSION_EDITSESSIONWINDOW_H_ */
