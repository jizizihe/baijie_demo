#include "touchscreen.h"
#include "ui_touchscreen.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScreen>
#include <QDebug>
#include <QScreen>

static int screenWidth;
static int screeHeight;
static int screenFlag;
static int btnWidth;
static int btnHeight;
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

    btn_clear = new CTouchButton(this);
    btn_return = new CTouchButton(this);

    QScreen *screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screeHeight = screen->size().height();
    if(screenWidth < screeHeight)
    {
        screenFlag = 1;
    }

    connect(btn_clear,SIGNAL(clicked()),this,SLOT(btn_clear_clicked()));
    connect(btn_return,SIGNAL(clicked()),this,SLOT(btn_ret_clicked()));
}

touchscreen::~touchscreen()
{
    delete ui;
}

void touchscreen::btn_ret_clicked()
{
    clearImage();
    emit touch_back_msg();
}

void touchscreen::btn_clear_clicked()
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
        int newWidth = qMax(width(), image.width());
        int newHeight = qMax(height(), image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }

    if (screenFlag == 1)
    {
        btnWidth = this->width()/7;
        btnHeight = this->height()/10;
        btn_return->setIcon(QIcon(":/button_image/rotate_return.png"));
        btn_return->setIconSize(QSize(32,32));
        btn_return->setGeometry(screenWidth-btnWidth-10,10,btnWidth,btnHeight);

        btn_clear->setIcon(QIcon(":/button_image/rotate_clean.png"));
        btn_clear->setIconSize(QSize(32,32));
        btn_clear->setGeometry(screenWidth-btnWidth-10,btnHeight+20,btnWidth,btnHeight);
    }
    else
    {
        btnWidth = this->width()/10;
        btnHeight = this->height()/7;
        btn_return->setIcon(QIcon(":/button_image/return.png"));
        btn_return->setIconSize(QSize(32,32));
        btn_return->setGeometry(10,10,btnWidth,btnHeight);

        btn_clear->setIcon(QIcon(":/button_image/clean.png"));
        btn_clear->setIconSize(QSize(32,32));
        btn_clear->setGeometry(btnWidth+20,10,btnWidth,btnHeight);
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
