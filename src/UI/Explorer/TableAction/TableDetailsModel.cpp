#include "TableDetailsModel.h"
#include <QDebug>

TableDetailsModel::TableDetailsModel(QString createString, QObject *parent) : QAbstractTableModel(parent)
{
    // Column headers
    this->headers << tr("Name");
    this->headers << tr("Data type");
    this->headers << tr("Length/Set");
    this->headers << tr("Unsigned");
    this->headers << tr("Allow NULL");
    this->headers << tr("Default");


    QStringList createParts = createString.split("\n");
    createParts.removeAt(0); // Removes the first line with CREATE TABLE
    foreach(QString part, createParts) {

        // e.g `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
        QRegExp line("^\\s*`(\\S+)` (\\w+)\\(?(\\d*)\\)?", Qt::CaseInsensitive);
        if (line.indexIn(part) != -1) { // Column definition
            ColumnDefinition column;
            column.name = line.cap(1);
            column.type = line.cap(2);
            column.length = -1;

            if (line.captureCount() == 3) {
                QString length = line.cap(3);
                if (!length.isEmpty()) {
                    column.length = line.cap(3).toInt();
                }
            }

            QRegExp unsignedRx("unsigned", Qt::CaseInsensitive);
            if (unsignedRx.indexIn(part) != -1) {
                column.unsignedCol = true;
            } else {
                column.unsignedCol = false;
            }

            QRegExp notNullRx("not null", Qt::CaseInsensitive);
            if (notNullRx.indexIn(part) != -1) {
                column.allowNull = false;
            } else {
                column.allowNull = true;
            }

            QRegExp defaultValueRx("default '(\\w)'", Qt::CaseInsensitive);
            if (defaultValueRx.indexIn(part) != -1) {
                column.defaultValue = defaultValueRx.cap(1);
            } else {
                column.defaultValue = "";
            }

            defaultValueRx = QRegExp("default null", Qt::CaseInsensitive);
            if (defaultValueRx.indexIn(part) != -1) {
                column.defaultValue = QVariant();
            }



            this->columns << column;
        }
    }

}


int TableDetailsModel::rowCount(const QModelIndex & parent) const
{
    return this->columns.size();
}

int TableDetailsModel::columnCount(const QModelIndex & parent) const
{
    return this->headers.size();
}

QVariant TableDetailsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < this->headers.size() && role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return QVariant(this->headers.at(section));
    }

    return QVariant();
}

QVariant TableDetailsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < this->columns.size()) {
        ColumnDefinition col = this->columns.at(index.row());

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
            case 0:
                return QVariant(col.name);
            case 1:
                return QVariant(col.type.toUpper());
            case 2:
                if (col.length != -1) {
                    return col.length;
                }
                break;
            case 3:
                if (col.unsignedCol) {
                    return tr("Yes");
                }
                 break;

            case 4:
                if (col.allowNull) {
                    return tr("Yes");
                }
                 break;

            case 5:
                if (col.defaultValue.isNull()) {
                    return "NULL";
                } else {
                    return col.defaultValue;
                }

            }
        } else if (role == Qt::FontRole && index.column() == 5 && col.defaultValue.isNull()) {
            QFont font("Ubuntu Regular");
            font.setItalic(true);
            return QVariant(font);
        }
    }

    return QVariant();
}

Qt::ItemFlags TableDetailsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
