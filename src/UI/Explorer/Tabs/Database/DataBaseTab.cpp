/*
 * DataBaseTab.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Database/DataBaseTab.h>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include "DatabaseModel.h"

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Database {

DataBaseTab::DataBaseTab(QWidget * parent) : QTableView(parent) {

    DatabaseModel *model = new DatabaseModel(this);
    this->setModel(model);
	this->verticalHeader()->hide();
    this->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(this, SIGNAL(doubleClicked(QModelIndex)), SLOT(handleDoubleClicked(QModelIndex)));
}

void DataBaseTab::handleDoubleClicked(QModelIndex index)
{

    qDebug() << index.data();
    //TODO display the table
    qDebug() << "table selected";
}

void DataBaseTab::refresh()
{
    ((DatabaseModel *)this->model())->reload();
    this->resizeColumnsToContents();
}

DataBaseTab::~DataBaseTab() {
}

} /* namespace Database */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
