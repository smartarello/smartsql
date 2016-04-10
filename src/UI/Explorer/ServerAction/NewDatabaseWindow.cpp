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
#include "NewDatabaseWindow.h"
#include <QFormLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPushButton>
#include <QStringListModel>


namespace UI {
namespace Explorer {
namespace ServerAction {


NewDatabaseWindow::NewDatabaseWindow(QWidget *parent) : QMainWindow(parent)
{
    // Search the default collation, MySQL server configuration
    QSqlQuery defaultCollationQuery;
    QString defaultCollation = "";
    if (defaultCollationQuery.exec("SHOW VARIABLES LIKE 'collation_server'")){
        if (defaultCollationQuery.next()) {
            defaultCollation = defaultCollationQuery.value("Value").toString();
        }
    } else {
        qDebug() << "NewDatabaseWindow::NewDatabaseWindow - " + defaultCollationQuery.lastError().text();
    }

    // Load the list of collations available
    QStringList collations;
    QSqlQuery query;
    int index = 0;
    int defaultIndex = -1;
    if (query.exec("SHOW COLLATION")) {
        while(query.next()) {
            QString currentCollation = query.value("Collation").toString();
            collations << currentCollation;
            if (currentCollation == defaultCollation) {
                defaultIndex = index;
            }

            index++;
        }
    } else {
        qDebug() << "NewDatabaseWindow::getCollationList - " + query.lastError().text();
    }

    QStringListModel *model = new QStringListModel(collations);

    setWindowTitle(tr("Create database..."));
    setFixedSize(500, 150);
    QWidget *formWidget = new QWidget;

    this->nameLineEdit = new QLineEdit();
    this->collationComboBox = new QComboBox();
    QPushButton *createButton = new QPushButton(tr("OK"));
    createButton->setFixedWidth(100);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setFixedWidth(100);

    collationComboBox->setModel(model);
    if (defaultIndex != -1) {
        collationComboBox->setCurrentIndex(defaultIndex);
    } else {
        collationComboBox->setCurrentIndex(0);
    }

    // Sort the collations by name
     model->sort(0);

     QFormLayout *formLayout = new QFormLayout;
     formLayout->addRow(tr("Name:"), nameLineEdit);
     formLayout->addRow(tr("Collation:"), collationComboBox);

     QWidget *buttonContainer = new QWidget();
     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonContainer->setLayout(buttonLayout);
     buttonLayout->addWidget(createButton);
     buttonLayout->addWidget(cancelButton);
     buttonLayout->setAlignment(Qt::AlignRight);
     formLayout->addWidget(buttonContainer);

     formWidget->setLayout(formLayout);

     this->setCentralWidget(formWidget);

     // Events
     connect(cancelButton, SIGNAL(clicked(bool)), this ,SLOT(handleCancelButton()));
     connect(createButton, SIGNAL(clicked(bool)), this ,SLOT(handleCreateButton()));
     connect(this->nameLineEdit, SIGNAL(returnPressed()), this, SLOT(handleCreateButton()));
}

void NewDatabaseWindow::handleCancelButton()
{
    this->close();
}

void NewDatabaseWindow::handleCreateButton()
{
    this->close();
    emit createDatabase(this->nameLineEdit->text(), this->collationComboBox->currentText());
}

}
}
}
