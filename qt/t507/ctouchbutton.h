#ifndef CTOUCHBUTTON_H
#define CTOUCHBUTTON_H

#include <QPushButton>
#include <QEvent>
#include <QCoreApplication>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QPainter>

class CTouchButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CTouchButton(QWidget *parent = 0);

protected:
    bool event(QEvent *e);

private:
    bool m_bTouchEnd = false;
};

/*****************************/  //slide button

//class slideButton : public QWidget
//{
//    Q_OBJECT

//public:
//    explicit slideButton(QWidget *parent = 0);
//    void mouseReleaseEvent(QMouseEvent *);  //mouse up time
//    int switchflag = 0;
//    int initflag = 0;

//protected:
//    void paintEvent(QPaintEvent *event);

//signals:
//    void buttonChange();

//public slots:
//};

/******************************/

#endif // CTOUCHBUTTON_H
