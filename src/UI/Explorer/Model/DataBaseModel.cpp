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

#include "../../../Util/DataBase.h"

namespace UI {
namespace Explorer {
namespace Model {

DataBaseModel::DataBaseModel(QJsonObject sessionConf, QObject *parent) : QStandardItemModel(parent) {

	this->setColumnCount(2);
	this->addDatabase(sessionConf);
}

void DataBaseModel::addDatabase(QJsonObject sessionConf)
{
	QString sessionName = sessionConf.value("name").toString();
	QString sessionUuid = sessionConf.value("uuid").toString();
	QStandardItem *rootItem = this->invisibleRootItem();

	// Search if the session is already open
	for (int i = 0; i < rootItem->rowCount(); i++) {
		QStandardItem *host = rootItem->child(i, 0);
		QJsonObject conf = host->data().toJsonObject();
		QString itemUuid = conf.value("uuid").toString();

		if (itemUuid == sessionUuid) {
			return;
		}
	}

	if (!Util::DataBase::open(sessionConf)) {
		return;
	}

	QStandardItem *host = new QStandardItem(sessionName);
	host->setData(QVariant(sessionConf));
	host->setData(QIcon(":/resources/icons/database-server-icon.png"),Qt::DecorationRole);

	rootItem->appendRow(host);

	QList<QStandardItem *> dbList = this->getDataBaseList();

	foreach (QStandardItem * db, dbList){

		db->setData(QIcon(":/resources/icons/database-icon.png"),Qt::DecorationRole);
		QList<QStandardItem *> cols;
		cols << db;
		QStandardItem *size = new QStandardItem();
		size->setTextAlignment(Qt::AlignRight);
		cols << size;

		host->appendRow(cols);
	}
}

QList<QStandardItem *> DataBaseModel::getDataBaseList()
{
	QList<QStandardItem *> dbList;

	QSqlQuery query;
	query.exec("SHOW DATABASES");

	if (query.lastError().isValid()) {
		qWarning() << query.lastError();
		return dbList;
	}

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

				return (dataBaseItem->rowCount() == 0 && this->hasChildren(parent));
			}
		}
	}

	return false;
}

bool DataBaseModel::hasChildren(const QModelIndex & index) const
{
	QStandardItem *rootItem = this->invisibleRootItem();

	if (!index.isValid()) {
		return true;
	} else if (!index.parent().isValid()) {
		// Server node: is there database ?

		QStandardItem *serverItem = rootItem->child(index.row());
		QJsonObject serverConf = serverItem->data().toJsonObject();

		if (Util::DataBase::open(serverConf)) {
			QSqlQuery query;
			if (query.exec("SHOW DATABASES")) {
				return query.size() > 0;
			}
		}

	} else if (!index.parent().parent().isValid()) {
		// Database node: is there table ?

		QStandardItem *serverItem = rootItem->child(index.parent().row());
		QJsonObject serverConf = serverItem->data().toJsonObject();
		QStandardItem *dataBaseItem = serverItem->child(index.row());

		if (Util::DataBase::open(serverConf, dataBaseItem->text())) {
			QSqlQuery query;
			if (query.exec("SHOW TABLES")) {
				return query.size() > 0;
			}
		}

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
					qWarning() << db.lastError().text();
					return ;
				}

				QMap<QString, QString> tableSize = this->getTableSize();

				qDebug() << "Table count: " + QString::number(tableSize.count());

				QMapIterator<QString, QString> iterator(tableSize);
				while(iterator.hasNext()){
					iterator.next();

					if (iterator.key() != "DATABASE_SIZE"){
						QList<QStandardItem *> cols;

						QStandardItem *table = new QStandardItem(iterator.key());
						table->setData(QIcon(":/resources/icons/database-table-icon.png"),Qt::DecorationRole);

						cols << table;
						QStandardItem *size = new QStandardItem(iterator.value());
						size->setTextAlignment(Qt::AlignRight);
						cols << size;


						dataBaseItem->appendRow(cols);
					}

				}

				QStandardItem *size = connectionItem->child(dataBaseItem->index().row(), 1);
				size->setText(tableSize.value("DATABASE_SIZE"));

				qDebug() << "Table list retrieves successfully";

				emit databaseChanged();
			}
		}
	}
}

QMap<QString, QString> DataBaseModel::getTableSize()
{
	QMap<QString, QString> size;
	QSqlQuery query;

	if (!query.exec("show table status")) {
		qWarning() << query.lastError();
		return size;
	}

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

void DataBaseModel::refresh(const QModelIndex & index)
{
	QStandardItem *rootItem = this->invisibleRootItem();

	// Refresh Server node, reload database list
	if (!index.parent().isValid()) {
		qInfo() << "Reload database list";

		QStandardItem *serverItem = rootItem->child(index.row());
		QJsonObject serverConf = serverItem->data().toJsonObject();

		if (Util::DataBase::open(serverConf)) {
			QList<QStandardItem *> dbList = this->getDataBaseList();

			// Process item to add
			foreach (QStandardItem *db, dbList) {

				bool bdFound = false;
				for (int i = 0; i < serverItem->rowCount(); i++) {
					QStandardItem *currentItem = serverItem->child(i, 0);
					if (db->text() == currentItem->text()) {
						bdFound = true;
						break;
					}
				}

				// Item not found in the current model, it should be added
				if (!bdFound) {
					qDebug() << "New database detected : "+ db->text();

					db->setData(QIcon(":/resources/icons/database-icon.png"),Qt::DecorationRole);
					QList<QStandardItem *> cols;
					cols << db;
					QStandardItem *size = new QStandardItem();
					size->setTextAlignment(Qt::AlignRight);
					cols << size;

					serverItem->appendRow(db);
				}
			}

			// Process item to remove
			int i = 0;
			while (i < serverItem->rowCount()) {
				QStandardItem *currentItem = serverItem->child(i, 0);

				bool bdFound = false;
				foreach (QStandardItem *db, dbList) {
					if (db->text() == currentItem->text()) {
						bdFound = true;
						break;
					}
				}

				if (!bdFound) {
					qDebug() << "Remove the database: " + currentItem->text();
					serverItem->removeRow(i); // Do not increase `i` in this case, because we are removing the current index
				} else {
					i++;
				}
			}

			serverItem->sortChildren(0);
		}


	} else if (!index.parent().parent().isValid()) { // Refresh Database node, reload table list

		QStandardItem *serverItem = rootItem->child(index.parent().row());
		QStandardItem *dbItem = serverItem->child(index.row());

		qInfo() << "Reload table list for: "+dbItem->text();

		QJsonObject serverConf = serverItem->data().toJsonObject();

		if (Util::DataBase::open(serverConf)) {
			QMap<QString, QString> tableSize = this->getTableSize();

			QMapIterator<QString, QString> iterator(tableSize);
			while(iterator.hasNext()){
				iterator.next();


				if (iterator.key() != "DATABASE_SIZE"){

					bool tableFound = false;
					for (int i = 0; i < dbItem->rowCount(); i++) {
						QStandardItem *currentItem = dbItem->child(i, 0);
						if (iterator.key() == currentItem->text()) {
							tableFound = true;
							break;
						}
					}

					if (!tableFound) {

						QList<QStandardItem *> cols;

						QStandardItem *table = new QStandardItem(iterator.key());
						table->setData(QIcon(":/resources/icons/database-table-icon.png"),Qt::DecorationRole);

						cols << table;
						QStandardItem *size = new QStandardItem(iterator.value());
						size->setTextAlignment(Qt::AlignRight);
						cols << size;

						dbItem->appendRow(cols);
					}
				}
			}

			// Process item to remove
			int i = 0;
			while (i < dbItem->rowCount()) {
				QStandardItem *currentItem = dbItem->child(i, 0);

				bool tableFound = false;
				QMapIterator<QString, QString> iterator(tableSize);
				while(iterator.hasNext()){
					iterator.next();

					if (iterator.key() == currentItem->text()) {
						tableFound = true;
						break;
					}
				}

				if (!tableFound) {
					qDebug() << "Remove the table: " + currentItem->text();
					dbItem->removeRow(i); // Do not increase `i` in this case, because we are removing the current index
				} else {
					i++;
				}
			}

			dbItem->sortChildren(0);
		}
	} else if (!index.parent().parent().parent().isValid()) {
		// Table node: refresh table size

	}
}

DataBaseModel::~DataBaseModel() {

}

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */
