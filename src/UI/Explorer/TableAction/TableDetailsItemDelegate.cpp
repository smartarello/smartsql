#include "TableDetailsItemDelegate.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QDebug>
#include <QStringListModel>

TableDetailsItemDelegate::TableDetailsItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget *TableDetailsItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if (index.column() == 3 || index.column() == 4) {
        QComboBox *box = new QComboBox(parent);
        QStringList values;
        values << tr("Yes");
        values << tr("No");
        QStringListModel *model = new QStringListModel(values) ;
        box->setModel(model);
        return box;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void TableDetailsItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    QVariant data = index.data();
//    if (data.isValid() && (index.column() == 3 || index.column() == 4)) {
//        painter->drawImage(QPoint(0, 0), QImage(":/resources/icons/exit.png"));
//        return ;
//    }

    return QStyledItemDelegate::paint(painter, option, index);
}
