#include "touchscreen.h"
#include "ui_touchscreen.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScreen>
#include <QDebug>
#include <QScreen>

static int s_width;
static int s_height;
static int screen_flag;

static int btn_w;
static int btn_h;
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
    btnreturn = new CTouchButton(this);

    QScreen *screen = qApp->primaryScreen();
    s_width = screen->size().width();			//屏幕宽
    s_height = screen->size().height();
    if(s_width < s_height)
    {
        screen_flag = 1;
    }

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
        int newWidth = qMax(width(), image.width());
        int newHeight = qMax(height(), image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }

    if (screen_flag == 1)
    {
        btn_w = this->width()/7;
        btn_h = this->height()/10;
        btnreturn->setIcon(QIcon(":/button_image/rotate_return.png"));
        btnreturn->setIconSize(QSize(32,32));
        btnreturn->setGeometry(s_width-btn_w-10,10,btn_w,btn_h);

        btnclear->setIcon(QIcon(":/button_image/rotate_clean.png"));
        btnreturn->setIconSize(QSize(32,32));
        btnclear->setGeometry(s_width-btn_w-10,btn_h+20,btn_w,btn_h);
    }
    else
    {
        btn_w = this->width()/10;
        btn_h = this->height()/7;
        btnreturn->setIcon(QIcon(":/button_image/return.png"));
        btnreturn->setIconSize(QSize(32,32));
        btnreturn->setGeometry(10,10,btn_w,btn_h);

        btnclear->setIcon(QIcon(":/button_image/clean.png"));
        btnreturn->setIconSize(QSize(32,32));
        btnclear->setGeometry(btn_w+20,10,btn_w,btn_h);
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

}
