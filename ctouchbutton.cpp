#include "ctouchbutton.h"
#include <QDebug>

CTouchButton::CTouchButton(QWidget *parent)
    : QPushButton(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents,true);

    //slideButton *SwitchButton = new slideButton(this);
}

bool CTouchButton::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::TouchBegin:
    {
        m_bTouchEnd = false;
        QMouseEvent mouseEvent(QEvent::MouseButtonPress,QPointF(0,0),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        QCoreApplication::sendEvent(this,&mouseEvent);
        e->accept();
        return true;
    } break;
    case QEvent::TouchEnd:
    {
        QMouseEvent mouseEvent(QEvent::MouseButtonRelease,QPointF(0,0),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        QCoreApplication::sendEvent(this,&mouseEvent);
        m_bTouchEnd = true;
        e->accept();
        return true;
    } break;
    case QEvent::MouseButtonPress:
//        qDebug() << "mouse press";
    case QEvent::MouseButtonRelease:
    {
       if(m_bTouchEnd){
           e->accept();
           return true;
       }
    }break;
    default:
        break;
    }

    return QPushButton::event(e);
}


/********************************/  //slide button

