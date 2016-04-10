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
