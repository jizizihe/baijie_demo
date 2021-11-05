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

    void language_reload();

signals:
    void Mysignal();

public slots:
    void screenclear();
    void btnreturn_clicked();
    void clearImage();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);
    QList<QColor> myPenColors;
    QImage image;

    void clear();

    CTouchButton *btnclear;
    CTouchButton *btnreturn;
    Ui::touchscreen *ui;

};

#endif // TOUCHSCREEN_H

