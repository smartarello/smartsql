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
#include "MySQLDump.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlDriver>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace Util {
    MySQLDump::MySQLDump(ConnectionConfiguration conf, QString filename):
        configuration(conf),
        filename(filename)
    {

    }


    void MySQLDump::setDropTable(bool dropTable)
    {
        this->dropTable = dropTable;
    }

    void MySQLDump::setDropDatabase(bool dropDatabase)
    {
        this->dropDatabase = dropDatabase;
    }

    void MySQLDump::setCreateDatabase(bool createDatabase)
    {
        this->createDatabase = createDatabase;
    }

    void MySQLDump::setCreateTable(bool createTable)
    {
        this->createTable = createTable;
    }

    void MySQLDump::setFormat(MySQLDumpFormat format)
    {
        this->format = format;
    }

    void MySQLDump::setTable(QString table)
    {
        this->tablename = table;
    }

    void MySQLDump::dump()
    {
        qDebug() << "Start dump ...";

        QSqlDatabase database = DataBase::createFromConfig(this->configuration);
        if (!database.open()) {
            qDebug() << database.lastError().text();
            emit dumpFinished();
            return ;
        }


        QFile *file = new QFile(this->filename);
        if (file->exists()) {
            qDebug() << "Remove existing file";
            file->remove();
        }

        if (file->open(QIODevice::Append))
        {
            QTextStream stream(file);


            if (this->dropDatabase) {
                stream << "DROP DATABASE `"+ database.databaseName() +"`;" << endl;
            }

            if (this->createDatabase) {
                QSqlQuery createDatabaseQuery(database);
                if (createDatabaseQuery.exec("SHOW CREATE DATABASE "+ database.databaseName()) && createDatabaseQuery.next()) {
                    stream << createDatabaseQuery.value(1).toString() + ";" << endl;
                }
            }

            if (this->tablename.isEmpty()) {

            } else {
                this->dumpTable(database, this->tablename, file);
            }

            stream << endl;

            file->close();
        } else {
            qDebug() << "Unable to open the file: "+this->filename;
        }

        qDebug() << "End thread";
        database.close();
        emit dumpFinished();
    }

    void MySQLDump::dumpTable(QSqlDatabase database, QString table, QFile *file)
    {
        QTextStream stream(file);
        if (this->dropTable) {
            stream << "DROP TABLE `"+ database.databaseName() +"`;" << endl;
        }

        if (this->createTable) {
            QSqlQuery createTableQuery(database);
            if (createTableQuery.exec("SHOW CREATE TABLE "+ table) && createTableQuery.next()) {
                stream << createTableQuery.value(1).toString() + ";" << endl;
            }
        }

        QSqlQuery tableQuery(database);
        if (tableQuery.exec("SELECT * FROM "+table)){

            if (tableQuery.next()) {

                stream << "INSERT INTO "+table+" (";

                QSqlRecord row = tableQuery.record();
                QStringList fields;
                for (int i = 0; i < row.count(); i++) {
                    fields << "`"+row.fieldName(i)+"`";
                }



                stream << fields.join(",")  << ") VALUES " << endl << "(";

                QStringList values;
                for(int i = 0; i<row.count(); i++) {
                    values << database.driver()->formatValue(row.field(i));
                }

                stream << values.join(",") << ")" ;

                while(tableQuery.next()) {
                    stream << "," << endl << "(";
                    row = tableQuery.record();

                    QStringList values;
                    for(int i = 0; i<row.count(); i++) {
                        values << database.driver()->formatValue(row.field(i));
                    }

                    stream << values.join(",") << ")";

                } ;

                stream << ";" << endl;
            }

        }
    }
}
