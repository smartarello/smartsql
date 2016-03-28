/*
 * QueryModel.cpp
 *
 *  Created on: Mar 14, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Query/QueryModel.h>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QFont>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryModel::QueryModel(QObject * parent) : QSqlQueryModel(parent) {


}


QVariant QueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if (value.isNull() && role == Qt::DisplayRole) {
            return QVariant("(NULL)");
    } else if (role == Qt::FontRole) {

        QVariant data = QSqlQueryModel::data(index, Qt::DisplayRole);
        if (data.isNull()) {
            QFont font("Courier");
            font.setItalic(true);
            return QVariant(font);
        }
    }


    return value;
}

QueryModel::~QueryModel() {

}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
