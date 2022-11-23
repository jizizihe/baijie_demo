#include "touch_screen.h"
#include "ui_touchscreen.h"
#include <QScreen>
#include <QDebug>
#include <QScreen>

static int g_screenWidth;
static int g_screeHeight;
static int g_btnWidth;
static int g_btnHeight;
static const qreal MinimumDiameter = 3.0;
static const qreal MaximumDiameter = 50.0;

touchscreen::touchscreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::touchscreen)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_AcceptTouchEvents);
    setAttribute(Qt::WA_StaticContents);

    g_myPenColors
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

    g_btnClear = new CTouchButton(this);
    g_btnReturn = new CTouchButton(this);

    QScreen *screen = qApp->primaryScreen();
    g_screenWidth = screen->size().width();
    g_screeHeight = screen->size().height();

    connect(g_btnClear,SIGNAL(clicked()),this,SLOT(btn_clear_clicked()));
    connect(g_btnReturn,SIGNAL(clicked()),this,SLOT(btn_ret_clicked()));
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

void touchscreen::clearImage()   // Clear g_screen
{
    g_image.fill(qRgb(255, 255, 255));
    update();
}

void touchscreen::paintEvent(QPaintEvent *event)  // Paint Event
{
    QPainter painter(this);
    const QRect rect = event->rect();
    painter.drawImage(rect.topLeft(), g_image, rect);
}

void touchscreen::resizeEvent(QResizeEvent *event)  // Load images
{
    if (width() > g_image.width() || height() > g_image.height())
    {
        int newWidth = qMax(width(), g_image.width());
        int newHeight = qMax(height(), g_image.height());
        resizeImage(&g_image, QSize(newWidth, newHeight));
        update();
    }
    g_btnWidth = this->width()/10;
    g_btnHeight = this->height()/7;
    g_btnReturn->setIcon(QIcon(":/button_image/return.png"));
    g_btnReturn->setIconSize(QSize(32,32));
    g_btnReturn->setGeometry(10,10,g_btnWidth,g_btnHeight);
    g_btnClear->setIcon(QIcon(":/button_image/clean.png"));
    g_btnClear->setIconSize(QSize(32,32));
    g_btnClear->setGeometry(g_btnWidth+20,10,g_btnWidth,g_btnHeight);

    QWidget::resizeEvent(event);
}

void touchscreen::resizeImage(QImage *g_image, const QSize &newSize)  // Image replace
{
    if (g_image->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *g_image);
    *g_image = newImage;
}

bool touchscreen::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
        foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
            switch (touchPoint.state()) {
            case Qt::TouchPointStationary:
            case Qt::TouchPointReleased: // Don't do anything if this touch point hasn't moved or has been released

                continue;
            default:
            {
                QRectF rect = touchPoint.rect();
                if (rect.isEmpty()) {
                    qreal diameter = MaximumDiameter;
                    diameter = MinimumDiameter + (MaximumDiameter - MinimumDiameter) * touchPoint.pressure();
                    rect.setSize(QSizeF(diameter, diameter));
                }

                QPainter painter(&g_image);
                painter.setPen(Qt::NoPen);
                painter.setBrush(g_myPenColors.at(touchPoint.id() % g_myPenColors.count()));
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
