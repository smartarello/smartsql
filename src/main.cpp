/**
 * Copyright (C) 2016  Stéphane Martarello
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

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
        font.setPointSize(10);
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




