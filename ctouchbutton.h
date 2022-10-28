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
    bool touchEnd = false;
};

#endif // CTOUCHBUTTON_H
