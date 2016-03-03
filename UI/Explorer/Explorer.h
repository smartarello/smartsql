/*
 * Explorer.h
 *
 *  Created on: Mar 3, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_EXPLORER_H_
#define UI_EXPLORER_EXPLORER_H_

#include <QWidget>
#include <QLineEdit>
#include "DataBaseTree.h"

namespace UI {
namespace Explorer {

class Explorer: public QWidget {

	Q_OBJECT

public:
	Explorer(QWidget *parent = 0);
	virtual ~Explorer();

private:
	DataBaseTree *dataBaseTree;
	QLineEdit *tableFilterLineEdit;
};

} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_EXPLORER_H_ */
