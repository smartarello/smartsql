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
