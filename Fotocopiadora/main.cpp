#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fotocopiadora.h"

int main(int argc, char *argv[])
{
    Fotocopiadora _Fotocopiadora;
    _Fotocopiadora.initAll();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    return a.exec();
}
