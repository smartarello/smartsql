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
#include <QTreeView>
#include <QMessageBox>

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
                setMinimumSize(800, 300);

                QWidget *mainContainer = new QWidget(this);
                QHBoxLayout *mainContainerLayout = new QHBoxLayout(mainContainer);
                mainContainerLayout->setContentsMargins(20, 20, 20, 20);
                mainContainerLayout->setSpacing(0);


                // LEFT PART: the table list with checkbox
                QTreeView *tableList = new QTreeView(this);
                tableList->setMinimumWidth(250);
                tableList->setHeaderHidden(true);
                tableList->setEditTriggers(QAbstractItemView::NoEditTriggers);


                // Create model
                this->model = new QStandardItemModel(this);
                QSqlDatabase db = Util::DataBase::createFromConfig(conf);
                if (db.open()) {

                    QStringList tables = db.tables();
                    QStandardItem *rootItem = this->model->invisibleRootItem();

                    QStandardItem *dbItem = new QStandardItem(db.databaseName());
                    dbItem->setCheckable(true);
                    if (table.isEmpty()) {
                        dbItem->setCheckState(Qt::Checked);
                    }

                    rootItem->appendRow(dbItem);

                    foreach (QString tableName, tables) {
                        QStandardItem *item = new QStandardItem(tableName);
                        item->setCheckable(true);
                        if (table.isEmpty() || table == tableName) {
                            item->setCheckState(Qt::Checked);
                        }

                        dbItem->appendRow(item);
                    }

                    db.close();
                }

                tableList->setModel(this->model);
                tableList->expandAll();
                mainContainerLayout->addWidget(tableList);

                // RIGHT PART: the configuration
                QWidget *rightPartContainer = new QWidget(this);
                QVBoxLayout *rightPartLayout = new QVBoxLayout(rightPartContainer);
                rightPartLayout->setContentsMargins(20, 0, 0, 0);
                rightPartLayout->setSpacing(0);

                // Database options
                QLabel *labelDatabase = new QLabel(tr("Database"), rightPartContainer);
                QFont font;
                font.setBold(true);
                labelDatabase->setFont(font);
                rightPartLayout->addWidget(labelDatabase);

                QWidget *databaseCheckboxContainer = new QWidget(rightPartContainer);
                QHBoxLayout *databaseCheckboxLayout = new QHBoxLayout(databaseCheckboxContainer);
                databaseCheckboxLayout->setContentsMargins(30, 5, 0, 10);
                databaseCheckboxLayout->setAlignment(Qt::AlignLeft);
                databaseCreateCheckbox = new QCheckBox(tr("Create"), databaseCheckboxContainer);
                databaseDropCheckbox = new QCheckBox(tr("Drop"), databaseCheckboxContainer);
                databaseCheckboxLayout->addWidget(databaseDropCheckbox);
                databaseCheckboxLayout->addWidget(databaseCreateCheckbox);

                rightPartLayout->addWidget(databaseCheckboxContainer);


                // Table options
                QLabel *labelTable = new QLabel(tr("Table(s)"), rightPartContainer);
                labelTable->setFont(font);
                rightPartLayout->addWidget(labelTable);

                QWidget *tableCheckboxContainer = new QWidget(rightPartContainer);
                QHBoxLayout *tableCheckboxLayout = new QHBoxLayout(tableCheckboxContainer);
                tableCheckboxLayout->setContentsMargins(30, 5, 0, 10);
                tableCheckboxLayout->setAlignment(Qt::AlignLeft);
                tableCreateCheckbox = new QCheckBox(tr("Create"), tableCheckboxContainer);
                tableDropCheckbox = new QCheckBox(tr("Drop"), tableCheckboxContainer);
                tableCheckboxLayout->addWidget(tableDropCheckbox);
                tableCheckboxLayout->addWidget(tableCreateCheckbox);

                rightPartLayout->addWidget(tableCheckboxContainer);

                // Export format
                QLabel *labelData = new QLabel(tr("Data"), rightPartContainer);
                labelData->setFont(font);
                rightPartLayout->addWidget(labelData);

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
                rightPartLayout->addWidget(radioButtonContainer);

                // File selection
                QLabel *fileSelectionLabel = new QLabel(tr("Filename"), this);
                fileSelectionLabel->setFont(font);
                rightPartLayout->addWidget(fileSelectionLabel);

                QWidget *fileSelectionContainer = new QWidget(this);
                QHBoxLayout *fileSelectionLayout = new QHBoxLayout(fileSelectionContainer);
                fileSelectionLayout->setContentsMargins(30, 5, 0, 0);

                this->filePath = new QLineEdit(this);
                this->filePath->setText(QDir::currentPath()+"/export.sql");
                QPushButton *browseButton = new QPushButton(tr("browse..."), this);


                fileSelectionLayout->addWidget(filePath);
                fileSelectionLayout->addWidget(browseButton);

                rightPartLayout->addWidget(fileSelectionContainer);


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

                rightPartLayout->addWidget(buttonContainer);

                progressbarContainer = new QWidget(this);
                QVBoxLayout *progressbarLayout = new QVBoxLayout(progressbarContainer);
                progressbarLayout->setContentsMargins(0, 30, 0, 0);
                progressLabel = new QLabel(this);
                progressbar = new QProgressBar(this);
                progressbar->setMinimumWidth(300);
                progressbarLayout->addWidget(progressLabel, 0, Qt::AlignCenter);
                progressbarLayout->addWidget(progressbar, 0, Qt::AlignCenter);
                rightPartLayout->addWidget(progressbarContainer);
                progressbarContainer->hide();



                mainContainerLayout->addWidget(rightPartContainer);
                this->setCentralWidget(mainContainer);

                // Events
                connect(browseButton, SIGNAL(released()), SLOT(handleBrowseFile()));
                connect(closeButton, SIGNAL(released()), SLOT(handleClose()));
                connect(this->exportButton, SIGNAL(released()), SLOT(handleExport()));
                connect(this->stopButton, SIGNAL(released()), SLOT(handleStop()));
                connect(this->filePath, SIGNAL (textEdited(QString)), SLOT (handleFilePathEdit(QString)));
                connect(tableList, SIGNAL(clicked(QModelIndex)), SLOT(databaseTreeClicked(QModelIndex)));
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
                this->handleStop();
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

                QStandardItem *databaseItem = this->model->invisibleRootItem()->child(0);
                if (databaseItem->checkState() != Qt::Checked) {
                    dumpWorker->setTables(this->getSelectedTables());
                }

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
                connect(dumpWorker, SIGNAL(dumpFinished(bool)), SLOT(handleDumpFinished(bool)));
                connect(this->timer, SIGNAL(timeout()), SLOT(handleTimer()));

                workerThread->start();

                this->progressbar->setMinimum(0);
                this->progressbar->reset();                
                this->progressLabel->setText("");
                this->progressbarContainer->show();

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
                    QString label = table;
                    if (totalLine > 0) {
                        label += ": "+QLocale(QLocale::English).toString(tableProgress)+"/"+QLocale(QLocale::English).toString(totalLine);
                    }

                    this->progressLabel->setText(label);

                    this->progressbar->setMaximum(totalLine);
                    this->progressbar->setValue(tableProgress);
                }               
            }

            void ExportWindow::handleDumpFinished(bool stopped)
            {
                if (!stopped) {
                    QMessageBox::information(this, "", tr("Export completed successfully"));
                }

                this->progressbarContainer->hide();
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
                if (this->workerThread != nullptr) {
                    this->workerThread->quit();
                }
            }

            void ExportWindow::databaseTreeClicked(QModelIndex index)
            {
                if (!index.parent().isValid()) {
                    // Click on the database
                    QStandardItem *databaseItem = this->model->itemFromIndex(index);

                    for (int i = 0 ; i < databaseItem->rowCount() ; ++i) {
                        QStandardItem* child = databaseItem->child(i);
                        child->setCheckState(databaseItem->checkState());
                    }
                } else {
                    // Click on a table
                    QStandardItem *tableItem = this->model->itemFromIndex(index);
                    if (tableItem->checkState() != Qt::Checked) {
                        // The table is not checked, the database should not be checked
                        QStandardItem *databaseItem = this->model->invisibleRootItem()->child(0);
                        databaseItem->setCheckState(Qt::Unchecked);
                    }
                }
            }

            QStringList ExportWindow::getSelectedTables()
            {
                QStringList tables;
                QStandardItem *databaseItem = this->model->invisibleRootItem()->child(0);

                for (int i = 0 ; i < databaseItem->rowCount() ; ++i) {
                    QStandardItem* child = databaseItem->child(i);

                    if (child->checkState() == Qt::Checked) {
                        tables << child->text();
                    }
                }

                return tables;
            }
        }
    }
}

