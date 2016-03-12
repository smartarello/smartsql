/*
 * QueryTextEdit.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Query/QueryTextEdit.h>
#include "../SQLSyntaxHighlighter.h"
#include <QFont>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryTextEdit::QueryTextEdit(QWidget *parent) : QTextEdit(parent) {

	QFont font = QFont("Monospace");
	font.setPointSize(12);
	this->setFont(font);

	SQLSyntaxHighlighter *highlighter  = new SQLSyntaxHighlighter(this->document());
}

QueryTextEdit::~QueryTextEdit() {
	// TODO Auto-generated destructor stub
}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
