#include "ServerTab.h"
#include <QSqlQueryModel>
#include <QHeaderView>

ServerTab::ServerTab(QWidget *parent) : QTableView(parent)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SHOW DATABASES");

    this->setModel(model);
    this->resizeColumnsToContents();
    this->verticalHeader()->hide();
}

ServerTab::~ServerTab() {
    // TODO Auto-generated destructor stub
}
