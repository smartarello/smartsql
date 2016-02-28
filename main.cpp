#include <QApplication>
#include "UI/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    UI::MainWindow win ;

    win.show();

    return app.exec();
}




