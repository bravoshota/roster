#include <QApplication>

#include "mainwindow.h"
#include "config.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Config::initialize();
    if (Config::hasError())
        return 1;

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
