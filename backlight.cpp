#include "backlight.h"
#include "ui_backlight.h"
#include <QScreen>
#include <QDebug>

static int g_lightValue;
static int g_indexNumber;         //The subscript of an array that stores sleep times
static int g_screenWidth;
static unsigned int g_timerArray[7] = {15,30,60,120,300,600,429499999};   //Sleep time
static QTimer *g_timeUp;
static QTimer *g_timing;         //sleep

int setBacklight(int value)
{
    int fd;
    int sourceValue;
    unsigned long args[2] = {0} ;
    fd = open("/dev/disp", O_RDWR, 0);
    if(fd < 0)
    {
        qDebug() << "open /dev/disp failed.\n";
        return -1;
    }

    args[0] = 0;
    args[1] = value;
    sourceValue = ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args);

    ioctl(fd,DISP_LCD_SET_BRIGHTNESS,args);
    close(fd);
    return sourceValue;
}

int getBacklight()
{
    int fd;
    int sourceValue;
    unsigned long args[2] = {0} ;
    fd = open("/dev/disp", O_RDWR, 0);
    if(fd < 0)
    {
        qDebug() << "open /dev/disp failed.\n";
        return -1;
    }
    sourceValue = ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args);
    close(fd);
    return sourceValue;
}

backlight::backlight(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::backlight)
{
    ui->setupUi(this);
    ui->horizontalSlider_light->setRange(138,255);
    ui->horizontalSlider_light->setValue(255);
    g_lightValue = 255;
    setBacklightFont();
    ui->lbl_lightValue->setText(tr("255"));
    g_timing = new QTimer(this);
    g_timing->start();
    connect(g_timing,SIGNAL(timeout()),this,SLOT(light_screen()));
    g_timeUp = new QTimer(this);
    g_timeUp->start();
    connect(g_timeUp,SIGNAL(timeout()),this,SLOT(dark_screen()));
    g_indexNumber = 6;
    ui->cmb_sleep_time->setCurrentIndex(6);
}

backlight::~backlight()
{
    delete ui;
}

void backlight::light_screen()
{
    int nowValueLight = getBacklight();
    if(nowValueLight == 0 && g_touchFlag)   //touchFlag external variable in globalapp.h
    {
        g_timeUp->start();
        setBacklight(g_lightValue);
    }
    usleep(5000);
}

void backlight::dark_screen()       //check whether events are generated
{
    QDateTime now = QDateTime::currentDateTime().addSecs(g_timerArray[g_indexNumber]);
    QDateTime shade = QDateTime::currentDateTime().addSecs((unsigned)g_timerArray[g_indexNumber] - 1);

    while(QDateTime::currentDateTime() < now)
    {
        if(g_touchFlag)
        {
            now = QDateTime::currentDateTime().addSecs((unsigned)g_timerArray[g_indexNumber]);
            shade = QDateTime::currentDateTime().addSecs((unsigned)g_timerArray[g_indexNumber] - 1);
            g_touchFlag = false;
        }

        if(QDateTime::currentDateTime() > shade)
        {
        }

        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
    g_timeUp->stop();
    setBacklight(0);
}

void backlight::on_horizontalSlider_light_valueChanged(int value)
{
    g_lightValue = setBacklight(value);
    QString strValue = QString("%1").arg(value);
    ui->lbl_lightValue->setText(tr(QString("%1").arg(strValue).toUtf8()));
}

void backlight::on_cmb_sleep_time_currentIndexChanged(int index)
{
    g_indexNumber = index;
}

void backlight::on_btn_ret_clicked()
{
    emit backlight_back_msg();
}

void backlight::languageReload()
{
    ui->retranslateUi(this);
}

void backlight::setBacklightFont()
{
    QScreen *screen = qApp->primaryScreen();
    g_screenWidth = screen->size().width();
    qreal realX = screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;
    if(realWidth < 15)
    {
        font.setPointSize(12);
    }
    else if (realWidth < 17)
    {
        font.setPointSize(14);
    }
    else
    {
        font.setPointSize(17);
    }

    ui->lbl_lightValue->setFont(font);
    ui->lbl_light->setFont(font);
    ui->cmb_sleep_time->setFont(font);
    ui->lbl_sleepTime->setFont(font);
    ui->lbl_backlightTitle->setFont(font);
    ui->lbl_backlight->setFont(font);
}
