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
#include <QTabWidget>
#include "DataBaseTree.h"
#include "Tabs/Database/DataBaseTab.h"

namespace UI {
namespace Explorer {

class Explorer: public QWidget {

	Q_OBJECT

public:
	Explorer(QWidget *parent, QJsonObject sessionConf);
	virtual ~Explorer();

signals:
	void databaseChanged();

public slots:
	void dataBaseTreeClicked(QModelIndex index);
	void dataBaseTreeDoubleClicked(QModelIndex index);
	void addQueryTab();
	void closeQueryTab(int index);
	void refreshDatabase();

private:
	DataBaseTree *dataBaseTree;
	QLineEdit *tableFilterLineEdit;
	QTabWidget *explorerTabs;
	Tabs::Database::DataBaseTab *databaseTab;
	Tabs::Table::TableTab *tableTab;
};

} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_EXPLORER_H_ */
