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
#include "ResultTableView.h"
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QIcon>
#include <QDebug>
#include <QModelIndexList>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

ResultTableView::ResultTableView(QWidget *parent) : QTableView(parent)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(contextMenuRequested(QPoint)));
}

void ResultTableView::contextMenuRequested(QPoint point)
{
    this->contextMenuIndex = this->indexAt(point);

    if (!this->contextMenuIndex.isValid()){
        return ;
    }

    QModelIndexList list = this->selectionModel()->selectedRows(0);
    QMenu *menu = new QMenu(this);

    QAction *copyAction = new QAction(tr("Copy"), this);
    copyAction->setIcon(QIcon(":/resources/icons/copy-icon.png"));
    menu->addAction(copyAction);

    connect(copyAction, SIGNAL(triggered(bool)), SLOT(handleCopyAction()));
    menu->popup(this->viewport()->mapToGlobal(point));
}

void ResultTableView::handleCopyAction()
{
    QVariant cellData = this->model()->data(this->contextMenuIndex);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(cellData.toString());
}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
