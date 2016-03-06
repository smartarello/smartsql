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
#include <QObject>
#include <QJsonObject>
#include <QStandardItem>
#include <QSqlDatabase>
#include <QModelIndex>

namespace UI {
namespace Explorer {
namespace Model {

class DataBaseModel : public QStandardItemModel {

	Q_OBJECT

public:
	DataBaseModel(QJsonObject sessionConf, QObject * parent = 0);
	virtual ~DataBaseModel();

protected:
	bool canFetchMore(const QModelIndex & parent) const Q_DECL_OVERRIDE;
	bool hasChildren(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
	void fetchMore(const QModelIndex & parent) Q_DECL_OVERRIDE;

private:
	QList<QStandardItem *> getDataBaseList();
	QHash<QString, QString> getTableSize();
	QString getSizeString(double size);
};

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_MODEL_DATABASEMODEL_H_ */
