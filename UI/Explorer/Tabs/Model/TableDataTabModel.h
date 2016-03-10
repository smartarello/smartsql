/*
 * TableDataTabModel.h
 *
 *  Created on: Mar 4, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_MODEL_TABLEDATATABMODEL_H_
#define UI_EXPLORER_TABS_MODEL_TABLEDATATABMODEL_H_

#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QObject>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Model {

class TableDataTabModel: public QSqlQueryModel {

	Q_OBJECT

public:
	TableDataTabModel(QObject * parent = 0);
	virtual ~TableDataTabModel();
	void setTable(QString table);
	QList<QString> getColumns();

public slots:
	void refreshWithFilter(QString filter);

private:
	QString table;
	QString filter;
	QString sortOrder;
	QList<QString> columns;
	void sort(int column, Qt::SortOrder order);
	QString buildQuery();
};

} /* namespace Model */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_MODEL_TABLEDATATABMODEL_H_ */
