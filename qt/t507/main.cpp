#include "mainwindow.h"
//#include "globalapp.h"
#include <QApplication>
#include "keyBoard.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
    globalApp a(argc,argv);

    keyBoard keyBoard;
    keyBoard.hide();

    MainWindow w;
    w.show();

    return a.exec();
}
