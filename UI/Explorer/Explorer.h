/*
 * Explorer.h
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_EXPLORER_H_
#define UI_EXPLORER_EXPLORER_H_

#include <QWidget>
#include <QLineEdit>
#include <QModelIndex>
#include <QTabWidget>
#include "DataBaseTree.h"
#include "Tabs/DataBaseTab.h"
#include "Tabs/TableDataTab.h"

namespace UI {
namespace Explorer {

class Explorer: public QWidget {

	Q_OBJECT

public:
	Explorer(QWidget *parent = 0);
	virtual ~Explorer();

public slots:
	void dataBaseTreeClicked(QModelIndex index);


private:
	DataBaseTree *dataBaseTree;
	QLineEdit *tableFilterLineEdit;
	QTabWidget *explorerTabs;
	Tabs::DataBaseTab *databaseTab;
	Tabs::TableDataTab *tableTab;
};

} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_EXPLORER_H_ */
