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

QSqlDatabase DataBase::createFromJSON(QJsonObject config) {
    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());

    database.setHostName(config.value("hostname").toString());
    database.setUserName(config.value("user").toString());
    database.setPassword(config.value("password").toString());
    database.setPort(config.value("port").toInt());

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
