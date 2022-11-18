#include "main_touch.h"
#include <QDebug>

bool g_touchFlag = false;

globalApp::globalApp(int &argc,char **argv):QApplication(argc,argv)
{

}

void globalApp::setWindowInstance(QWidget *wnd)
{
    g_widget = wnd;
}

bool globalApp::notify(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::KeyPress || e->type() == QEvent::MouseButtonPress ||  e->type() == QEvent::MouseMove  || e->type() == QEvent::TouchBegin || e->type() == QEvent::TouchEnd || e->type() == QEvent::TouchUpdate )
    {
        //        QDBG << e->type();

        if(e->type() == QEvent::TouchEnd || e->type() == QEvent::TouchBegin ||  e->type() == QEvent::TouchUpdate)
        {
            g_touchFlag = true;
        }
    }
    return QApplication::notify(obj,e);
}
