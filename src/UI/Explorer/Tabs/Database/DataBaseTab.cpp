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

    DatabaseModel *model = new DatabaseModel();
    QSortFilterProxyModel *sortModel = new QSortFilterProxyModel();
    sortModel->setSourceModel(model);
    sortModel->setSortRole(Qt::UserRole+1);

    this->setModel(sortModel);
	this->verticalHeader()->hide();
    this->setSortingEnabled(true);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(this, SIGNAL(doubleClicked(QModelIndex)), SLOT(handleDoubleClicked(QModelIndex)));
}

void DataBaseTab::handleDoubleClicked(QModelIndex index)
{
    QSortFilterProxyModel *sortModel = (QSortFilterProxyModel *) this->model();
    QModelIndex sourceIndex = sortModel->mapToSource(index);
    qDebug() << sourceIndex.data();
    //TODO display the table
    qDebug() << "table selected";
}

void DataBaseTab::refresh()
{
    QSortFilterProxyModel *sortModel = (QSortFilterProxyModel *) this->model();
    ((DatabaseModel *)sortModel->sourceModel())->reload();
    //sortModel->sort(0);
    this->sortByColumn(0, Qt::AscendingOrder);
    this->resizeColumnsToContents();
}

DataBaseTab::~DataBaseTab() {
    delete this->model();
}

} /* namespace Database */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
