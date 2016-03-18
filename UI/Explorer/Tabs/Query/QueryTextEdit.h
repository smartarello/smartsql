/*
 * QueryTextEdit.h
 *
 *  Created on: Mar 12, 2016
 *      Author: stephane
 */

#ifndef UI_EXPLORER_TABS_QUERY_QUERYTEXTEDIT_H_
#define UI_EXPLORER_TABS_QUERY_QUERYTEXTEDIT_H_

#include <qtextedit.h>
#include <QCompleter>
#include <QStringListModel>

namespace UI {
namespace Explorer {
namespace Tabs {
namespace Query {

class QueryTextEdit: public QTextEdit {

	Q_OBJECT

public:
	QueryTextEdit(QWidget *parent = 0);
	virtual ~QueryTextEdit();

signals:
	void queryChanged();

protected:
	void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

private slots:
	void insertCompletion(const QString &completion);

public slots:
		void databaseChanged();

private:
	QString textUnderCursor() const;
	QStringList getTableList();
	QCompleter *autocomplete;
	QStringList tableList;
	QStringListModel *autoCompleteModel;
	void loadTableFields();
};

} /* namespace Query */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */

#endif /* UI_EXPLORER_TABS_QUERY_QUERYTEXTEDIT_H_ */
