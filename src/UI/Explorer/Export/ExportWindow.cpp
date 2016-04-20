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
#include <QDebug>
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
                this->stopButton = new QPushButton(tr("Stop"), this);
                QPushButton *closeButton = new QPushButton(tr("Close"), this);
                buttonLayout->addWidget(this->exportButton, 0, Qt::AlignRight);
                buttonLayout->addWidget(this->stopButton, 0, Qt::AlignRight);
                buttonLayout->addWidget(closeButton, 0, Qt::AlignRight);
                buttonLayout->setAlignment(Qt::AlignRight);
                buttonLayout->setContentsMargins(0, 30, 0, 0);
                this->stopButton->hide();

                mainLayout->addWidget(buttonContainer);

                progressLabel = new QLabel(this);
                progressLabel->hide();
                progressbar = new QProgressBar(this);
                progressbar->setMinimumWidth(200);
                progressbar->hide();
                mainLayout->addWidget(progressLabel, 0, Qt::AlignCenter);
                mainLayout->addWidget(progressbar, 0, Qt::AlignCenter);

                this->setCentralWidget(mainContainer);

                // Events
                connect(browseButton, SIGNAL(released()), SLOT(handleBrowseFile()));
                connect(closeButton, SIGNAL(released()), SLOT(handleClose()));
                connect(this->exportButton, SIGNAL(released()), SLOT(handleExport()));
                connect(this->stopButton, SIGNAL(released()), SLOT(handleStop()));
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

                this->exportButton->hide();
                this->stopButton->show();

                dumpWorker = new Util::MySQLDump(this->connectionConf, filename);
                dumpWorker->setCreateDatabase(databaseCreateCheckbox->isChecked());
                dumpWorker->setDropDatabase(databaseDropCheckbox->isChecked());
                dumpWorker->setCreateTable(tableCreateCheckbox->isChecked());
                dumpWorker->setDropTable(tableDropCheckbox->isChecked());
                dumpWorker->setTable(this->tableName);

                if (insert->isChecked()) {
                    dumpWorker->setFormat(Util::MySQLDump::INSERT);
                } else if (deleteAndInsert->isChecked()) {
                    dumpWorker->setFormat(Util::MySQLDump::DELETE_AND_INSERT);
                } else if (insertIgnore->isChecked()) {
                    dumpWorker->setFormat(Util::MySQLDump::INSERT_IGNORE);
                } else {
                    dumpWorker->setFormat(Util::MySQLDump::REPLACE);
                }


                this->timer = new QTimer(this);
                this->workerThread = new QThread(this);
                dumpWorker->moveToThread(workerThread);

                connect(workerThread, &QThread::finished, dumpWorker, &QObject::deleteLater);
                connect(this, SIGNAL(startDump()), dumpWorker, SLOT(dump()));
                connect(dumpWorker, SIGNAL(dumpFinished()), SLOT(handleDumpFinished()));
                connect(this->timer, SIGNAL(timeout()), SLOT(handleTimer()));

                workerThread->start();

                this->progressbar->setMinimum(0);
                this->progressbar->reset();
                this->progressbar->show();
                this->progressLabel->setText("");
                this->progressLabel->show();

                this->timer->start(200);
                emit startDump();
            }

            void ExportWindow::handleTimer()
            {
                QString table = this->dumpWorker->getCurrentTable();
                int total = this->dumpWorker->getTableCount();
                int progress = this->dumpWorker->getProgress();
                if (!table.isEmpty()) {
                    int totalLine = this->dumpWorker->getTotalLine();
                    int tableProgress = this->dumpWorker->getProgressCurrentTable();
                    this->progressLabel->setText(table+": "+QString::number(tableProgress)+"/"+QString::number(totalLine));
                }

                this->progressbar->setMaximum(total);
                this->progressbar->setValue(progress);
            }

            void ExportWindow::handleDumpFinished()
            {
                this->progressLabel->hide();
                this->progressbar->hide();
                this->workerThread->quit();
                this->exportButton->show();
                this->stopButton->hide();
                this->timer->stop();
                delete this->timer;
            }

            void ExportWindow::handleStop()
            {
                this->dumpWorker->stopRequired();
            }


            ExportWindow::~ExportWindow()
            {
                this->workerThread->quit();
            }
        }
    }
}

