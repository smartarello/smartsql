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

    QString fontPath = ":/resources/fonts/DejaVuSans.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontPath);
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




