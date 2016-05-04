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
#include "TableDetailsTab.h"
#include <QVBoxLayout>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QTabWidget>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "UI/Explorer/Tabs/SQLSyntaxHighlighter.h"
#include "TableDetailsModel.h"
#include "ForeignKeyModel.h"
#include "TableIndexModel.h"

TableDetailsTab::TableDetailsTab(QWidget *parent) : QWidget(parent)
{
    QTabWidget *tabWidget = new QTabWidget(this);

    QVBoxLayout *containerLayout = new QVBoxLayout(this);

    this->tableColumns = new QTableView(this);
    this->tableColumns->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabWidget->addTab(this->tableColumns, tr("Columns"));

    this->tableindexes = new QTableView(this);
    this->tableindexes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabWidget->addTab(this->tableindexes, tr("Indexes"));

    this->tableForeignKeys = new QTableView(this);
    this->tableForeignKeys->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabWidget->addTab(this->tableForeignKeys, tr("Foreign keys"));

    createTableTextEdit = new QTextEdit(this);
    createTableTextEdit->setReadOnly(true);
    UI::Explorer::Tabs::SQLSyntaxHighlighter *highlighter  = new UI::Explorer::Tabs::SQLSyntaxHighlighter(createTableTextEdit->document());
    tabWidget->addTab(createTableTextEdit, tr("Create table"));

    containerLayout->addWidget(tabWidget);
}

void TableDetailsTab::setTable(QSqlDatabase database, QString tableName)
{
    Util::TableDefinition table(database, tableName);

    this->tableColumns->setModel(new TableDetailsModel(table, this));
    this->tableForeignKeys->setModel(new ForeignKeyModel(table, this));
    this->tableindexes->setModel(new TableIndexModel(table, this));
    this->tableForeignKeys->resizeColumnsToContents();
    this->tableindexes->resizeColumnsToContents();
    this->tableColumns->resizeColumnsToContents();

    QString createString = table.createTable().replace("\n", "<br/>");
    createTableTextEdit->setText(createString);
}
