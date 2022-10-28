#include "backlight.h"
#include "ui_backlight.h"
#include <QScreen>

#define qd
#ifdef qd
#define qdebug(format, ...)  qDebug("File:%s, Function:%s, Line:%d  " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define qdebug(format, ...)
#endif

static int lightValue;
static int indexNumber;
static int screenFlag;
static int screenWidth;
static int screenHeight;
static unsigned int timerArray[7] = {15,30,60,120,300,600,429499999};   //Sleep time
static QScreen *screen;
static QString strUsbStatus;
static QString strBtnSetUsb;
QTimer *timeUp;

int setBacklight(int value)
{
    int fd;
    int sourceValue;
    unsigned long args[2] = {0} ;
    fd = open("/dev/disp", O_RDWR, 0);
    if(fd < 0)
    {
        qdebug("open /dev/disp failed.\n");
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
        qdebug("open /dev/disp failed.\n");
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
    lightValue = 255;
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    backlightFont();
    ui->lbl_lightValue->setText(tr("255"));
    timing = new QTimer(this);
    timing->start();
    connect(timing,SIGNAL(timeout()),this,SLOT(light_screen()));
    timeUp = new QTimer(this);
    timeUp->start();
    connect(timeUp,SIGNAL(timeout()),this,SLOT(timer_up()));
    indexNumber = 6;
    ui->cmb_sleep_time->setCurrentIndex(6);
}

backlight::~backlight()
{
    delete ui;
}

void backlight::light_screen()
{
    int nowValueLight = getBacklight();
    if(nowValueLight == 0 && touchFlag)   //touchFlag external variable in globalapp.h
    {
        timeUp->start();
        setBacklight(lightValue);

    }
    usleep(5000);
}

void backlight::timer_up()       //check whether events are generated
{
    QDateTime now = QDateTime::currentDateTime().addSecs(timerArray[indexNumber]);
    QDateTime shade = QDateTime::currentDateTime().addSecs((unsigned)timerArray[indexNumber] - 1);

    while(QDateTime::currentDateTime() < now)
    {
        if(touchFlag)
        {
            now = QDateTime::currentDateTime().addSecs((unsigned)timerArray[indexNumber]);
            shade = QDateTime::currentDateTime().addSecs((unsigned)timerArray[indexNumber] - 1);
            touchFlag = false;
        }

        if(QDateTime::currentDateTime() > shade)
        {
        }

        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
    timeUp->stop();
    setBacklight(0);
}

void backlight::on_horizontalSlider_light_valueChanged(int value)
{
    lightValue = setBacklight(value);
    QString strValue = QString("%1").arg(value);
    ui->lbl_lightValue->setText(tr(QString("%1").arg(strValue).toUtf8()));
}

void backlight::on_cmb_sleep_time_currentIndexChanged(int index)
{
    indexNumber = index;
}

void backlight::on_btn_ret_clicked()
{
    emit backlight_back_msg();
}

void backlight::languageReload()
{
    ui->retranslateUi(this);
}

void backlight::backlightFont()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = screenWidth / realX * 2.54;
    qreal realHeight = screenHeight / realY *2.54;
    QFont font;
    if(screenFlag)
    {
        if(realHeight < 15)
        {
            font.setPointSize(12);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(14);
        }
        else
        {
            font.setPointSize(17);
        }
    }
    else
    {
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
    }

    ui->lbl_lightValue->setFont(font);
    ui->lbl_light->setFont(font);
    ui->cmb_sleep_time->setFont(font);
    ui->lbl_sleepTime->setFont(font);
    ui->lbl_backlightTitle->setFont(font);
    ui->lbl_backlight->setFont(font);
}
