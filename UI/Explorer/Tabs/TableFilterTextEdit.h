/*
 * TableFilterTextEdit.h
 *
 *  Created on: Mar 9, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_TABLEFILTERTEXTEDIT_H_
#define UI_EXPLORER_TABS_TABLEFILTERTEXTEDIT_H_

#include <qtextedit.h>
#include <QWidget>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QCompleter>

namespace UI {
namespace Explorer {
namespace Tabs {

class TableFilterTextEdit: public QTextEdit {

	Q_OBJECT

public:
	TableFilterTextEdit(QWidget *parent = 0);
	virtual ~TableFilterTextEdit();
	QCompleter *getAutocomplete();

signals:
	    void filterChanged(QString filter);

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

private slots:
    void insertCompletion(const QString &completion);

private:
    QString textUnderCursor() const;

private:
    QCompleter *autocomplete;
};

} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_TABLEFILTERTEXTEDIT_H_ */
