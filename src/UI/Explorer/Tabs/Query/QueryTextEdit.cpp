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

#include <UI/Explorer/Tabs/Query/QueryTextEdit.h>
#include "../SQLSyntaxHighlighter.h"
#include <QFont>
#include <QSqlQuery>
#include <QTextCursor>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QSqlError>
#include <QShortcut>
#include <QKeySequence>


namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

QueryTextEdit::QueryTextEdit(QWidget *parent) : QTextEdit(parent) {

    this->setFontFamily("DejaVu Sans Mono");
    this->setAcceptRichText(false);


    SQLSyntaxHighlighter *highlighter  = new SQLSyntaxHighlighter(this->document());
	this->tableList = this->getTableList();

	this->autocomplete = new QCompleter(this);
	this->autocomplete->setCaseSensitivity(Qt::CaseInsensitive);
	this->autocomplete->setWrapAround(false);
	this->autocomplete->setWidget(this);
	this->autocomplete->setCompletionMode(QCompleter::PopupCompletion);

	this->autoCompleteModel = new QStringListModel(this->tableList, this->autocomplete);
	this->autocomplete->setModel(this->autoCompleteModel);


    QShortcut* refreshShortcut = new QShortcut(QKeySequence("Ctrl+Shift+F"), this);
    refreshShortcut->setContext(Qt::WidgetShortcut);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(showContextMenu(const QPoint &)));

    connect(refreshShortcut, SIGNAL(activated()), SLOT(formatSql()));
    connect(this->autocomplete, SIGNAL(activated(QString)), SLOT(insertCompletion(QString)));
}

void QueryTextEdit::showContextMenu(const QPoint &pt)
{
    QMenu *menu = this->createStandardContextMenu();

    menu->addSeparator();

    QAction *formatAction = new QAction(QString(tr("Reformat SQL")), this);
    formatAction->setShortcut(QKeySequence("Ctrl+Shift+F"));
    menu->addAction(formatAction);

    connect(formatAction, SIGNAL(triggered(bool)), SLOT(formatSql()));

    menu->exec(this->mapToGlobal(pt));
    delete menu;
}

/**
 * Formats the SQL query
 * @brief QueryTextEdit::formatSql
 */
void QueryTextEdit::formatSql()
{
    QString sql = this->toPlainText();

    QStringList keywords ;
    keywords << "SELECT";
    keywords << "INNER JOIN ";
    keywords << "LEFT JOIN ";
    keywords << "FROM ";
    keywords << "WHERE ";
    keywords << "GROUP BY ";
    keywords << "ORDER BY ";
    keywords << "LIMIT ";
    keywords << "UNION ";

    QStringList secondaryKeywords;
    secondaryKeywords << " AND ";
    secondaryKeywords << " OR ";

    QStringList lines = sql.split("\n");

    QString formatedSql = "";
    QString commentPart = "";

    QRegExp commentRegExp("-- ");

    foreach(QString line, lines) {

        int commentIdx = line.indexOf(commentRegExp);
        if (commentIdx != -1) {
            commentPart = line.mid(commentIdx);
            line = line.left(commentIdx);

        } else {
            commentPart = "";
        }

        foreach (QString keyword, keywords) {
            line = line.replace(QRegExp(keyword, Qt::CaseInsensitive), QString("\n%1").arg(keyword));
        }

        foreach (QString keyword, secondaryKeywords) {
            line = line.replace(keyword, QString("\n   %1 ").arg(keyword), Qt::CaseInsensitive);
        }

        formatedSql += "\n" + line + commentPart;
    }


    QRegExp regExp("(\n)+");
    formatedSql = formatedSql.replace(regExp, "\n");

    formatedSql = formatedSql.trimmed();

    this->setText(formatedSql);
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

    if (isShortcut || this->autocomplete->popup()->isVisible()) {

        if (completionPrefix != this->autocomplete->completionPrefix()) {
            this->autocomplete->setCompletionPrefix(completionPrefix);
            this->autocomplete->popup()->setCurrentIndex(this->autocomplete->completionModel()->index(0, 0));
        }

        QRect cr = cursorRect();
        cr.setWidth(this->autocomplete->popup()->sizeHintForColumn(0)
                    + this->autocomplete->popup()->verticalScrollBar()->sizeHint().width());

        this->autocomplete->complete(cr);
    }
}

QString QueryTextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);

    return tc.selectedText();
}

QStringList QueryTextEdit::getTableList()
{
    QSqlDatabase db = QSqlDatabase::database();

	QStringList tables;

    if (db.isValid() && db.isOpen() && db.databaseName() != "") {
        QSqlQuery query;
        if (query.exec("show table status")) {
            while (query.next()){
                    tables << query.value("Name").toString();
                }
        } else {
            qDebug() << "QueryTextEdit::getTableList - " + query.lastError().text();
        }
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
		if (query.exec(queryString)) {

			QStringList columns;

			while (query.next()){
				columns << query.value("Field").toString();
			}

			this->autoCompleteModel->setStringList(columns);
		} else {
			qDebug() << "QueryTextEdit::loadTableFields - " + query.lastError().text();
		}
	}
}

QueryTextEdit::~QueryTextEdit() {
}

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
