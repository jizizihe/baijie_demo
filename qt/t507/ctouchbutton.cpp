#include "ctouchbutton.h"

CTouchButton::CTouchButton(QWidget *parent)
    : QPushButton(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents,true);
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

slideButton::slideButton(QWidget *parent) :
    QWidget(parent)
{
    resize(50,35);
    switchflag=0; // default close
    initflag=0; //initflag = 1 init
    //update();
}

void slideButton::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if(initflag==0){ // init
        painter.setPen(Qt::NoPen);
        if(switchflag==0){
            painter.setBrush(Qt::gray);
        }
        else if(switchflag==1){
            painter.setBrush(Qt::blue);
        }
        QRectF re(1,2,40,40);
        int startAngle = 90*16;
        int endAngel = 180*16;
        painter.drawPie(re,startAngle,endAngel);
        painter.drawRect(20,2,40,40);
        QRectF re2(40,2,40,40);
        int startAngle2 = -90*16;
        int endAngel2 = 180*16;
        painter.drawPie(re2,startAngle2,endAngel2);

        if(switchflag==0){ // close status
            painter.setBrush(Qt::white);
            painter.drawEllipse(2,3,38,38);
        }
        else if(switchflag==1){ // open status
            painter.setBrush(Qt::white);
            painter.drawEllipse(41,3,38,38);
        }
        initflag=1;
    }
    else{

        painter.setPen(Qt::NoPen);
        if(switchflag==1){
            painter.setBrush(Qt::gray);
        }
        else if(switchflag==0){
            painter.setBrush(Qt::blue);
        }
        QRectF re(1,2,40,40);
        int startAngle = 90*16;
        int endAngel = 180*16;
        painter.drawPie(re,startAngle,endAngel);
        painter.drawRect(20,2,40,40);
        QRectF re2(40,2,40,40);
        int startAngle2 = -90*16;
        int endAngel2 = 180*16;
        painter.drawPie(re2,startAngle2,endAngel2);

        if(switchflag==0){ // open
            painter.setBrush(Qt::white);
            painter.drawEllipse(41,3,38,38);
            switchflag=1;
        }
        else if(switchflag==1){  //close
            painter.setBrush(Qt::white);
            painter.drawEllipse(2,3,38,38);
            switchflag=0;
        }
        emit buttonChange();
    }
}

void slideButton::mouseReleaseEvent(QMouseEvent *)
{
    update();
}


/********************************/
