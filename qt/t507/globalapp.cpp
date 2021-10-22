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

<<<<<<< HEAD
//        QDBG << e->type();
=======
        //QDBG << e->type();
>>>>>>> 4f340c1430a42f76bd62369d8a77885a1f0dcd86
        if(e->type() == QEvent::TouchEnd || e->type() == QEvent::TouchBegin)
        {
            touch_flag = true;
           //QDBG << touch_flag;
        }

    }
    return QApplication::notify(obj,e);
}
