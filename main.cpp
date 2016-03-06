#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>
#include "UI/MainWindow.h"

int main(int argc, char *argv[])
{
	qInfo() << "Start the application";
    QApplication app(argc, argv);

//    QTranslator qtTranslator;
//    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//    app.installTranslator(&qtTranslator);
//
//    QTranslator myappTranslator;
//    myappTranslator.load("mysqlclient_" + QLocale::system().name());
//    app.installTranslator(&myappTranslator);

    UI::MainWindow win ;

    win.show();

    return app.exec();
}




