#include "TableDetailsWindow.h"
#include <QVBoxLayout>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QTabWidget>
#include <QTextEdit>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "../Tabs/SQLSyntaxHighlighter.h"
#include "TableDetailsModel.h"

TableDetailsWindow::TableDetailsWindow(QSqlDatabase database, QString tableName, QWidget *parent) : QMainWindow(parent)
{
        setWindowTitle(tableName);
        setMinimumSize(800, 400);

        QWidget *container = new QWidget(this);

        QTabWidget *tabWidget = new QTabWidget(container);

        QVBoxLayout *containerLayout = new QVBoxLayout(container);

        this->tableColumns = new QTableView(this);
        QString createString = "";

        database.open();
        QSqlQuery query(database);
        if (query.exec(QString("SHOW CREATE TABLE %1").arg(tableName))) {
            if (query.next()) {
                createString = query.value(1).toString();
                this->tableColumns->setModel(new TableDetailsModel(createString));
            }
        } else {
            qDebug() << "TableDetailsWindow::TableDetailsWindow - " + query.lastError().text();
        }

        this->tableColumns->horizontalHeader()->setStretchLastSection(true);
        this->tableColumns->setEditTriggers(QAbstractItemView::NoEditTriggers);

        tabWidget->addTab(this->tableColumns, tr("Columns"));

        createString = createString.replace("\n", "<br/>");
        QTextEdit *createTableTextEdit = new QTextEdit(createString);
        createTableTextEdit->setReadOnly(true);
        UI::Explorer::Tabs::SQLSyntaxHighlighter *highlighter  = new UI::Explorer::Tabs::SQLSyntaxHighlighter(createTableTextEdit->document());
        tabWidget->addTab(createTableTextEdit, tr("Create table"));

        containerLayout->addWidget(tabWidget);
        this->setCentralWidget(container);
}
