/*
 * QueryTextEdit.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: stephane
 */

#include <UI/Explorer/Tabs/Query/QueryTextEdit.h>
#include "../SQLSyntaxHighlighter.h"
#include <QFont>
#include <QSqlQuery>
#include <QTextCursor>
#include <QDebug>
#include <QAbstractItemView>
#include <QScrollBar>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryTextEdit::QueryTextEdit(QWidget *parent) : QTextEdit(parent) {

	QFont font = QFont("Monospace");
	font.setPointSize(12);
	this->setFont(font);

	SQLSyntaxHighlighter *highlighter  = new SQLSyntaxHighlighter(this->document());
	this->tableList = this->getTableList();

	this->autocomplete = new QCompleter(this);
	this->autocomplete->setCaseSensitivity(Qt::CaseInsensitive);
	this->autocomplete->setWrapAround(false);
	this->autocomplete->setWidget(this);
	this->autocomplete->setCompletionMode(QCompleter::PopupCompletion);

	this->autoCompleteModel = new QStringListModel(this->tableList, this->autocomplete);
	this->autocomplete->setModel(this->autoCompleteModel);

	QObject::connect(this->autocomplete, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
}

void QueryTextEdit::databaseChanged()
{
	// Refresh the table list from the new database
	this->tableList = this->getTableList();
	this->autoCompleteModel->setStringList(this->tableList);
}


void QueryTextEdit::insertCompletion(const QString& completion)
{
    QTextCursor tc = this->textCursor();
    int extra = completion.length() - this->autocomplete->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    this->setTextCursor(tc);
}

void QueryTextEdit::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_F5){
		emit queryChanged();
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

	QTextCursor tc = textCursor();
	tc.movePosition(QTextCursor::PreviousWord);
	tc.select(QTextCursor::WordUnderCursor);
	QString previousWord = tc.selectedText();

	tc.movePosition(QTextCursor::PreviousWord);
	tc.movePosition(QTextCursor::PreviousWord);
	tc.select(QTextCursor::WordUnderCursor);
	QString previousWord2 = tc.selectedText();

	if (previousWord == "." || previousWord2 == "."){
		this->loadTableFields();
	} else {
		this->autoCompleteModel->setStringList(this->tableList);
	}

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

QString QueryTextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);

    return tc.selectedText();
}

QStringList QueryTextEdit::getTableList()
{
	QSqlQuery query;
	query.exec("show table status");

	QStringList tables;

	while (query.next()){
		tables << query.value("Name").toString();
	}

	return tables;
}

void QueryTextEdit::loadTableFields()
{
	QTextCursor tc = textCursor();
	tc.movePosition(QTextCursor::PreviousWord);
	tc.movePosition(QTextCursor::PreviousWord);

	tc.select(QTextCursor::WordUnderCursor);
	QString tableName =  tc.selectedText();
	if (tableName == "."){
		tc.movePosition(QTextCursor::PreviousWord);
		tc.movePosition(QTextCursor::PreviousWord);
		tc.select(QTextCursor::WordUnderCursor);
		tableName =  tc.selectedText();
	}

	if (tableName.isNull() || tableName.isEmpty()){
		return ;
	}

	if (this->tableList.contains(tableName)){

		QString queryString = QString("SHOW COLUMNS FROM %1").arg(tableName);
		QSqlQuery query;
		query.exec(queryString);

		QStringList columns;

		while (query.next()){
			columns << query.value("Field").toString();
		}

		this->autoCompleteModel->setStringList(columns);
	}
}

QueryTextEdit::~QueryTextEdit() {
	// TODO Auto-generated destructor stub
}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
