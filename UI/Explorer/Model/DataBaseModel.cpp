/*
 * DataBaseModel.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Model/DataBaseModel.h>
#include <QSqlQuery>
#include <QHash>
#include <QDebug>

namespace UI {
namespace Explorer {
namespace Model {

DataBaseModel::DataBaseModel() {

	this->setColumnCount(2);
	QStandardItem *rootItem = this->invisibleRootItem();

	QSqlDatabase db = QSqlDatabase::database();
	QString name = db.hostName();

	QStandardItem *defaultHost = new QStandardItem(name);

	QList<QStandardItem *> dbList = this->getDataBaseList(db);

	int row = 0;
	foreach (QStandardItem * db, dbList){
		defaultHost->setChild(row, 0, db);
		defaultHost->setChild(row++, 1, new QStandardItem(""));
	}

	rootItem->setChild(0, 0, defaultHost);
	rootItem->setChild(0, 1, new QStandardItem(""));


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

				return (dataBaseItem->rowCount() == 0);
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
					QHash<QString, QString> tableSize = this->getTableSize(db);

					for(int i = 0; i < tables.count(); i++){

						QString tableName = tables.at(i);

						QList<QStandardItem *> cols;
						cols << new QStandardItem(tableName);
						QStandardItem *size = new QStandardItem(tableSize.value(tableName));
						size->setTextAlignment(Qt::AlignRight);
						cols << size;
						dataBaseItem->appendRow(cols);
					}

					QStandardItem *size = new QStandardItem(tableSize.value("DATABASE_SIZE"));
					size->setTextAlignment(Qt::AlignRight);
					connectionItem->setChild(parent.row(), 1, size);

					db.close();
				}
			}
		}
	}
}

QHash<QString, QString> DataBaseModel::getTableSize(QSqlDatabase db)
{
	QHash<QString, QString> size;
	QSqlQuery query;
	query.exec("show table status");

	double dataBaseTotalSize = 0;

	while (query.next()){
		double dataLength = query.value("Data_length").toDouble();
		double indexLength = query.value("Index_length").toDouble();

		double totalSize = (dataLength + indexLength) / 1024;

		dataBaseTotalSize += totalSize;

		size.insert(query.value("Name").toString(), this->getSizeString(totalSize));
	}

	size.insert("DATABASE_SIZE", this->getSizeString(dataBaseTotalSize));

	return size;
}

QString DataBaseModel::getSizeString(double size) {
	if (size < 1000){
		return QString("%1 Kb").arg(QString::number(size, 'f', 0));
	}
	else{
		size = size / 1024;
		if (size < 1000){
			return QString("%1 Mb").arg(QString::number(size, 'f', 0));
		}
		else{
			size = size / 1024;
			return QString("%1 Gb").arg(QString::number(size, 'f', 2));
		}
	}
}

DataBaseModel::~DataBaseModel() {
	// TODO Auto-generated destructor stub
}

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */
