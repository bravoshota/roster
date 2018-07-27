#include <QApplication>

#include "mainwindow.h"
#include "config.h"

int main(int argc, char *argv[])
{
    Config::initialize();
    if (Config::hasError())
        return 1;

    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
