/*
 * DataBaseModel.h
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_MODEL_DATABASEMODEL_H_
#define UI_EXPLORER_MODEL_DATABASEMODEL_H_

#include <QStandardItemModel>
#include <QList>
#include <QStandardItem>
#include <QSqlDatabase>
#include <QModelIndex>

namespace UI {
namespace Explorer {
namespace Model {

class DataBaseModel : public QStandardItemModel {

	Q_OBJECT

public:
	DataBaseModel();
	virtual ~DataBaseModel();
	bool canFetchMore(const QModelIndex & parent) const;
	bool hasChildren(const QModelIndex & parent = QModelIndex()) const;
	void fetchMore(const QModelIndex & parent);

private:
	QList<QStandardItem *> getDataBaseList(QSqlDatabase db);
	QHash<QString, QString> getTableSize(QSqlDatabase db);
	QString getSizeString(double size);
};

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_MODEL_DATABASEMODEL_H_ */
