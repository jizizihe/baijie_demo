#include "backlight.h"
#include "ui_backlight.h"

#define qd
#ifdef qd
#define qdebug(format, ...)  qDebug("File:%s, Function:%s, Line:%d  " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define qdebug(format, ...)
#endif

static int light_value;
static int index_number;
static unsigned int timer_array[7] = {15,30,60,120,300,600,429499999};

QTimer *timeUp;

enum index_value
{
    fifteen_se,
    thirty_se,
    one_minute,
    two_minute,
    five_minute,
    ten_minute,
};

int set_backlight(int value)
{
    int fd;
    int source_value;
    unsigned long args[2] = {0} ;
    fd = open("/dev/disp", O_RDWR, 0);
    if(fd < 0)
    {
        qdebug("open /dev/disp failed.\n");
        return -1;
    }

    args[0] = 0;
    args[1] = value;
    source_value = ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args);

    ioctl(fd,DISP_LCD_SET_BRIGHTNESS,args);
    close(fd);
    return source_value;
}

int get_backlight()
{
    int fd;
    int source_value;
    unsigned long args[2] = {0} ;
    fd = open("/dev/disp", O_RDWR, 0);
    if(fd < 0)
    {
        qdebug("open /dev/disp failed.\n");
        return -1;
    }
    source_value = ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args);

    close(fd);
    return source_value;
}


backlight::backlight(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::backlight)
{
    ui->setupUi(this);


    ui->horizontalSlider->setRange(138,255);
    ui->horizontalSlider->setValue(255);
    light_value = 255;

    timing = new QTimer(this);
    timing->start();
    connect(timing,SIGNAL(timeout()),this,SLOT(light_screen()));

    QTimer *show_timer = new QTimer(this);
    show_timer->start(1000);
    connect(show_timer,SIGNAL(timeout()),this,SLOT(show_time()));

    timeUp = new QTimer(this);
    timeUp->start();
    connect(timeUp,SIGNAL(timeout()),this,SLOT(timerUp()));
    index_number = 6;
    ui->comboBox->setCurrentIndex(6);
}

backlight::~backlight()
{
    delete ui;
}

void backlight::show_time()
{
    ui->time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss"));
}

void backlight::light_screen()      //Click on the light screen
{
    int now_value = get_backlight();

    if(now_value == 0 && touch_flag)   //touch_flag external variable in globalapp.h
    {
        timeUp->start();
//        qDebug() << "light_screen" << QTime::currentTime();
        set_backlight(light_value);

    }
    usleep(5000);
}

void backlight::timerUp()       //check whether events are generated
{
    QDateTime now = QDateTime::currentDateTime().addSecs(timer_array[index_number]);
    QDateTime shade = QDateTime::currentDateTime().addSecs((unsigned)timer_array[index_number] - 1);

//    qDebug() << "begin_timing" << QDateTime::currentDateTime() << "now_timing" << now;
    while(QDateTime::currentDateTime() < now)
    {
        if(touch_flag)
        {
            now = QDateTime::currentDateTime().addSecs((unsigned)timer_array[index_number]);
            shade = QDateTime::currentDateTime().addSecs((unsigned)timer_array[index_number] - 1);
            set_backlight(light_value);
            touch_flag = false;
        }

        if(QDateTime::currentDateTime() > shade)
        {
            set_backlight(140);
        }

        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
    timeUp->stop();
    set_backlight(0);
//    qDebug() << "black_screen" << QTime::currentTime();

}

void backlight::on_horizontalSlider_valueChanged(int value)
{
    light_value = set_backlight(value);
}

void backlight::on_comboBox_currentIndexChanged(int index)
{
    index_number = index;
}

void backlight::on_return_2_clicked()
{
    emit Mysignal();
}

void backlight::language_reload()
{
    ui->retranslateUi(this);
}
