#include "globalapp.h"
#include <QDebug>

bool touch_flag = false;

#define QDBG qDebug()<<__FILE__<<__FUNCTION__<<__LINE__

globalApp::globalApp(int &argc,char **argv):QApplication(argc,argv)
{

}

void globalApp::setWindowInstance(QWidget *wnd)
{
     widget = wnd;
}

bool globalApp::notify(QObject *obj, QEvent *e)
{

    if(e->type() == QEvent::KeyPress || e->type() == QEvent::MouseButtonPress ||  e->type() == QEvent::MouseMove  || e->type() == QEvent::TouchBegin || e->type() == QEvent::TouchEnd || e->type() == QEvent::TouchUpdate )
    {
//        QDBG << e->type();

        if(e->type() == QEvent::TouchEnd || e->type() == QEvent::TouchBegin ||  e->type() == QEvent::TouchUpdate)
        {
            touch_flag = true;
        }
    }
    return QApplication::notify(obj,e);
}
