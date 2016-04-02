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



    int fontId = QFontDatabase::addApplicationFont(":/resources/fonts/Lato-Regular.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Lato-Light.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Lato-Bold.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Lato-Italic.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Lato-BoldItalic.ttf");

    QFontDatabase::addApplicationFont(":/resources/fonts/DejaVuSansMono.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/DejaVuSansMono-Bold.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/DejaVuSansMono-Oblique.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/DejaVuSansMono-BoldOblique.ttf");
    if (fontId != -1)
    {
        QFont font("Lato Regular");
        app.setFont(font);
    } else {
        qWarning() << "Unable to load the font: Lato-Regular.ttf";
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




