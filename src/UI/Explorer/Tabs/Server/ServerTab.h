#ifndef SERVERTAB_H
#define SERVERTAB_H

#include <QTableView>
#include <QWidget>

class ServerTab : public QTableView
{
    Q_OBJECT

public:
    ServerTab(QWidget *parent = 0);
    virtual ~ServerTab();
};

#endif // SERVERTAB_H
