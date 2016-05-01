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
#include "InsertWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QDateTimeEdit>
#include <QDateEdit>
#include <QCheckBox>
#include <QSqlDatabase>
#include <QSqlDriver>
#include "UI/Explorer/Tabs/SQLSyntaxHighlighter.h"


namespace UI {
namespace Explorer {
namespace Tabs {
namespace Table {
InsertWindow::InsertWindow(QSqlDatabase connection, Util::TableDefinition table, QWidget *parent) :
    connection(connection),
    QMainWindow(parent),
    table(table)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setMinimumWidth(600);

    QWidget *container = new QWidget(this);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);


    QList<ColumnDefinition> columns = table.columns();
    QWidget *fieldsContainer = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(fieldsContainer);
    gridLayout->setHorizontalSpacing(30);

    QLabel *label = new QLabel(tr("Column"), this);
    QLabel *type = new QLabel(tr("Type"), this);
    QLabel *isnull = new QLabel(tr("Null"), this);
    QLabel *value = new QLabel(tr("Value"), this);

    QFont font;
    font.setBold(true);
    label->setFont(font);
    isnull->setFont(font);
    type->setFont(font);
    value->setFont(font);

    gridLayout->addWidget(label, 0, 0);
    gridLayout->addWidget(type, 0, 1);
    gridLayout->addWidget(isnull, 0, 2);
    gridLayout->addWidget(value, 0, 3);

    int row = 1;
    foreach(ColumnDefinition column, columns) {

        QLabel *label = new QLabel(column.name, this);
        QLabel *type = new QLabel(column.type, this);

        gridLayout->addWidget(label, row, 0);
        gridLayout->addWidget(type, row, 1);

        QCheckBox *setNull = new QCheckBox(this);
        checkboxes << setNull;
        gridLayout->addWidget(setNull, row, 2);
        if (column.allowNull) {
            setNull->setChecked(true);
            connect(setNull, SIGNAL (stateChanged(int)), SLOT(refreshInsertStatement()));
        } else {
            setNull->setDisabled(true);
        }

        if (column.type.toLower() == "datetime") {
            QDateTimeEdit *datetime = new QDateTimeEdit(this);
            datetime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
            gridLayout->addWidget(datetime, row, 3);

            connect(datetime, SIGNAL (dateTimeChanged(const QDateTime &)), SLOT(refreshInsertStatement()));
            values << datetime;
        } else if (column.type.toLower() == "date") {
            QDateEdit *date = new QDateEdit(this);
            date->setDisplayFormat("yyyy-MM-dd");
            gridLayout->addWidget(date, row, 3);

            connect(date, SIGNAL (dateChanged(const QDate &)), SLOT(refreshInsertStatement()));
            values << date;
        } else {
            QLineEdit *edit = new QLineEdit(this);
            gridLayout->addWidget(edit, row, 3);
            connect(edit, SIGNAL (textEdited(QString)), SLOT(refreshInsertStatement()));

            values << edit;
        }

        row++;
    }

    insertStatement = new QTextEdit(this);
    insertStatement->setReadOnly(true);
    insertStatement->setMinimumHeight(200);
    new SQLSyntaxHighlighter(insertStatement->document());

    QWidget *buttonContainer = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->setAlignment(Qt::AlignRight);

    QPushButton *insertButton = new QPushButton(tr("Insert"), this);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);
    buttonLayout->addWidget(insertButton);
    buttonLayout->addWidget(cancelButton);

    containerLayout->addWidget(fieldsContainer);
    containerLayout->addWidget(insertStatement);
    containerLayout->addWidget(buttonContainer);

    this->setCentralWidget(container);

    connect(cancelButton, SIGNAL(clicked(bool)), this ,SLOT(close()));
    connect(insertButton, SIGNAL(clicked(bool)), this ,SLOT(handleInsert()));

    // initialize the INSERT statement
    this->refreshInsertStatement();
}

void InsertWindow::refreshInsertStatement()
{
    QString insertString = "INSERT INTO "+table.name();
    QStringList columnNames;
    foreach (ColumnDefinition column, table.columns()) {
        columnNames << "`" + column.name + "`";
    }

    insertString += "(" + columnNames.join(", ") + ")\nVALUES ";

    QStringList insertValues;
    for (int i = 0; i < values.size(); i++) {
        QCheckBox *checkbox = checkboxes.at(i);
        if (checkbox->isChecked()) {
            insertValues << "NULL";
        } else {
            ColumnDefinition column = table.columns().at(i);
            QString value;
            if (column.type.toLower() == "datetime") {
                QDateTimeEdit *datetime = (QDateTimeEdit *) values.at(i);
                value = datetime->text();
            } else if (column.type.toLower() == "date") {
                QDateEdit *date = (QDateEdit *)values.at(i);
                value = date->text();
            } else {
                QLineEdit *text = (QLineEdit *)values.at(i);
                value = text->text();
            }

            value = value.replace("'", "''");
            insertValues << "'"+value+"'";
        }
    }

    insertString += "(" + insertValues.join(", ") + ")";

    insertStatement->setText(insertString);
}

void InsertWindow::handleInsert()
{
    QSqlQuery insertQuery(this->connection);
    if (insertQuery.exec(insertStatement->toPlainText())) {
        this->close();
    } else {
        QMessageBox::critical(this, tr("Error"), insertQuery.lastError().text());
    }
}

} /* namespace Table */
} /* namespace Tabs */
} /* namespace Explorer */
} /* namespace UI */
