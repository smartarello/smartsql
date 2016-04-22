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
#ifndef EXPORTWINDOW_H
#define EXPORTWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QThread>
#include <QProgressBar>
#include <QTimer>
#include <QLabel>
#include <QModelIndex>
#include <QStandardItemModel>
#include "Util/DataBase.h"
#include "Util/MySQLDump.h"
namespace UI {
    namespace Explorer {
        namespace Export {
            class ExportWindow : public QMainWindow
            {
                Q_OBJECT
            public:
                explicit ExportWindow(QWidget *parent, ConnectionConfiguration conf, QString tableName);
                virtual ~ExportWindow();

            private:
                QThread *workerThread = nullptr;
                QPushButton *exportButton;
                QPushButton *stopButton;
                QLineEdit *filePath;
                QLabel *progressLabel;
                ConnectionConfiguration connectionConf;
                QString tableName;
                QCheckBox *databaseCreateCheckbox, *databaseDropCheckbox, *tableCreateCheckbox, *tableDropCheckbox;
                QRadioButton *deleteAndInsert, *insert, *insertIgnore, *replace;
                QProgressBar *progressbar;
                QTimer *timer;
                Util::MySQLDump *dumpWorker;
                QStandardItemModel *model;
                QWidget *progressbarContainer;

                QStringList getSelectedTables();

            signals:
                void startDump();

            public slots:
                void handleBrowseFile();
                void handleExport();
                void handleStop();
                void handleClose();
                void handleFilePathEdit(QString value);
                void handleDumpFinished(bool stopped);
                void handleTimer();
                void databaseTreeClicked(QModelIndex index);
            };
        }
    }
}
#endif // EXPORTWINDOW_H
