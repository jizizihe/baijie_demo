#include "backlight.h"
#include "ui_backlight.h"

static int global_secs = 6;
static int light_value;
static int index_number;
static int index_number_flag = -1;
static int array[7] = {15,30,60,120,300,600,-1};

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
        qDebug() << "open /dev/disp failed.\n";
        return -1;
    }

    args[0] = 0;
    args[1] = value;
//    qDebug() << "the old lcd" << args[1] << "brightness is" << ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args) << endl;
    source_value = ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args);

    ioctl(fd,DISP_LCD_SET_BRIGHTNESS,args);
//    qDebug() << "the new lcd" << args[1] << "brightness is" << ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args) << endl;
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
        qDebug() << "open /dev/disp failed.\n";
        return -1;
    }
//    qDebug() << "the old lcd" << args[1] << "brightness is" << ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args) << endl;
    source_value = ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args);
//    qDebug() <<"get_backlight:" << source_value;

    close(fd);
    return source_value;
}


backlight::backlight(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::backlight)
{
    extern bool touch_flag;
    ui->setupUi(this);
    ui->return_2->setGeometry(10,10,200,30);
    ui->horizontalSlider->setRange(140,255);
    ui->horizontalSlider->setValue(200);
    light_value = 200;
    int str ;
    str = ui->comboBox->currentIndex();
    qDebug() << str;

    timing = new QTimer(this);
    timing->start(1);
    connect(timing,SIGNAL(timeout()),this,SLOT(light_screen()));

//    QTimer *timeUp = new QTimer(this);
//    timeUp->start(3000);
//    connect(timeUp,SIGNAL(timeout()),this,SLOT(timerUp()));


}

backlight::~backlight()
{
    delete ui;
}

void backlight::light_screen()
{
    int now_value = get_backlight();

    if(now_value == 0 && touch_flag)
    {
        timeUp->start(3000);
        set_backlight(light_value);

    }
}

void backlight::timerUp()
{
    QTime now = QTime::currentTime().addSecs(array[global_secs]);

    while(QTime::currentTime() < now)
    {
        if(touch_flag)
        {
            now = QTime::currentTime().addSecs(array[global_secs]);
            touch_flag = false;
        }
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
    set_backlight(0);
    timeUp->stop();
}

void backlight::on_horizontalSlider_valueChanged(int value)
{
//    qDebug() << value;
    light_value = set_backlight(value);
}

void backlight::on_normal_clicked()
{
     ui->horizontalSlider->setValue(180);
     light_value = 180;
}

void backlight::on_comboBox_currentIndexChanged(int index)
{
    index_number = index;
//    qDebug() << "11111 from comboBox" << index_number;
}

void backlight::on_sure_clicked()
{
    timeUp = new QTimer(this);
    timeUp->start(3000);
    connect(timeUp,SIGNAL(timeout()),this,SLOT(timerUp()));

    global_secs = index_number;
    switch(index_number)
    {
    case fifteen_se :
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
//            qDebug() << "fifteen_se  begin";
            //             connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate(int)));
            //             timer->start(1000);
            timerUpDate(15);
            set_backlight(0);
//            qDebug() << "-------fifteen_se   end";
        }
        break;

    case thirty_se :
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
//            qDebug() << "thirty_se begin";
            timerUpDate(30);
            set_backlight(0);
//            qDebug() << "----------thirty_se  end";

        }
        break;

    case one_minute :
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
//            qDebug() << "one_minute  begin";
            timerUpDate(60);
            set_backlight(0);
//            qDebug() << "-------one_minute  end";
        }
        break;

    case two_minute:
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
//            qDebug() << "two_minute begin";
            timerUpDate(120);
            set_backlight(0);
//            qDebug() << "two_minute end";
        }
        break;

    case five_minute:
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
//            qDebug() << "five_minute begin";
            timerUpDate(300);
            set_backlight(0);
//            qDebug() << "five_minute end";
        }
        break;

    case ten_minute:
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
//            qDebug() << "ten_minute begin";
            timerUpDate(600);
            set_backlight(0);
//            qDebug() << "ten_minute end";
        }
        break;

    default:
            index_number_flag = index_number;
        break;

    }
}

void backlight::timerUpDate(int secs)
{
    QTime time = QTime::currentTime();
    QTime now = QTime::currentTime().addSecs(secs);
    //QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    QString str = time.toString("hh:mm:ss");
    //ui->label->setText(str);
    qDebug() << str;
    while(QTime::currentTime() < now)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    str = now.toString("hh:mm:ss");
    //ui->label->setText(str);
    qDebug() << str;
}

void backlight::on_return_2_clicked()
{
    emit Mysignal();
}
