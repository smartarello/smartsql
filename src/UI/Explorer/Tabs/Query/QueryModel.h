/*
 * QueryModel.h
 *
 *  Created on: Mar 14, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_QUERY_QUERYMODEL_H_
#define UI_EXPLORER_TABS_QUERY_QUERYMODEL_H_

#include <QAbstractTableModel>
#include <QSqlRecord>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

class QueryModel: public QAbstractTableModel {

	Q_OBJECT

public:
    QueryModel(QList<QSqlRecord> data, QObject * parent = 0);
	virtual ~QueryModel();
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QList<QSqlRecord> records;
};

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_QUERY_QUERYMODEL_H_ */
