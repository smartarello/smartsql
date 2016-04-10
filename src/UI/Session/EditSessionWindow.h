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

#ifndef UI_SESSION_EDITSESSIONWINDOW_H_
#define UI_SESSION_EDITSESSIONWINDOW_H_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
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
	QPushButton *saveButton;

private:
	QLineEdit *nameLineEdit ;
	QLineEdit *hostLineEdit;
	QLineEdit *userLineEdit ;
	QLineEdit *passwordLineEdit ;
	QSpinBox *portLineEdit ;


public slots:
	void edited();
	void testConnection();
};

}; /* namespace Session */
}; /* namespace UI */

#endif /* UI_SESSION_EDITSESSIONWINDOW_H_ */
