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
#include "ExportWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include "Util/MySQLDump.h"

namespace UI {
    namespace Explorer {
        namespace Export {
            ExportWindow::ExportWindow(QWidget *parent, ConnectionConfiguration conf, QString table) :
                QMainWindow(parent),
                connectionConf(conf),
                tableName(table)
            {
                setWindowTitle(tr("Export database as SQL"));
                setMinimumSize(600, 300);

                QWidget *mainContainer = new QWidget(this);
                QVBoxLayout *mainLayout = new QVBoxLayout(mainContainer);
                mainLayout->setContentsMargins(20, 20, 20, 20);
                mainLayout->setSpacing(0);

                // Database options
                QLabel *labelDatabase = new QLabel(tr("Database"), mainContainer);
                QFont font;
                font.setBold(true);
                labelDatabase->setFont(font);
                mainLayout->addWidget(labelDatabase);

                QWidget *databaseCheckboxContainer = new QWidget(mainContainer);
                QHBoxLayout *databaseCheckboxLayout = new QHBoxLayout(databaseCheckboxContainer);
                databaseCheckboxLayout->setContentsMargins(30, 5, 0, 10);
                databaseCheckboxLayout->setAlignment(Qt::AlignLeft);
                databaseCreateCheckbox = new QCheckBox(tr("Create"), databaseCheckboxContainer);
                databaseDropCheckbox = new QCheckBox(tr("Drop"), databaseCheckboxContainer);
                databaseCheckboxLayout->addWidget(databaseDropCheckbox);
                databaseCheckboxLayout->addWidget(databaseCreateCheckbox);

                mainLayout->addWidget(databaseCheckboxContainer);


                // Table options
                QLabel *labelTable = new QLabel(tr("Table(s)"), mainContainer);
                labelTable->setFont(font);
                mainLayout->addWidget(labelTable);

                QWidget *tableCheckboxContainer = new QWidget(mainContainer);
                QHBoxLayout *tableCheckboxLayout = new QHBoxLayout(tableCheckboxContainer);
                tableCheckboxLayout->setContentsMargins(30, 5, 0, 10);
                tableCheckboxLayout->setAlignment(Qt::AlignLeft);
                tableCreateCheckbox = new QCheckBox(tr("Create"), tableCheckboxContainer);
                tableDropCheckbox = new QCheckBox(tr("Drop"), tableCheckboxContainer);
                tableCheckboxLayout->addWidget(tableDropCheckbox);
                tableCheckboxLayout->addWidget(tableCreateCheckbox);

                mainLayout->addWidget(tableCheckboxContainer);

                // Export format
                QLabel *labelData = new QLabel(tr("Data"), mainContainer);
                labelData->setFont(font);
                mainLayout->addWidget(labelData);

                QWidget *radioButtonContainer = new QWidget(this);
                QVBoxLayout *radioButtonLayout = new QVBoxLayout(radioButtonContainer);
                radioButtonLayout->setContentsMargins(30, 0, 0, 10);
                radioButtonLayout->setSpacing(0);

                deleteAndInsert = new QRadioButton("DELETE + INSERT (truncate existing data)", this);
                insert = new QRadioButton("INSERT", this);
                insertIgnore = new QRadioButton("INSERT IGNORE (do not update existing)", this);
                replace = new QRadioButton("REPLACE existing data", this);

                deleteAndInsert->setChecked(true);
                radioButtonLayout->addWidget(deleteAndInsert);
                radioButtonLayout->addWidget(insert);
                radioButtonLayout->addWidget(insertIgnore);
                radioButtonLayout->addWidget(replace);
                mainLayout->addWidget(radioButtonContainer);

                // File selection
                QLabel *fileSelectionLabel = new QLabel(tr("Filename"), this);
                fileSelectionLabel->setFont(font);
                mainLayout->addWidget(fileSelectionLabel);

                QWidget *fileSelectionContainer = new QWidget(this);
                QHBoxLayout *fileSelectionLayout = new QHBoxLayout(fileSelectionContainer);
                fileSelectionLayout->setContentsMargins(30, 5, 0, 0);

                this->filePath = new QLineEdit(this);
                this->filePath->setText(QDir::currentPath()+"/export.sql");
                QPushButton *browseButton = new QPushButton(tr("browse..."), this);


                fileSelectionLayout->addWidget(filePath);
                fileSelectionLayout->addWidget(browseButton);

                mainLayout->addWidget(fileSelectionContainer);


                QWidget *buttonContainer = new QWidget(this);
                QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
                this->exportButton = new QPushButton(tr("Export"), this);
                QPushButton *closeButton = new QPushButton(tr("Close"), this);
                buttonLayout->addWidget(this->exportButton, 0, Qt::AlignRight);
                buttonLayout->addWidget(closeButton, 0, Qt::AlignRight);
                buttonLayout->setAlignment(Qt::AlignRight);
                buttonLayout->setContentsMargins(0, 30, 0, 0);

                mainLayout->addWidget(buttonContainer);

                this->setCentralWidget(mainContainer);

                // Events
                connect(browseButton, SIGNAL(released()), SLOT(handleBrowseFile()));
                connect(closeButton, SIGNAL(released()), SLOT(handleClose()));
                connect(this->exportButton, SIGNAL(released()), SLOT(handleExport()));
                connect(this->filePath, SIGNAL (textEdited(QString)), SLOT (handleFilePathEdit(QString)));
            }

            void ExportWindow::handleBrowseFile()
            {
               QString file = QFileDialog::getSaveFileName(this, tr("Save File"));
                if (!file.isEmpty()) {
                    this->filePath->setText(file);
                    this->exportButton->setEnabled(true);
                }
            }

            void ExportWindow::handleFilePathEdit(QString value)
            {
                if (value.trimmed().isEmpty()) {
                    this->exportButton->setEnabled(false);
                } else {
                    this->exportButton->setEnabled(true);
                }
            }

            void ExportWindow::handleClose()
            {
               this->close();
                this->deleteLater();
            }

            void ExportWindow::handleExport()
            {
                QString filename = this->filePath->text().trimmed();
                if (filename.isEmpty()) {
                    return;
                }

                this->exportButton->setEnabled(false);

                Util::MySQLDump *dump = new Util::MySQLDump(this->connectionConf, filename);
                dump->setCreateDatabase(databaseCreateCheckbox->isChecked());
                dump->setDropDatabase(databaseDropCheckbox->isChecked());
                dump->setCreateTable(tableCreateCheckbox->isChecked());
                dump->setDropTable(tableDropCheckbox->isChecked());
                dump->setTable(this->tableName);

                if (insert->isChecked()) {
                    dump->setFormat(Util::MySQLDump::INSERT);
                } else if (deleteAndInsert->isChecked()) {
                    dump->setFormat(Util::MySQLDump::DELETE_AND_INSERT);
                } else if (insertIgnore->isChecked()) {
                    dump->setFormat(Util::MySQLDump::INSERT_IGNORE);
                } else {
                    dump->setFormat(Util::MySQLDump::REPLACE);
                }


                 this->workerThread = new QThread(this);
                 dump->moveToThread(workerThread);

                 connect(workerThread, &QThread::finished, dump, &QObject::deleteLater);
                 connect(this, SIGNAL(startDump()), dump, SLOT(dump()));
                 connect(dump, SIGNAL(dumpFinished()), this, SLOT(handleDumpFinished()));

                 workerThread->start();

                 emit startDump();
            }

            void ExportWindow::handleDumpFinished()
            {
                this->workerThread->quit();
                this->exportButton->setEnabled(true);
            }
        }
    }
}

