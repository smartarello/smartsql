#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QFontDatabase>
#include <QDebug>
#include "UI/MainWindow.h"

int main(int argc, char *argv[])
{
	qInfo() << "Start the application";
    QApplication app(argc, argv);

    if (QFontDatabase::addApplicationFont(":/resources/fonts/cour.pfa") == -1) {
    	qDebug() << "Unable to load font";
    }

    QFontDatabase::addApplicationFont(":/resources/fonts/couri.pfa");
    QFontDatabase::addApplicationFont(":/resources/fonts/courb.pfa");

    int fontId = QFontDatabase::addApplicationFont(":/resources/fonts/Ubuntu-R.ttf");
    if (fontId != -1)
    {
        QFont font("Ubuntu Regular");
        app.setFont(font);

        qDebug() << "App font familly: " + app.font().family();
    } else {
    	qWarning() << "Font not found";
    }

//    QTranslator qtTranslator;font.defaultFamily()
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




