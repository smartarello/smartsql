/*
 * QueryTextEdit.h
 *
 *  Created on: Mar 12, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_QUERY_QUERYTEXTEDIT_H_
#define UI_EXPLORER_TABS_QUERY_QUERYTEXTEDIT_H_

#include <qtextedit.h>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

class QueryTextEdit: public QTextEdit {

	Q_OBJECT

public:
	QueryTextEdit(QWidget *parent = 0);
	virtual ~QueryTextEdit();
};

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_QUERY_QUERYTEXTEDIT_H_ */
