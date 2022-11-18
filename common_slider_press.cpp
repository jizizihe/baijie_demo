#include "myslider.h"
#include <QMouseEvent>
#include <QDebug>

MySlider::MySlider(QWidget *parent):QSlider(parent)        //horizontal direction
{
    mouseValue=0;
    mousePress = false;
    isMoving=false;
}

MySlider::~MySlider()
{

}

void MySlider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    isMoving = false;
    mousePress = true;

    double pos = event->pos().x() / (double)width();
    double value = pos * (maximum() - minimum()) + minimum();

    if(value>maximum()){
        value=maximum();
    }
    if(value<minimum()){
        value=minimum();
    }
    mouseValue=value+0.5;
    setValue(mouseValue);

    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
}

void MySlider::mouseMoveEvent(QMouseEvent *event)
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

    if(mousePress){
        mouseValue=value + 0.5;
        isMoving=true;
    }
    setValue(value + 0.5);
    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));

}

void MySlider::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
    mousePress = false;
    isMoving=false;
}
