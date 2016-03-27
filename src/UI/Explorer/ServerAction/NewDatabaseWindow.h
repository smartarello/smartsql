#ifndef NEWDATABASEWINDOW_H
#define NEWDATABASEWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>

namespace UI {
namespace Explorer {
namespace ServerAction {
class NewDatabaseWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit NewDatabaseWindow(QWidget *parent = 0);

private:
    QStringList getCollationList();
    QLineEdit *nameLineEdit;
    QComboBox *collationComboBox;

signals:
    void createDatabase(QString databaseName, QString collation);

public slots:
    void handleCancelButton();
    void handleCreateButton();
};
}
}
}



#endif // NEWDATABASEWINDOW_H
