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
#ifndef RESULTTABLEVIEW_H
#define RESULTTABLEVIEW_H

#include <QTableView>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {
class ResultTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ResultTableView(QWidget *parent = 0);

private:
    QModelIndex contextMenuIndex;

signals:

public slots:

    void contextMenuRequested(QPoint point);
    void handleCopyAction();
};
} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
#endif // RESULTTABLEVIEW_H
