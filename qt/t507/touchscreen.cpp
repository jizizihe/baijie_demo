#include "touchscreen.h"
#include "ui_touchscreen.h"

static const qreal MinimumDiameter = 3.0;
static const qreal MaximumDiameter = 50.0;

touchscreen::touchscreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::touchscreen)
{
    ui->setupUi(this);
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

    btnclear = new CTouchButton(this);
    btnclear->setText(tr("clear"));
    btnclear->setGeometry(900,480,100,40);

    btnreturn = new CTouchButton(this);
//    btnreturn->setText(tr("return"));
    btnreturn->setIcon(QIcon(":/t507_button_image/return.png"));
    btnreturn->setGeometry(10,10,100,40);

    connect(btnclear,SIGNAL(clicked()),this,SLOT(screenclear()));
    connect(btnreturn,SIGNAL(clicked()),this,SLOT(btnreturn_clicked()));
}

touchscreen::~touchscreen()
{
    delete ui;
}

void touchscreen::btnreturn_clicked()
{
    clearImage();
    emit Mysignal();
}

void touchscreen::screenclear()
{
    clearImage();
}

void touchscreen::clearImage()  //clear screen
{
    image.fill(qRgb(255, 255, 255));

    update();
}

void touchscreen::paintEvent(QPaintEvent *event)  //paint Event
{
    QPainter painter(this);
    const QRect rect = event->rect();
    painter.drawImage(rect.topLeft(), image, rect);
}

void touchscreen::resizeEvent(QResizeEvent *event)  //Load Images
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void touchscreen::resizeImage(QImage *image, const QSize &newSize)  //image replace
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}


bool touchscreen::event(QEvent *event)
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


void touchscreen::language_reload()
{
    btnclear->setText(tr("clear"));
}
