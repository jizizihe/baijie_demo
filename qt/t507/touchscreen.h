#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <QMainWindow>
#include <QEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QTouchEvent>

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QPushButton>
#include <QDebug>
#include "ctouchbutton.h"
#include "touch_interface.h"

namespace Ui {
class touchscreen;
}

class touchscreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit touchscreen(QWidget *parent = 0);
    ~touchscreen();

    void language_reload();

signals:
    void Mysignal();

public slots:
    void screenclear();
    void btnreturn_clicked();

private:
    void clear();
    touch *scribbleArea;

    CTouchButton *btnclear;
    CTouchButton *btnreturn;
    Ui::touchscreen *ui;

};

#endif // TOUCHSCREEN_H

