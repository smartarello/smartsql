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

#include "ServerTab.h"
#include <QSqlQueryModel>
#include <QHeaderView>

ServerTab::ServerTab(QWidget *parent) : QTableView(parent)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SHOW DATABASES");

    this->setModel(model);
    this->resizeColumnsToContents();
    this->verticalHeader()->hide();

    connect(this, SIGNAL(doubleClicked(QModelIndex)), SLOT(handleDoubleClicked(QModelIndex)));
}

void ServerTab::handleDoubleClicked(QModelIndex index)
{
    QVariant databaseName = this->model()->data(index);
    emit showDatabase(databaseName.toString());
}

ServerTab::~ServerTab() {

}
