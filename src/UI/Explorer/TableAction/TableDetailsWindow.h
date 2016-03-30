#ifndef TABLEDETAILSWINDOW_H
#define TABLEDETAILSWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QSqlDatabase>

class TableDetailsWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit TableDetailsWindow(QSqlDatabase database, QString tableName,  QWidget *parent = 0);

private:
    QTableView *tableColumns;

signals:

public slots:
};

#endif // TABLEDETAILSWINDOW_H
