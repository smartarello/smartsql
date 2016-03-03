/*
 * DataBaseModel.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Model/DataBaseModel.h>
#include <QSqlQuery>
#include <QDebug>

namespace UI {
namespace Explorer {
namespace Model {

DataBaseModel::DataBaseModel() {


	QStandardItem *rootItem = this->invisibleRootItem();

	rootItem->setWhatsThis("root");
	qDebug() << rootItem->whatsThis();

	QSqlDatabase db = QSqlDatabase::database();
	QString name = db.hostName();

	QStandardItem *defaultHost = new QStandardItem(name);
	defaultHost->setWhatsThis("host");
	QList<QStandardItem *> dbList = this->getDataBaseList(db);
	defaultHost->appendRows(dbList);

	rootItem->appendRow(defaultHost);

}

QList<QStandardItem *> DataBaseModel::getDataBaseList(QSqlDatabase db)
{
	db.open();
	QSqlQuery query;
	query.exec("SHOW DATABASES");

	QList<QStandardItem *> dbList;

	while (query.next()) {
		QString dbname = query.value(0).toString();
		QStandardItem *currentDB = new QStandardItem(dbname);
		currentDB->setWhatsThis("database");
		dbList.append(currentDB);
	}

	db.close();
	return dbList;
}

bool DataBaseModel::canFetchMore(const QModelIndex & parent) const
{
	QModelIndex root = parent.parent();
	if (root.isValid() && !root.parent().isValid()){
		QStandardItem *rootItem = this->invisibleRootItem();
		QStandardItem *connectionItem = rootItem->child(root.row());

		if (connectionItem != 0){

			QStandardItem *dataBaseItem = connectionItem->child(parent.row());

			if (dataBaseItem != 0){
				return !dataBaseItem->hasChildren();
			}
		}
	}

	return false;
}

bool DataBaseModel::hasChildren(const QModelIndex & parent) const
{
	QModelIndex root = parent.parent();
	if (root.isValid() && !root.parent().isValid()){
		return true;
	}
	else if (!parent.isValid() || !root.isValid())
	{
		return  true;
	}

	return false;
}

void DataBaseModel::fetchMore(const QModelIndex & parent)
{
	QModelIndex root = parent.parent();
	if (root.isValid() && !root.parent().isValid()){
		QStandardItem *rootItem = this->invisibleRootItem();
		QStandardItem *connectionItem = rootItem->child(root.row());

		if (connectionItem != 0){

			QStandardItem *dataBaseItem = connectionItem->child(parent.row());

			if (dataBaseItem != 0){

				QSqlDatabase db = QSqlDatabase::database();
				db.setDatabaseName(dataBaseItem->text());
				if (db.open()){
					QStringList tables = db.tables();
					for(int i = 0; i < tables.count(); i++){
						dataBaseItem->appendRow(new QStandardItem(tables.at(i)));
					}

					db.close();
				}
			}
		}
	}
}

DataBaseModel::~DataBaseModel() {
	// TODO Auto-generated destructor stub
}

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */
