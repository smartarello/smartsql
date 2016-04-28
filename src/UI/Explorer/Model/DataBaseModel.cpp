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
    this->addServer(sessionConf);
}

void DataBaseModel::addServer(QJsonObject sessionConf)
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
		qDebug() << "DataBaseModel::addDatabase - fail to open connection";
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

void DataBaseModel::addDatabase(QString databaseName, QString collation)
{
    QString sql = QString("CREATE DATABASE %1 COLLATE=%2").arg(databaseName).arg(collation);

    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "DataBaseModel::addDatabase - " + query.lastError().text();
        emit queryError(query.lastError().text());
    }
}

QList<QStandardItem *> DataBaseModel::getDataBaseList()
{
	QList<QStandardItem *> dbList;

	QSqlQuery query;
	query.exec("SHOW DATABASES");

	if (query.lastError().isValid()) {
		qWarning() << "DataBaseModel::getDataBaseList - " + query.lastError().text();
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
				QVariant data = dataBaseItem->data();
				return data.isNull();
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
			} else {
				qDebug() << "DataBaseModel::hasChildren - " + query.lastError().text();
			}
		}

	} else if (!index.parent().parent().isValid()) {

		return true;

	}

	return false;
}

void DataBaseModel::fetchMore(const QModelIndex & index)
{
    if (index.parent().isValid() && !index.parent().parent().isValid()){
        this->refresh(index);
        emit databaseChanged();
	}
}

QMap<QString, QString> DataBaseModel::getTableSize()
{
	QMap<QString, QString> size;
	QSqlQuery query;

	if (!query.exec("show table status")) {
		qWarning() << "DataBaseModel::getTableSize - " + query.lastError().text();
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

void DataBaseModel::dropDatabase(const QModelIndex & index)
{
	QStandardItem *rootItem = this->invisibleRootItem();
	QStandardItem *serverItem = rootItem->child(index.parent().row());
	QStandardItem *databaseItem = serverItem->child(index.row());

	QJsonObject serverConf = serverItem->data().toJsonObject();
	if (Util::DataBase::open(serverConf, databaseItem->text())) {
		QSqlQuery dropQuery;

		if (dropQuery.exec("DROP DATABASE "+databaseItem->text())){
			serverItem->removeRow(index.row());
		} else {
			qDebug() << "DataBaseModel::dropDatabase - " + dropQuery.lastError().text();
			emit queryError(dropQuery.lastError().text());
		}
	}
}

void DataBaseModel::dropTable(const QModelIndex & index)
{
	QStandardItem *rootItem = this->invisibleRootItem();
	QStandardItem *serverItem = rootItem->child(index.parent().parent().row());
	QStandardItem *databaseItem = serverItem->child(index.parent().row());
	QStandardItem *tableItem = databaseItem->child(index.row());

	QJsonObject serverConf = serverItem->data().toJsonObject();
	if (Util::DataBase::open(serverConf, databaseItem->text())) {
		QSqlQuery dropQuery;

		qInfo() << "Drop table " + tableItem->text();
		if (dropQuery.exec("DROP TABLE "+tableItem->text())){
			databaseItem->removeRow(index.row());
		} else {
			qDebug() << "DataBaseModel::dropTable - " + dropQuery.lastError().text() ;
			emit queryError(dropQuery.lastError().text());
		}
	}
}

void DataBaseModel::truncateTable(const QModelIndex & index)
{
	QStandardItem *rootItem = this->invisibleRootItem();
	QStandardItem *serverItem = rootItem->child(index.parent().parent().row());
	QStandardItem *databaseItem = serverItem->child(index.parent().row());
	QStandardItem *tableItem = databaseItem->child(index.row());

	QJsonObject serverConf = serverItem->data().toJsonObject();
	if (Util::DataBase::open(serverConf, databaseItem->text())) {
		QSqlQuery truncateQuery;

		qInfo() << "Truncate table " + tableItem->text();
		if (truncateQuery.exec("TRUNCATE TABLE "+tableItem->text())){

			this->refresh(index); // Refresh table size
		} else {
			qDebug() << "DataBaseModel::truncateTable - " + truncateQuery.lastError().text();
			emit queryError(truncateQuery.lastError().text());
		}
	}
}

void DataBaseModel::refresh(const QModelIndex & index)
{
	QStandardItem *rootItem = this->invisibleRootItem();

	// Refresh Server node, reload database list
	if (!index.parent().isValid()) {

		QStandardItem *serverItem = rootItem->child(index.row());
		QJsonObject serverConf = serverItem->data().toJsonObject();

		qInfo() << "Reload database list: " + serverItem->text();

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
					db->setData(QIcon(":/resources/icons/database-icon.png"),Qt::DecorationRole);
					QList<QStandardItem *> cols;
					cols << db;
					QStandardItem *size = new QStandardItem();
					size->setTextAlignment(Qt::AlignRight);
					cols << size;

                    serverItem->appendRow(cols);
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
					serverItem->removeRow(i); // Do not increase `i` in this case, because we are removing the current index
				} else {
					i++;
				}
			}

		}


	} else if (!index.parent().parent().isValid()) { // Refresh Database node, reload table list

		QStandardItem *serverItem = rootItem->child(index.parent().row());
		QStandardItem *dbItem = serverItem->child(index.row());

		QJsonObject serverConf = serverItem->data().toJsonObject();

		if (Util::DataBase::open(serverConf, dbItem->text())) {
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
					dbItem->removeRow(i); // Do not increase `i` in this case, because we are removing the current index
				} else {
					i++;
				}
			}

			dbItem->setData(QVariant(true)); // Mark data as loaded
            QStandardItem *dbSize = serverItem->child(index.row(), 1);
            dbSize->setText(tableSize.value("DATABASE_SIZE"));
		}
	} else if (!index.parent().parent().parent().isValid()) {
		// Table node: refresh table size

		QStandardItem *serverItem = rootItem->child(index.parent().parent().row());
		QStandardItem *dbItem = serverItem->child(index.parent().row());
		QStandardItem *tableItem = dbItem->child(index.row());

		qInfo() << "Reload table size: " + tableItem->text();

		QJsonObject serverConf = serverItem->data().toJsonObject();

		if (Util::DataBase::open(serverConf, dbItem->text())) {

			QSqlQuery query;

			if (query.exec(QString("show table status WHERE Name LIKE '%1'").arg(tableItem->text()))) {

				if (query.next()) {
					double dataLength = query.value("Data_length").toDouble();
					double indexLength = query.value("Index_length").toDouble();

					double totalSize = (dataLength + indexLength) / 1024;
					QStandardItem *tableSize = dbItem->child(index.row(), 1);
					tableSize->setText(this->getSizeString(totalSize));
				}
			} else {

				qWarning() << "DataBaseModel::refresh - " + query.lastError().text();
			}
		}
	}
}

DataBaseModel::~DataBaseModel() {

}

} /* namespace Model */
} /* namespace Explorer */
} /* namespace UI */
