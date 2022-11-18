#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QObject>
#include <QSlider>

class common_slider_press : public QSlider
{
public:
    common_slider_press(QWidget *parent = nullptr);
    ~common_slider_press();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool g_isMoving;
    int g_mouseValue;
    bool g_mousePress;
};

#endif // MYSLIDER_H
