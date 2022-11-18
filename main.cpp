#include <QApplication>
#include <QTextCodec>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QGraphicsProxyWidget>
#include "language_selection.h"
#include "database.h"

#include<stdio.h>
#include<sys/ioctl.h>
#include<linux/fb.h>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("tgtsml"));

    globalApp a(argc,argv);
    QScreen *g_screen = qApp->primaryScreen();
    int Width = g_screen->size().width();
    int Height = g_screen->size().height();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    language_selection languageSelectionWg;
    database databaseWg;

    QStringList list = databaseWg.tableShow("language");
    if(!list.isEmpty())
    {
       languageSelectionWg.desktopShow();
    }
    else
    {
        if(Width < Height)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&languageSelectionWg);
            w->setRotation(90);                                              //Rotate 90 degrees,keep the landscape
            QGraphicsView *view = new QGraphicsView(scene);
            view->setWindowFlags(Qt::FramelessWindowHint);                   //Set frameless
            view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            view->resize(Width,Height);
            languageSelectionWg.resize(Height,Width);
            languageSelectionWg.show();
            view->show();
        }
        else
        {
            languageSelectionWg.show();
        }
    }
    return a.exec();
}
