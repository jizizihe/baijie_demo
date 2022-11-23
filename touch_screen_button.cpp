#include "touch_screen_button.h"

CTouchButton::CTouchButton(QWidget *parent)
    : QPushButton(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents,true);
}

bool CTouchButton::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::TouchBegin:
    {
        g_touchEnd = false;
        QMouseEvent mouseEvent(QEvent::MouseButtonPress,QPointF(0,0),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        QCoreApplication::sendEvent(this,&mouseEvent);
        event->accept();
        return true;
    } break;
    case QEvent::TouchEnd:
    {
        QMouseEvent mouseEvent(QEvent::MouseButtonRelease,QPointF(0,0),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        QCoreApplication::sendEvent(this,&mouseEvent);
        g_touchEnd = true;
        event->accept();
        return true;
    } break;
    case QEvent::MouseButtonPress:

    case QEvent::MouseButtonRelease:
    {
        if(g_touchEnd){
            event->accept();
            return true;
        }
    }
        break;
    default:
        break;
    }
    return QPushButton::event(event);
}

