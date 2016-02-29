/*
 * SessionWindow.h
 *
 *  Created on: Feb 28, 2016
 *      Author: stephane
 */

#ifndef UI_SESSION_SESSIONWINDOW_H_
#define UI_SESSION_SESSIONWINDOW_H_
#include <QWidget>
#include <QListView>
#include <QItemSelection>
#include "EditSessionWindow.h"

namespace UI {
namespace Session {

class SessionWindow : public QWidget
{
	Q_OBJECT


public:
	explicit SessionWindow(QWidget *parent);
	~SessionWindow();

private:
	QListView *sessionList;
	EditSessionWindow * editSession;

public slots:
		void handleNewConnection();
		void handleSaveConnection();
		void handleSelectionChanged(const QItemSelection& selection);
		void handleExit();

};
};
};

#endif /* UI_SESSION_SESSIONWINDOW_H_ */
