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

#ifndef UI_EXPLORER_MODEL_DATABASEMODEL_H_
#define UI_EXPLORER_MODEL_DATABASEMODEL_H_

#include <QStandardItemModel>
#include <QList>
#include <QObject>
#include <QJsonObject>
#include <QStandardItem>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QMap>

namespace UI {
namespace Explorer {
namespace Model {

class DataBaseModel : public QStandardItemModel {

	Q_OBJECT

public:
	DataBaseModel(QJsonObject sessionConf, QObject * parent = 0);
	virtual ~DataBaseModel();
	void addServer(QJsonObject sessionConf);
	void refresh(const QModelIndex & parent) ;
	void dropDatabase(const QModelIndex & index);
	void dropTable(const QModelIndex & index);
	void truncateTable(const QModelIndex & index);
    void addDatabase(QString databaseName, QString collation);

protected:
	bool canFetchMore(const QModelIndex & parent) const Q_DECL_OVERRIDE;
	bool hasChildren(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    void fetchMore(const QModelIndex & index) Q_DECL_OVERRIDE;

private:
	QList<QStandardItem *> getDataBaseList();
	QMap<QString, QString> getTableSize();
	QString getSizeString(double size);

signals:
	void databaseChanged();
	void queryError(QString error);
};

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_MODEL_DATABASEMODEL_H_ */
