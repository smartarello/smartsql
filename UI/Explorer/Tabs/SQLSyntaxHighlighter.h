/*
 * SQLSyntaxHighlighter.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stephane
 */

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
