#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("tgtsml"));
    globalApp a(argc,argv);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    MainWindow w_ui;
    QScreen *screen = qApp->primaryScreen();
    int Width = screen->size().width();
    int Height = screen->size().height();

    if(Width < Height)
    {
        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *w = scene->addWidget(&w_ui);
        w->setRotation(90);                                           //Rotate 90 degrees,keep the landscape
        QGraphicsView *view = new QGraphicsView(scene);
        view->setWindowFlags(Qt::FramelessWindowHint);                //Set frameless
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->resize(Width,Height);
        w_ui.resize(Height,Width);
        w_ui.show();
        view->show();
    }
    else
    {
        w_ui.show();
    }

    return a.exec();
}
