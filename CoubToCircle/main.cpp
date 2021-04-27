#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setFixedSize(700,580);
    w.setWindowTitle("Coub2Sphere");
    w.show();
    return a.exec();
}
