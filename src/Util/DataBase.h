/*
 * DataBase.h
 *
 *  Created on: Mar 19, 2016
 *      Author: stephane
 */

#ifndef UTIL_DATABASE_H_
#define UTIL_DATABASE_H_

#include <QJsonObject>
#include <QSqlDatabase>

struct ConnectionConfiguration {
    QString hostname;
    QString username;
    QString password;
    QString databaseName;
    int port;
};

namespace Util {

class DataBase {
public:
	DataBase();
	virtual ~DataBase();

	static bool open(QJsonObject sessionConfiguration, QString database = "");
    static QSqlDatabase cloneCurrentConnection();
    static QSqlDatabase createFromConfig(ConnectionConfiguration config);
    static ConnectionConfiguration dumpConfiguration();

private:
	static QSqlDatabase defaultConnection;
};

} /* namespace Util */

#endif /* UTIL_DATABASE_H_ */
