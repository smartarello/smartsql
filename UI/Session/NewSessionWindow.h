/*
 * NewSessionWindow.h
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#ifndef UI_SESSION_NEWSESSIONWINDOW_H_
#define UI_SESSION_NEWSESSIONWINDOW_H_

#include <QWidget>
#include <QLineEdit>

namespace UI {
namespace Session {

class NewSessionWindow : public QWidget
{
	Q_OBJECT


public:
	explicit NewSessionWindow(QWidget *parent);
	virtual ~NewSessionWindow();
	QString getHost();
//	std::string getName();
//	std::string getPassword();
//	std::string getPort();

private:
	QLineEdit *nameLineEdit ;
	QLineEdit *hostLineEdit;
	QLineEdit *userLineEdit ;
	QLineEdit *password ;
	QLineEdit *portLineEdit ;
};

}; /* namespace Session */
}; /* namespace UI */

#endif /* UI_SESSION_NEWSESSIONWINDOW_H_ */
