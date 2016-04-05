/*
 * DataBase.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: stephane
 */

#include <Util/DataBase.h>
#include <QSqlError>
#include <QDebug>
#include <QUuid>

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

QSqlDatabase DataBase::cloneCurrentConnection()
{
    QSqlDatabase db;
    if (defaultConnection.isValid()) {
        db = QSqlDatabase::cloneDatabase(defaultConnection, QUuid::createUuid().toString());
    }

    return db;
}

QSqlDatabase DataBase::createFromConfig(ConnectionConfiguration config) {
    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
    database.setHostName(config.hostname);
    database.setUserName(config.username);
    database.setPassword(config.password);
    database.setDatabaseName(config.databaseName);
    database.setPort(config.port);

    return database;
}

ConnectionConfiguration DataBase::dumpConfiguration()
{
    QSqlDatabase db = QSqlDatabase::database();
    ConnectionConfiguration conf;
    conf.hostname = db.hostName();
    conf.username = db.userName();
    conf.password = db.password();
    conf.databaseName = db.databaseName();
    conf.port = db.port();

    return conf;
}

DataBase::~DataBase() {
}

} /* namespace Util */
