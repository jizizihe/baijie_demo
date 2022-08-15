#ifndef MYSLIDER_VER_H
#define MYSLIDER_VER_H

#include <QObject>
#include <QSlider>

class myslider_ver : public QSlider
{
    Q_OBJECT

public:
    explicit myslider_ver(QWidget *parent = nullptr);
    ~myslider_ver();

        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

        bool m_isMoving;
        int m_value;
        bool m_mousePress;
};

#endif // MYSLIDER_VER_H
