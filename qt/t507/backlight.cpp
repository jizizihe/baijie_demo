#include "backlight.h"
#include "ui_backlight.h"

static int index_number;
static int index_number_flag = -1;
static int array[6] = {15,30,60,120,300,600};

enum index_value
{
    fifteen_se,
    thirty_se,
    one_minute,
    two_minute,
    five_minute,
    ten_minute,
};

void set_backlight(int value)
{
    int fd;
    unsigned long args[2] = {0} ;
    fd = open("/dev/disp", O_RDWR, 0);
    if(fd < 0)
    {
        qDebug() << "open /dev/disp failed.\n";
        return ;
    }

    args[0] = 0;
    args[1] = value;
    qDebug() << "the old lcd" << args[1] << "brightness is" << ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args) << endl;
    ioctl(fd,DISP_LCD_SET_BRIGHTNESS,args);
    qDebug() << "the new lcd" << args[1] << "brightness is" << ioctl(fd,DISP_LCD_GET_BRIGHTNESS,args) << endl;
    close(fd);
    return ;
}


backlight::backlight(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::backlight)
{
    ui->setupUi(this);
    ui->return_2->setGeometry(10,10,200,30);
    ui->horizontalSlider->setRange(140,255);
    ui->horizontalSlider->setValue(200);
    int str ;
    str = ui->comboBox->currentIndex();
    qDebug() << str;
}

backlight::~backlight()
{
    delete ui;
}

void backlight::on_horizontalSlider_valueChanged(int value)
{
    qDebug() << value;
    set_backlight(value);
}

void backlight::on_normal_clicked()
{
     ui->horizontalSlider->setValue(180);
}

void backlight::on_comboBox_currentIndexChanged(int index)
{
    index_number = index;
    qDebug() << "11111 from comboBox" << index_number;
}

void backlight::on_sure_clicked()
{
    timer = new QTimer(this);
    qDebug() << "22222 get comboBox index" << index_number;
    switch(index_number)
    {
    case fifteen_se :
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
            qDebug() << "fifteen_se  begin";
            //             connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate(int)));
            //             timer->start(1000);
            timerUpDate(15);
            set_backlight(0);
            qDebug() << "-------fifteen_se   end";
        }
        break;

    case thirty_se :
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
            qDebug() << "thirty_se begin";
            timerUpDate(30);
            set_backlight(0);
            qDebug() << "----------thirty_se  end";

        }
        break;

    case one_minute :
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
            qDebug() << "one_minute  begin";
            timerUpDate(60);
            set_backlight(0);
            qDebug() << "-------one_minute  end";
        }
        break;

    case two_minute:
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
            qDebug() << "two_minute begin";
            timerUpDate(120);
            set_backlight(0);
            qDebug() << "two_minute end";
        }
        break;

    case five_minute:
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
            qDebug() << "five_minute begin";
            timerUpDate(300);
            set_backlight(0);
            qDebug() << "five_minute end";
        }
        break;

    case ten_minute:
        if(index_number != index_number_flag)
        {
            index_number_flag = index_number;
            qDebug() << "ten_minute begin";
            timerUpDate(600);
            set_backlight(0);
            qDebug() << "ten_minute end";
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
