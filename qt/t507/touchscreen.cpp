#include "touchscreen.h"
#include "ui_touchscreen.h"
#include "ctouchbutton.h"
static const qreal MinimumDiameter = 3.0;
static const qreal MaximumDiameter = 50.0;

touchscreen::touchscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::touchscreen)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_AcceptTouchEvents);
    setAttribute(Qt::WA_StaticContents);

    CTouchButton *btnclear = new CTouchButton(this);
    btnclear->setText("clear");
    btnclear->setGeometry(780,480,100,50);

    CTouchButton *btnreturn = new CTouchButton(this);
    btnreturn->setText("return");
    btnreturn->setGeometry(900,480,100,50);

    connect(btnclear,SIGNAL(clicked()),this,SLOT(screenclear()));
    connect(btnreturn,SIGNAL(clicked()),this,SLOT(btnreturn_clicked()));

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

touchscreen::~touchscreen()
{
    delete ui;
}

void touchscreen::btnreturn_clicked()
{
    this->clearImage();
    emit Mysignal();
}

bool touchscreen::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;

    update();
    return true;
}

bool touchscreen::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {

        return true;
    } else {
        return false;
    }
}

void touchscreen::clearImage()
{
    image.fill(qRgb(255, 255, 255));

    update();
}

void touchscreen::paintEvent(QPaintEvent *event)
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

void touchscreen::resizeImage(QImage *image, const QSize &newSize)
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

                //qDebug() << "1111";
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
    //qDebug("1111\n");


    return true;
}

void touchscreen::screenclear()
{
    this->clearImage();
}

