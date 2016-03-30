#include "TableDetailsWindow.h"
#include <QVBoxLayout>
#include <QAbstractItemView>
#include "TableDetailsModel.h"
#include "TableDetailsItemDelegate.h"

TableDetailsWindow::TableDetailsWindow(QSqlDatabase database, QString tableName, QWidget *parent) : QMainWindow(parent)
{
        setWindowTitle(tableName);
        setMinimumSize(800, 400);

        QWidget *container = new QWidget(this);
        QVBoxLayout *containerLayout = new QVBoxLayout(container);

        this->tableColumns = new QTableView(this);
        this->tableColumns->setModel(new TableDetailsModel(tableName, database));
        this->tableColumns->setItemDelegate(new TableDetailsItemDelegate());

        containerLayout->addWidget(this->tableColumns);

        this->setCentralWidget(container);
}
