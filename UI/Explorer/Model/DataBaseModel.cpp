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
#include <QSqlError>

namespace UI {
namespace Explorer {
namespace Model {

DataBaseModel::DataBaseModel(QJsonObject sessionConf, QObject *parent) : QStandardItemModel(parent) {

	this->setColumnCount(2);
	QStandardItem *rootItem = this->invisibleRootItem();

	QSqlDatabase db = QSqlDatabase::database();
	QString name = db.hostName();

	QStandardItem *defaultHost = new QStandardItem(name);
	defaultHost->setData(QVariant(sessionConf));

	rootItem->appendRow(defaultHost);

	QList<QStandardItem *> dbList = this->getDataBaseList();

	foreach (QStandardItem * db, dbList){

		QList<QStandardItem *> cols;
		cols << db;
		QStandardItem *size = new QStandardItem();
		size->setTextAlignment(Qt::AlignRight);
		cols << size;

		defaultHost->appendRow(cols);
	}
}

QList<QStandardItem *> DataBaseModel::getDataBaseList()
{

	QSqlQuery query;
	query.exec("SHOW DATABASES");

	QList<QStandardItem *> dbList;

	while (query.next()) {
		QString dbname = query.value(0).toString();
		QStandardItem *currentDB = new QStandardItem(dbname);
		dbList.append(currentDB);
	}


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
				QJsonObject sessionConf = connectionItem->data().toJsonObject();
				if(db.isOpen()){
					db.close();
				}

				qDebug() << "Closing current connection";
				qInfo() << "Connection to the new database";
				qDebug() << sessionConf;

				db.setHostName(sessionConf.value("hostname").toString());
				db.setUserName(sessionConf.value("user").toString());
				db.setPassword(sessionConf.value("password").toString());
				db.setPort(sessionConf.value("port").toInt());
				db.setDatabaseName(dataBaseItem->text());

				qDebug() << "Loading table list for: " + db.databaseName();

				if (!db.open()){
					qDebug() << db.lastError().text();
					return ;
				}

				QStringList tables = db.tables();
				QHash<QString, QString> tableSize = this->getTableSize();

				qDebug() << "Table count: " + QString::number(tables.count());
				for(int i = 0; i < tables.count(); i++){

					QString tableName = tables.at(i);

					QList<QStandardItem *> cols;
					cols << new QStandardItem(tableName);
					QStandardItem *size = new QStandardItem(tableSize.value(tableName));
					size->setTextAlignment(Qt::AlignRight);
					cols << size;


					dataBaseItem->appendRow(cols);
				}

				QStandardItem *size = connectionItem->child(dataBaseItem->index().row(), 1);
				size->setText(tableSize.value("DATABASE_SIZE"));

				qDebug() << "Table list retrieves successfully";

			}
		}
	}
}

QHash<QString, QString> DataBaseModel::getTableSize()
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
