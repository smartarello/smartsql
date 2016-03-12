/*
 * TableFilterTextEdit.cpp
 *
 *  Created on: Mar 9, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Table/TableFilterTextEdit.h>
#include <QTextCursor>
#include <QStringListModel>
#include <qabstractitemview.h>
#include <QScrollBar>
#include <QDebug>
#include <QFont>
#include <QTextDocument>
#include "../SQLSyntaxHighlighter.h"

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Table {

TableFilterTextEdit::TableFilterTextEdit(QWidget *parent) : QTextEdit(parent) {

	QFont font = QFont("Monospace");
	font.setPointSize(12);
	this->setFont(font);

	SQLSyntaxHighlighter *highlighter  = new SQLSyntaxHighlighter(this->document());

	this->autocomplete = new QCompleter(this);
	this->autocomplete->setCaseSensitivity(Qt::CaseInsensitive);
	this->autocomplete->setWrapAround(false);
	this->autocomplete->setWidget(this);
	this->autocomplete->setCompletionMode(QCompleter::PopupCompletion);

	QObject::connect(this->autocomplete, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));

}

QCompleter *TableFilterTextEdit::getAutocomplete()
{
    return this->autocomplete;
}

void TableFilterTextEdit::insertCompletion(const QString& completion)
{
    QTextCursor tc = this->textCursor();
    int extra = completion.length() - this->autocomplete->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    this->setTextCursor(tc);
}

void TableFilterTextEdit::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_F5){
		emit filterChanged(this->toPlainText());
		return;
	}

	if (this->autocomplete->popup()->isVisible()) {
		// The following keys are forwarded by the completer to the widget
		switch (e->key()) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Escape:
		case Qt::Key_Tab:
		case Qt::Key_Backtab:
			e->ignore();
			return; // let the completer do default behavior
		default:
			break;
		}
	}

	bool isShortcut = ((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_Space));
	if (!isShortcut){ // do not process the shortcut when we have a completer
		QTextEdit::keyPressEvent(e);
	}

	const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);

	static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
	bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
	QString completionPrefix = this->textUnderCursor();

	if (!isShortcut && (hasModifier || e->text().isEmpty() || completionPrefix.length() < 3
			|| eow.contains(e->text().right(1)))) {
		this->autocomplete->popup()->hide();
		return;
	}

	if (completionPrefix != this->autocomplete->completionPrefix()) {
		this->autocomplete->setCompletionPrefix(completionPrefix);
		this->autocomplete->popup()->setCurrentIndex(this->autocomplete->completionModel()->index(0, 0));
	}

	QRect cr = cursorRect();
	cr.setWidth(this->autocomplete->popup()->sizeHintForColumn(0)
			+ this->autocomplete->popup()->verticalScrollBar()->sizeHint().width());

	this->autocomplete->complete(cr);
}

QString TableFilterTextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

TableFilterTextEdit::~TableFilterTextEdit() {

}

} /* namespace Table */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
