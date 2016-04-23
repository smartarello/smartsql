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
        this->tableCount = 0;
        this->progress = 0;
        this->currentTable = "";
        this->progressCurrentTable = 0;
        this->totalCurrentTable = 0;
        this->stop = false;
    }


    /**
     * @brief MySQLDump::setDropTable
     * @param dropTable if true, the dump includes the statements to drop the tables
     */
    void MySQLDump::setDropTable(bool dropTable)
    {
        this->dropTable = dropTable;
    }

    /**
     * @brief MySQLDump::setDropDatabase
     * @param dropDatabase if true, the dump includes the statements to drop the database
     */
    void MySQLDump::setDropDatabase(bool dropDatabase)
    {
        this->dropDatabase = dropDatabase;
    }

    /**
     * @brief MySQLDump::setCreateDatabase
     * @param createDatabase if true, the dump includes the statements to create the database
     */
    void MySQLDump::setCreateDatabase(bool createDatabase)
    {
        this->createDatabase = createDatabase;
    }

    /**
     * @brief MySQLDump::setCreateTable
     * @param createTable if true, the dump includes the statements to create the tables
     */
    void MySQLDump::setCreateTable(bool createTable)
    {
        this->createTable = createTable;
    }

    /**
     * @brief MySQLDump::setFormat
     * @param format the format for the dump, changes the way the lines are inserted (INSERT INTO, INSERT IGNORE INTO, REPLACE, ...)
     */
    void MySQLDump::setFormat(MySQLDumpFormat format)
    {
        this->format = format;
    }

    /**
     * @brief MySQLDump::setTables
     * @param tableList the list of tables name to dump, if the table list is not set, all the database will be dumped
     */
    void MySQLDump::setTables(QStringList tableList)
    {
        this->tables = tableList;
    }

    /**
     * Starts the dump process
     * @brief MySQLDump::dump
     */
    void MySQLDump::dump()
    {
        QSqlDatabase database = DataBase::createFromConfig(this->configuration);
        if (!database.open()) {
            qDebug() << database.lastError().text();
            emit dumpFinished(true);
            return ;
        }


        QFile *file = new QFile(this->filename);
        if (file->exists()) {
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

            stream <<  endl;

            if (this->tables.isEmpty()) {
                this->tables = database.tables();
            }

            this->tableCount = this->tables.size();
            foreach(QString table, this->tables) {
                if (this->stop) {
                    break;
                }
                this->dumpTable(database, table, file);
            }

            stream << endl;

            file->close();
        } else {
            qDebug() << "Unable to open the file: "+this->filename;
        }

        database.close();

        emit dumpFinished(this->stop);
    }

    /**
     * @brief MySQLDump::dumpTable
     * @param database the source database
     * @param table the table to dump
     * @param file the ouput file
     */
    void MySQLDump::dumpTable(QSqlDatabase database, QString table, QFile *file)
    {
        this->currentTable = table;
        this->progressCurrentTable = 0;
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

        stream << endl;

        QSqlQuery tableQuery(database);
        // The following query is used to compute the progress during the dump
        if (tableQuery.exec("SELECT count(*) FROM "+table) && tableQuery.next()) {
            this->totalCurrentTable = tableQuery.value(0).toInt();
            int offset = 0;
            bool isFirstLine = true;

            // Uses a batch process to avoid memory issue
            while (!this->stop && tableQuery.exec(QString("SELECT * FROM %1 LIMIT %2, 100000").arg(table).arg(offset)) && tableQuery.size() > 0){

                while(tableQuery.next() && !this->stop) {

                    QSqlRecord row = tableQuery.record();

                    if (isFirstLine) {

                        switch (this->format) {
                            case DELETE_AND_INSERT:
                                stream << "DELETE FROM "+table+";" << endl;
                                stream << "INSERT INTO "+table+" (";
                                break;

                            case INSERT_IGNORE:
                                stream << "INSERT IGNORE INTO "+table+" (";
                                break;

                            case REPLACE:
                                stream << "REPLACE INTO "+table+" (";
                                break;

                            default:
                                stream << "INSERT INTO "+table+" (";
                        }

                        QStringList fields;
                        for (int i = 0; i < row.count(); i++) {
                            fields << "`"+row.fieldName(i)+"`";
                        }

                        stream << fields.join(",")  << ") VALUES " << endl << "(";

                        isFirstLine = false;
                    } else {
                        stream << "," << endl << "(";
                    }

                    QStringList values;
                    for(int i = 0; i<row.count(); i++) {
                        values << database.driver()->formatValue(row.field(i));
                    }

                    stream << values.join(",") << ")";
                    this->progressCurrentTable++;
                }

                offset += tableQuery.size();
            }

            stream << ";" << endl;
        }

        this->progress++;

        stream << endl;
    }

    /**
     * Gets the number of rows exported for the current table
     * @brief MySQLDump::getProgressCurrentTable
     * @return the number of rows exported for the current table
     */
    int MySQLDump::getProgressCurrentTable()
    {
        return this->progressCurrentTable;
    }

    /**
     * @brief MySQLDump::getProgress
     * @return the number of tables exported
     */
    int MySQLDump::getProgress()
    {
        return this->progress;
    }

    /**
     * @brief MySQLDump::getTotalLine
     * @return the number of rows to dump for the current table
     */
    int MySQLDump::getTotalLine()
    {
        return this->totalCurrentTable;
    }

    /**
     * @brief MySQLDump::getCurrentTable
     * @return The table name for the table which are processing
     */
    QString MySQLDump::getCurrentTable()
    {
        return this->currentTable;
    }

    /**
     * @brief MySQLDump::getTableCount
     * @return The count of tables to export
     */
    int MySQLDump::getTableCount()
    {
        return tableCount;
    }

    /**
     * Stops the dump process
     * @brief MySQLDump::stopRequired
     */
    void MySQLDump::stopRequired()
    {
        this->stop = true;
    }
}

