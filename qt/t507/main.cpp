#include "mainwindow.h"
#include "globalapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
    globalApp a(argc,argv);

    MainWindow w;
    w.show();

    return a.exec();
}
