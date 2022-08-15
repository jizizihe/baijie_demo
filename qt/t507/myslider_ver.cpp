



#include "myslider_ver.h"
#include <QMouseEvent>
#include <QDebug>

myslider_ver::myslider_ver(QWidget *parent):QSlider(parent)
{
       m_value=0;
       m_mousePress = false;
       m_isMoving=false;
}

myslider_ver::~myslider_ver()
{
}
void myslider_ver::mousePressEvent(QMouseEvent *event)
{

    QSlider::mousePressEvent(event);
    m_isMoving = false;
    m_mousePress = true;

    double pos = event->pos().y() / (double)height();
    double value = pos * (maximum() - minimum()) + minimum();
    value = maximum()-value;
    //value + 0.5 四舍五入
    if(value>maximum()){
        value=maximum();
    }
    if(value<minimum()){
        value=minimum();
    }
    m_value=value+0.5;
    setValue(m_value);
    //emit sliderMoved( m_value );
    //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));


}

void myslider_ver::mouseMoveEvent(QMouseEvent *event)
{
    QSlider::mouseMoveEvent(event);
    //qDebug()<<"mouseMoveEvent";
    double pos = event->pos().y() / (double)height();
    double value = pos * (maximum() - minimum()) + minimum();
    value = maximum()-value;
    if(value>maximum()){
       value=maximum();
    }
    if(value<minimum()){
       value=minimum();
    }
    //value + 0.5 四舍五入
    if(m_mousePress){
       m_value=value + 0.5;
       m_isMoving=true;
       //emit sliderMoved(m_value);
    }
    setValue(value + 0.5);
    //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));

}

void myslider_ver::mouseReleaseEvent(QMouseEvent *event)
{

    QSlider::mouseReleaseEvent(event);
    m_mousePress = false;
    m_isMoving=false;
   // emit sliderReleasedAt(m_value);//抛出有用信号
    //get_pos_timer->start(500);
}
