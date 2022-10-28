#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <QMainWindow>
#include <QWidget>
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
#include <QGraphicsView>
#include <QWidget>


namespace Ui {
class touchscreen;
}

class touchscreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit touchscreen(QWidget *parent = 0);
    ~touchscreen();

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void clearImage();

signals:
    void touch_back_msg();

public slots:
    void btn_ret_clicked();
    void btn_clear_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);
    QList<QColor> myPenColors;
    QImage image;
    void clear();
    CTouchButton *btn_clear;
    CTouchButton *btn_return;
    Ui::touchscreen *ui;
};

#endif // TOUCHSCREEN_H

