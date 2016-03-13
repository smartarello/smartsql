/*
 * SQLSyntaxHighlighter.cpp
 *
 *  Created on: Mar 10, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/SQLSyntaxHighlighter.h>

namespace UI {
namespace Explorer {
namespace Tabs {

SQLSyntaxHighlighter::SQLSyntaxHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent){

	HighlightingRule rule;

	QTextCharFormat keywordFormat;
	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);

	QStringList keywordPatterns;
	keywordPatterns << "\\bselect\\b"
			<< "\\bfrom\\b"
			<< "\\bon\\b"
			<< "\\blimit\\b"
			<< "\\border\\b"
			<< "\\bby\\b"
			<< "\\basc\\b"
			<< "\\bdesc\\b"
			<< "\\bwhere\\b"
			<< "\\band\\b"
			<< "\\bor\\b"
			<< "\\bin\\b"
			<< "\\binsert\\b"
			<< "\\bupdate\\b"
			<< "\\bjoin\\b"
			<< "\\bcreate\\b"
			<< "\\btable\\b"
			<< "\\bexplain\\b"
			<< "\\bleft\\b"
			<< "\\binner\\b" ;

	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern, Qt::CaseInsensitive);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

}

void SQLSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            this->setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}

SQLSyntaxHighlighter::~SQLSyntaxHighlighter() {

}

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
