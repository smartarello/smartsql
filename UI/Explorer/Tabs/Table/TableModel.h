/*
 * TableModel.h
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

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
	void setTable(QString table);
	QList<QString> getColumns();
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex & item, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex()) Q_DECL_OVERRIDE;
	int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex & parent) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

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
	void reload();
	QSqlQuery query;
	QList<QSqlRecord> results;
};

} /* namespace Table */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_MODEL_TABLEDATATABMODEL_H_ */
