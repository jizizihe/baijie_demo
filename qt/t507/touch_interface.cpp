#include "touch_interface.h"
#include <QDebug>
//touch::touch(QWidget *parent) : QWidget(parent)
//{

//}

static const qreal MinimumDiameter = 3.0;
static const qreal MaximumDiameter = 50.0;

touch::touch(QWidget *parent) :QWidget(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents);
    setAttribute(Qt::WA_StaticContents);

    myPenColors
            << QColor("green")
            << QColor("purple")
            << QColor("red")
            << QColor("blue")
            << QColor("yellow")

            << QColor("pink")
            << QColor("orange")
            << QColor("brown")
            << QColor("grey")
            << QColor("black");
}

touch::~touch()
{

}


void touch::clearImage()
{
    image.fill(qRgb(255, 255, 255));

    update();
}

void touch::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    const QRect rect = event->rect();
    painter.drawImage(rect.topLeft(), image, rect);
}

void touch::resizeEvent(QResizeEvent *event)  //Load Images
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void touch::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}


bool touch::event(QEvent *event)
{
    //QTouchEvent *touch = static_cast<QTouchEvent *>(event);
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {

        QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
        foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
            switch (touchPoint.state()) {
            case Qt::TouchPointStationary:
            case Qt::TouchPointReleased:
                // don't do anything if this touch point hasn't moved or has been released
                continue;
            default:
                {
                    QRectF rect = touchPoint.rect();
                    if (rect.isEmpty()) {
                        qreal diameter = MaximumDiameter;
                        //if (touch->device()->capabilities() & QTouchDevice::Pressure)
                            diameter = MinimumDiameter + (MaximumDiameter - MinimumDiameter) * touchPoint.pressure();
                        rect.setSize(QSizeF(diameter, diameter));
                    }

                    QPainter painter(&image);
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(myPenColors.at(touchPoint.id() % myPenColors.count()));
                    painter.drawEllipse(rect);
                    painter.end();


                    int rad = 2;
                    update(rect.toRect().adjusted(-rad,-rad, +rad, +rad));
                }
                break;
            }
        }
        break;
    }
    default:

        return QWidget::event(event);
    }
    return true;
}
