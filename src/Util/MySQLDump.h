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
#ifndef MYSQLDUMP_H
#define MYSQLDUMP_H

#include "DataBase.h"
#include <QObject>
#include <QSqlDatabase>
#include <QTextStream>
#include <QFile>

namespace Util {
    class MySQLDump : public QObject
    {

        Q_OBJECT

    public:

        enum MySQLDumpFormat {
            DELETE_AND_INSERT,
            INSERT,
            INSERT_IGNORE,
            REPLACE
        };

        MySQLDump(ConnectionConfiguration conf, QString filename);
        void setDropTable(bool dropTable);
        void setDropDatabase(bool dropDatabase);
        void setCreateDatabase(bool createDatabase);
        void setCreateTable(bool createTable);
        void setFormat(MySQLDumpFormat format);
        void setTable(QString tablename);

        int getProgress();
        int getProgressCurrentTable();
        int getTotalLine();
        int getTableCount();
        QString getCurrentTable();
        void stopRequired();

    public slots:
        void dump();

    signals:
        void dumpFinished();

    private:
        ConnectionConfiguration configuration;
        bool dropTable;
        bool dropDatabase;
        bool createTable;
        bool createDatabase;
        MySQLDumpFormat format;
        QString tablename;
        QString filename;
        int progressCurrentTable;
        int totalCurrentTable;
        int progress;
        int tableCount;
        QString currentTable;
        bool stop;

        void dumpTable(QSqlDatabase database, QString table, QFile *stream);
    };
}


#endif // MYSQLDUMP_H
