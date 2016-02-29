/*
 * MainWindow.h
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#ifndef UI_MAINWINDOW_H_
#define UI_MAINWINDOW_H_

#include <UI/Session/EditSessionWindow.h>
#include <QMainWindow>
#include <QDebug>

namespace UI {

class MainWindow : public QMainWindow
{
	Q_OBJECT


public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Session::EditSessionWindow *editSession;

};
};

#endif /* UI_MAINWINDOW_H_ */








