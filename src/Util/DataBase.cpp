/*
 * DataBase.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: stephane
 */

#include <Util/DataBase.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

namespace Util {

DataBase::DataBase() {


}

bool DataBase::open(QJsonObject sessionConfiguration)
{
	QSqlDatabase db = QSqlDatabase::database();

	QString hostName = sessionConfiguration.value("hostname").toString();
	QString userName = sessionConfiguration.value("user").toString();
	int port = sessionConfiguration.value("port").toInt();

	if (!db.isOpen() || hostName != db.hostName() || db.userName() != userName || port != db.port()) {

		if (db.isOpen()) {
			db.close();
		}

		db.setHostName(hostName);
		db.setUserName(userName);
		db.setDatabaseName("");
		db.setPassword(sessionConfiguration.value("password").toString());
		db.setPort(port);

		if (!db.open()) {
			qDebug() << db.lastError();
			return false;
		}
	}

	return true;
}

DataBase::~DataBase() {
}

} /* namespace Util */
