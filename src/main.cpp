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



    int fontId = QFontDatabase::addApplicationFont(":/resources/fonts/Ubuntu-R.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Ubuntu-B.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Ubuntu-RI.ttf");
    if (fontId != -1)
    {
        QFont font("Ubuntu Regular");
        app.setFont(font);
    } else {
        qWarning() << "Unable to load the font: Ubuntu-R.ttf";
    }

    if (QFontDatabase::addApplicationFont(":/resources/fonts/UbuntuMono-R.ttf") == -1) {
        qDebug() << "Unable to load font: UbuntuMono-R.ttf";
    }

    if (QFontDatabase::addApplicationFont(":/resources/fonts/UbuntuMono-R.ttf") == -1) {
        qDebug() << "Unable to load font: UbuntuMono-R.ttf";
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




