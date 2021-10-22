#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

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

namespace Ui {
class touchscreen;
}

class touchscreen : public QWidget
{
    Q_OBJECT

public:
    explicit touchscreen(QWidget *parent = 0);
    ~touchscreen();

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void language_reload();

signals:
    void Mysignal();

public slots:
    void clearImage();
    void screenclear();
    void btnreturn_clicked();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);
    void clear();

    bool modified;
    QList<QColor> myPenColors;
    QImage image;
    CTouchButton *btnclear;
    CTouchButton *btnreturn;
    Ui::touchscreen *ui;

};

#endif // TOUCHSCREEN_H

