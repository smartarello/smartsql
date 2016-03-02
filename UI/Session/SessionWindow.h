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
#include <QJsonArray>
#include <QStringListModel>
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
	QJsonArray sessionStore;
	void updateSelected();
	void persistSessionStore();
	QStringListModel *model;

public slots:
		void handleNewConnection();
		void handleSaveConnection();
		void handleSelectionChanged(const QItemSelection& selection);
		void handleExit();
		void handleDelete();
		void handleOpenConnection();

};
};
};

#endif /* UI_SESSION_SESSIONWINDOW_H_ */
