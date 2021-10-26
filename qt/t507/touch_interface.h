#ifndef TOUCH_INTERFACE_H
#define TOUCH_INTERFACE_H

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

class touch : public QWidget
{
    Q_OBJECT

public:
    explicit touch(QWidget *parent = 0);
    ~touch();

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);

public slots:
    void clearImage();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);
    QList<QColor> myPenColors;
    QImage image;

};


#endif // TOUCH_INTERFACE_H
