/*
 * TableDataTabModel.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Model/TableDataTabModel.h>
#include <QSqlQuery>
namespace UI {
namespace Explorer {
namespace Tabs {
namespace Model {

TableDataTabModel::TableDataTabModel(QObject * parent) : QSqlTableModel(parent) {


}

//void TableDataTabModel::sort(int column, Qt::SortOrder order){
//	this->database().open();
//	QSqlTableModel::sort(column, order);
//	this->database().close();
//}

TableDataTabModel::~TableDataTabModel() {
	// TODO Auto-generated destructor stub
}

} /* namespace Model */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */