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
#include "Util/DataBase.h"
namespace UI {
    namespace Explorer {
        namespace Export {
            class ExportWindow : public QMainWindow
            {
                Q_OBJECT
            public:
                explicit ExportWindow(QWidget *parent, ConnectionConfiguration conf, QString tableName);

            private:
                QThread *workerThread;
                QPushButton *exportButton;
                QLineEdit *filePath;
                ConnectionConfiguration connectionConf;
                QString tableName;
                QCheckBox *databaseCreateCheckbox, *databaseDropCheckbox, *tableCreateCheckbox, *tableDropCheckbox;
                QRadioButton *deleteAndInsert, *insert, *insertIgnore, *replace;

            signals:
                void startDump();

            public slots:
                void handleBrowseFile();
                void handleExport();
                void handleClose();
                void handleFilePathEdit(QString value);
                void handleDumpFinished();
            };
        }
    }
}
#endif // EXPORTWINDOW_H
