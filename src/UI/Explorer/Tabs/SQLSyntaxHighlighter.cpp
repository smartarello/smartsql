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
    keywordFormat.setForeground(Qt::blue);
	keywordFormat.setFontWeight(QFont::Bold);
    keywordFormat.setFontFamily("DejaVu Sans Mono");

	QStringList keywordPatterns;
	keywordPatterns << "\\bselect\\b"
			<< "\\bfrom\\b"
			<< "\\bdelete\\b"
			<< "\\bon\\b"
			<< "\\blimit\\b"
			<< "\\bgroup\\b"
			<< "\\basc\\b"
			<< "\\bdesc\\b"
			<< "\\border\\b"
			<< "\\bby\\b"
			<< "\\basc\\b"
			<< "\\bdesc\\b"
			<< "\\bwhere\\b"
			<< "\\band\\b"
			<< "\\bor\\b"
			<< "\\bin\\b"
			<< "\\binsert\\b"
			<< "\\binto\\b"
			<< "\\bshow\\b"
			<< "\\bupdate\\b"
			<< "\\bset\\b"
			<< "\\bjoin\\b"
			<< "\\bcreate\\b"
			<< "\\btable\\b"
			<< "\\btables\\b"
			<< "\\bexplain\\b"
			<< "\\bleft\\b"
			<< "\\bis\\b"
            << "\\bnot\\b"
            << "\\bdefault\\b"
            << "\\bAUTO_INCREMENT\\b"
            << "\\bPRIMARY\\b"
            << "\\bKEY\\b"
            << "\\bENGINE\\b"
            << "\\bCHARSET\\b"
			<< "\\bnull\\b"
			<< "\\bdrop\\b"
			<< "\\bdatabase\\b"
			<< "\\binner\\b" ;

	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern, Qt::CaseInsensitive);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

    // Comments
    HighlightingRule commentRule;

    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::lightGray);
    commentFormat.setFontItalic(true);
    commentFormat.setFontFamily("DejaVu Sans Mono");

    commentRule.format = commentFormat;
    commentRule.pattern = QRegExp("-- .*");

    highlightingRules.append(commentRule);
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
