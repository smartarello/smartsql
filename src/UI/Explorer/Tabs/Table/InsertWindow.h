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
#ifndef INSERTWINDOW_H
#define INSERTWINDOW_H

#include <QMainWindow>
#include "Util/TableDefinition.h"
#include <QTextEdit>
#include <QCheckBox>
#include <QSqlDatabase>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Table {
class InsertWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit InsertWindow(QSqlDatabase connection, Util::TableDefinition table, QWidget *parent = 0);

private:
    QTextEdit *insertStatement;
    Util::TableDefinition table;
    QList<QWidget *> values;
    QList<QCheckBox *> checkboxes;
    QSqlDatabase connection;

signals:

public slots:
    void refreshInsertStatement();
    void handleInsert();
};

} /* namespace Table */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif // INSERTWINDOW_H
