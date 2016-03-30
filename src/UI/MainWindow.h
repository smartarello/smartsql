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
#include <QJsonObject>
#include <QSqlQuery>
#include <QMetaType>

#include "Explorer/Explorer.h"
#include "ToolBar.h"

namespace UI {

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	QMainWindow *sessionManager;
	Explorer::Explorer *explorer;
    ToolBar *toolbar;

public slots:
	void handleOpenConnection(QJsonObject sessionConfiguration);
	void openSessionManager();
	void exit();
	void closeExplorer();

};
};



#endif /* UI_MAINWINDOW_H_ */








