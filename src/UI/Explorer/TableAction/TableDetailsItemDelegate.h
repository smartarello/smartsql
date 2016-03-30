#ifndef TABLEDETAILSITEMDELEGATE_H
#define TABLEDETAILSITEMDELEGATE_H
#include <QStyledItemDelegate>
#include <QPainter>

class TableDetailsItemDelegate : public QStyledItemDelegate
{

    Q_OBJECT

public:
    TableDetailsItemDelegate(QObject * parent = 0);

    QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // TABLEDETAILSITEMDELEGATE_H
