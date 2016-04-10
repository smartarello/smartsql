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

#include <UI/Explorer/ServerAction/ShowProcessesWindow.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QUuid>
#include <QModelIndex>
#include <QModelIndexList>
#include <QIcon>
#include <QKeySequence>
#include <QDebug>
#include <QShortcut>
#include <QPushButton>
#include <QAbstractItemView>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>

namespace UI {
namespace Explorer {
namespace ServerAction {

ShowProcessesWindow::ShowProcessesWindow(QJsonObject sessionConf, QWidget * parent) : QMainWindow(parent) {

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->database = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());

    this->database.setHostName(sessionConf.value("hostname").toString());
    this->database.setUserName(sessionConf.value("user").toString());
    this->database.setPassword(sessionConf.value("password").toString());
    this->database.setPort(sessionConf.value("port").toInt());

	this->setMinimumSize(900, 300);	

    // Main container
    QWidget *container = new QWidget(this);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);

    // Button part
    QWidget *buttonContainer = new QWidget(container);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->setAlignment(Qt::AlignLeft);

    QPushButton *killButton = new QPushButton();
    killButton->setToolTip(tr("Kill process(es)"));
    killButton->setFixedSize(40, 40);
    killButton->setIcon(QIcon(":/resources/icons/stop.png"));
    killButton->setIconSize(QSize(30, 30));

    QPushButton *refreshButton = new QPushButton();
    refreshButton->setToolTip(tr("Refresh (F5)"));
    refreshButton->setFixedSize(40, 40);
    refreshButton->setIcon(QIcon(":/resources/icons/refresh-icon.png"));
    refreshButton->setIconSize(QSize(30, 30));


    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(killButton);

    // Table view with the process list
	this->processListTable = new QTableView(this);
    this->processListTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->processListTable->horizontalHeader()->stretchLastSection();

    this->processListTable->verticalHeader()->hide();
	QSqlQueryModel *model = new QSqlQueryModel();
	this->processListTable->setModel(model);

    containerLayout->addWidget(buttonContainer);
	containerLayout->addWidget(this->processListTable);

	this->setCentralWidget(container);

    this->worker = new ProcessListThread(this->database);
	connect(this->worker, SIGNAL(processListReady()), SLOT(processListReady()));
	this->worker->start();

    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_F5), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(refreshProcess()));

    connect(killButton, SIGNAL (released()), SLOT(killProcess()));
    connect(refreshButton, SIGNAL (released()), SLOT(refreshProcess()));
}

void ShowProcessesWindow::refreshProcess()
{
    if (!this->worker->isRunning()) {
         this->worker->start();
    }
}

void ShowProcessesWindow::killProcess()
{
    QModelIndexList selectedIndexes = this->processListTable->selectionModel()->selectedRows();
    foreach (QModelIndex idx, selectedIndexes) {
        QVariant data = this->processListTable->model()->data(idx);
        QSqlQuery query(this->database);
        if (query.exec(QString("KILL %1").arg(data.toInt()))) {
            this->refreshProcess();
        } else {
            qWarning() << "ShowProcessesWindow::killProcess - " + query.lastError().text();
        }
    }
}

void ShowProcessesWindow::processListReady()
{
	((QSqlQueryModel *)this->processListTable->model())->setQuery(this->worker->getQuery());
}

ShowProcessesWindow::~ShowProcessesWindow() {
    this->database.close();
    delete this->worker;
    delete this->processListTable;
}

} /* namespace ServerAction */
} /* namespace Explorer */
} /* namespace UI */
