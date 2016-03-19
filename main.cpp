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

    QFontDatabase::addApplicationFont(":/resources/fonts/cour.pfa");
    QFontDatabase::addApplicationFont(":/resources/fonts/couri.pfa");
    QFontDatabase::addApplicationFont(":/resources/fonts/courb.pfa");

    int fontId = QFontDatabase::addApplicationFont(":/resources/fonts/DejaVuSans.ttf");
    if (fontId != -1)
    {
        QFont font("DejaVuSans");
        app.setFont(font);
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




