/**
 * Copyright (C) 2016  Stéphane Martarello
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

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
            << "\\bFOREIGN\\b"
            << "\\bCONSTRAINT\\b"
            << "\\bCASCADE\\b"
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
