/*
 * Explorer.h
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_EXPLORER_H_
#define UI_EXPLORER_EXPLORER_H_

#include <UI/Explorer/Tabs/Table/TableTab.h>
#include <QWidget>
#include <QLineEdit>
#include <QModelIndex>
#include <QStandardItem>
#include <QItemSelection>
#include <QTabWidget>
#include "DataBaseTree.h"
#include "Tabs/Database/DataBaseTab.h"
#include "Tabs/Server/ServerTab.h"

namespace UI {
namespace Explorer {

class Explorer: public QWidget {

	Q_OBJECT

public:
	Explorer(QWidget *parent, QJsonObject sessionConf);
	virtual ~Explorer();
	void addDatabase(QJsonObject sessionConfiguration);

signals:
	void databaseChanged();
	void closeExplorer();

public slots:
    void dataBaseTreeItemChanged();
	void dataBaseTreeDoubleClicked(QModelIndex index);
	void addQueryTab();
	void closeQueryTab(int index);
	void refreshDatabase();
	void handleCloseExplorer();

private:
	DataBaseTree *dataBaseTree;
	QLineEdit *tableFilterLineEdit;
	QLineEdit *databaseFilterLineEdit;
	QTabWidget *explorerTabs;
	Tabs::Database::DataBaseTab *databaseTab;
	Tabs::Table::TableTab *tableTab;
    ServerTab *serverTab;

};

} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_EXPLORER_H_ */
