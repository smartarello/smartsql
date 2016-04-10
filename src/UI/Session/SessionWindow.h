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
