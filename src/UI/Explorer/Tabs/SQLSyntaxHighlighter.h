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

#ifndef UI_EXPLORER_TABS_SQLSYNTAXHIGHLIGHTER_H_
#define UI_EXPLORER_TABS_SQLSYNTAXHIGHLIGHTER_H_

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegExp>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QVector>

namespace UI {
namespace Explorer {
namespace Tabs {

class SQLSyntaxHighlighter: public QSyntaxHighlighter {

	Q_OBJECT

public:
	SQLSyntaxHighlighter(QTextDocument *parent = 0);
	virtual ~SQLSyntaxHighlighter();

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

};

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_SQLSYNTAXHIGHLIGHTER_H_ */
