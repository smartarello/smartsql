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
#include <QJsonObject>
#include <QStandardItemModel>
#include "EditSessionWindow.h"

namespace UI {
namespace Session {

class SessionWindow : public QWidget
{
	Q_OBJECT


public:
	explicit SessionWindow(QWidget *parent);
	~SessionWindow();
	void disableExitButton();


private:
	QListView *sessionList;
	EditSessionWindow * editSession;
	QJsonArray sessionStore;
	QStandardItemModel *model;
	QPushButton *exitButton;

	QJsonObject getSelectedSession();
	void updateSelected();
	void persistSessionStore();

public slots:
		void handleNewConnection();
		void handleSaveConnection();
		void handleSelectionChanged(const QItemSelection& selection);
		void handleExit();
		void handleDelete();
		void handleOpenConnection();

signals:
	void openConnection(QJsonObject sessionConfiguration);

};
};
};

#endif /* UI_SESSION_SESSIONWINDOW_H_ */
