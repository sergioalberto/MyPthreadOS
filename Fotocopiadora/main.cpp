#include <QtGui/QApplication>
#include "mainwindow.h"
#include "photocopier.h"

int main(int argc, char *argv[])
{
    photocopier _photocopier;
    //_Fotocopiadora.initAll();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    return a.exec();
}
