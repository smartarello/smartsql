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
#ifndef TABLEDETAILSWINDOW_H
#define TABLEDETAILSWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QSqlDatabase>

class TableDetailsWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit TableDetailsWindow(QSqlDatabase database, QString tableName,  QWidget *parent = 0);

private:
    QTableView *tableColumns;

signals:

public slots:
};

#endif // TABLEDETAILSWINDOW_H
