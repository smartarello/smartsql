/*
 * DataBase.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: stephane
 */

#include <Util/DataBase.h>
#include <QSqlError>
#include <QDebug>

namespace Util {

QSqlDatabase DataBase::defaultConnection;

DataBase::DataBase() {


}

bool DataBase::open(QJsonObject sessionConfiguration, QString database)
{
	if (!defaultConnection.isValid()) {
		defaultConnection = QSqlDatabase::addDatabase("QMYSQL");
	}

	QString hostName = sessionConfiguration.value("hostname").toString();
	QString userName = sessionConfiguration.value("user").toString();
	int port = sessionConfiguration.value("port").toInt();

    if (!defaultConnection.isOpen() || hostName != defaultConnection.hostName() || defaultConnection.userName() != userName || port != defaultConnection.port() || (database != "" && database != defaultConnection.databaseName())) {

		if (defaultConnection.isOpen()) {
			defaultConnection.close();
		}

		defaultConnection.setHostName(hostName);
		defaultConnection.setUserName(userName);
		defaultConnection.setDatabaseName(database);
		defaultConnection.setPassword(sessionConfiguration.value("password").toString());
		defaultConnection.setPort(port);

		if (!defaultConnection.open()) {
			qDebug() << "DataBase::open - " + defaultConnection.lastError().text();
			return false;
		}

		qInfo() << "Open database connection: " + defaultConnection.userName() + "@" + defaultConnection.hostName() + ":" + QString::number(defaultConnection.port()) + "/" + defaultConnection.databaseName();
	}

	return true;
}

DataBase::~DataBase() {
}

} /* namespace Util */
