#include "mainwindow.h"
//#include "globalapp.h"
#include <QApplication>
//#include "all_interface_test.h"
#include <QTextCodec>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

int main(int argc, char *argv[])
{

//QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    qputenv("QT_IM_MODULE", QByteArray("tgtsml"));
    globalApp a(argc,argv);
//    QApplication a(argc, argv);

QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    MainWindow w_ui;
    //MainWindow *from = new MainWindow;

    QScreen *screen = qApp->primaryScreen();
    int Width = screen->size().width();			//屏幕宽
    int Height = screen->size().height();

   if(Width < Height)
   {
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *w = scene->addWidget(&w_ui);
    w->setRotation(90);

    QGraphicsView *view = new QGraphicsView(scene);

    view->setWindowFlags(Qt::FramelessWindowHint);//无边框
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
