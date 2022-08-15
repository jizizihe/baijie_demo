#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QObject>
#include <QSlider>

class MySlider : public QSlider
{
public:
    MySlider(QWidget *parent = nullptr);
    ~MySlider();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool m_isMoving;
    int m_value;
    bool m_mousePress;
};

#endif // MYSLIDER_H
