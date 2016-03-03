/*
 * DataBaseTree.h
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_DATABASETREE_H_
#define UI_EXPLORER_DATABASETREE_H_

#include <QTreeView>
#include "Model/DataBaseModel.h"

namespace UI {
namespace Explorer {

class DataBaseTree: public QTreeView {

	Q_OBJECT

public:
	DataBaseTree(QWidget *parent = 0);
	virtual ~DataBaseTree();
public slots:
	void filterTable(QString text);

private:
	Model::DataBaseModel *dataBaseModel;
};

} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_DATABASETREE_H_ */
