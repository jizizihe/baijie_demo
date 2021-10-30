#include "mainwindow.h"
//#include "globalapp.h"
#include <QApplication>
#include "keyBoard.h"
//#include "all_interface_test.h"

int main(int argc, char *argv[])
{

    qputenv("QT_IM_MODULE", QByteArray("tgtsml"));
    globalApp a(argc,argv);
//    QApplication a(argc, argv);
    keyBoard keyBoard;
    keyBoard.hide();

    MainWindow w;
    w.show();

    return a.exec();
}
