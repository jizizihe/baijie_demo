#include "common_slider_press.h"
#include <QMouseEvent>
#include <QDebug>

common_slider_press::common_slider_press(QWidget *parent):QSlider(parent)        // Horizontal direction
{
    g_mouseValue=0;
    g_mousePress = false;
    g_isMoving=false;
}

common_slider_press::~common_slider_press()
{

}

void common_slider_press::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    g_isMoving = false;
    g_mousePress = true;

    double pos = event->pos().x() / (double)width();
    double value = pos * (maximum() - minimum()) + minimum();

    if(value>maximum()){
        value=maximum();
    }
    if(value<minimum()){
        value=minimum();
    }
    g_mouseValue=value+0.5;
    setValue(g_mouseValue);

    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
}

void common_slider_press::mouseMoveEvent(QMouseEvent *event)
{
    QSlider::mouseMoveEvent(event);
    double pos = event->pos().x() / (double)width();
    double value = pos * (maximum() - minimum()) + minimum();
    if(value>maximum()){
        value=maximum();
    }
    if(value<minimum()){
        value=minimum();
    }

    if(g_mousePress){
        g_mouseValue=value + 0.5;
        g_isMoving=true;
    }
    setValue(value + 0.5);
    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
}

void common_slider_press::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
    g_mousePress = false;
    g_isMoving=false;
}
