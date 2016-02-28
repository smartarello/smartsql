/*
 * MainWindow.h
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#ifndef UI_MAINWINDOW_H_
#define UI_MAINWINDOW_H_

#include <QMainWindow>
#include <QDebug>
#include "Session/NewSessionWindow.h"

namespace UI {

class MainWindow : public QMainWindow
{
	Q_OBJECT


public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void handleNewConnection();
	void handleCancelNewConnection();
	void handleSaveConnection();

private:
	Session::NewSessionWindow *editSession;

};
};

#endif /* UI_MAINWINDOW_H_ */








