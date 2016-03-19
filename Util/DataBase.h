/*
 * DataBase.h
 *
 *  Created on: Mar 19, 2016
 *      Author: stephane
 */

#ifndef UTIL_DATABASE_H_
#define UTIL_DATABASE_H_

#include <QJsonObject>

namespace Util {

class DataBase {
public:
	DataBase();
	virtual ~DataBase();

	static bool open(QJsonObject sessionConfiguration);
};

} /* namespace Util */

#endif /* UTIL_DATABASE_H_ */
