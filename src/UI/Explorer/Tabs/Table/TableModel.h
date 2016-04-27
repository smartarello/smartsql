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

#ifndef UI_EXPLORER_TABS_MODEL_TABLEDATATABMODEL_H_
#define UI_EXPLORER_TABS_MODEL_TABLEDATATABMODEL_H_

#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QAbstractTableModel>


namespace UI {
namespace Explorer {
namespace Tabs {
namespace Table {

class TableModel: public QAbstractTableModel {

	Q_OBJECT

public:
	TableModel(QObject * parent = 0);
	virtual ~TableModel();
    void setTable(QSqlDatabase database, QString table, QString filter = "");
	QList<QString> getColumns();
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex & item, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex()) Q_DECL_OVERRIDE;
	int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex & parent) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    void reload();
    QHash<QString,QStringList> getForeignKeys();

public slots:
	void refreshWithFilter(QString filter);

signals:
	void queryError(QString query, QString error);

private:
	QString table;
	QString filter;
	QString sortOrder;
	QList<QString> columns;
	QStringList primaryKey;
	void sort(int column, Qt::SortOrder order);
	QString buildQuery();
	QList<QSqlRecord> results;
    QSqlDatabase database;
    QHash<QString, QStringList> foreignKeys;
};

} /* namespace Table */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_MODEL_TABLEDATATABMODEL_H_ */
