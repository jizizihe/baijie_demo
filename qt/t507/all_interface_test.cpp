#include "all_interface_test.h"
#include "ui_all_interface_test.h"

#include "udev_interface.h"
#include "gpio_interface.h"

#include <unistd.h>

#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <QTime>
#include <QMovie>
#include <QRect>
#include<QScreen>
#include <QLineEdit>
#include <QHBoxLayout>

QTimer *enable_set;
QTimer *all_check;
int dex  = -1;
bool check_flag[10];

QLabel *ll;
QMovie *w_movie;


void camera_interface()
{
    QProcess *proc = new QProcess();

    proc->start("bash", QStringList() << "-c" << "rm /data/yuv.jpg");
    proc->waitForFinished(-1);

    proc->start("bash",QStringList() << "-c" << "cd /data && csi_test_mplane");
    proc->waitForFinished(-1);

    proc->close();
    delete proc;
    proc = 0;    

}


all_interface_test::all_interface_test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::all_interface_test)
{
    ui->setupUi(this);

    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();

    ll = new QLabel(this);
    //LoadLabel->resize(100,100);
    ll->move(screen_width/2 - 150,screen_height/2 -50);
    w_movie = new QMovie(":/t507_button_image/loading.webp");
    ll->setFixedSize(50, 50);
    ll->setScaledContents(true);
    ll->setMovie(w_movie);
    //pMovie->start();

    serialDialog = new serialdialog(this);


    ui->check_all->setChecked(Qt::Unchecked);

    enable_set = new QTimer();
    connect(enable_set,SIGNAL(timeout()),this,SLOT(time_test()));

    all_check = new QTimer();
    connect(all_check,SIGNAL(timeout()),this,SLOT(all_checkset()));
    all_check->start();

    connect(serialDialog,SIGNAL(serial_config_msg(serial_config)),this,SLOT(serial_config_func(serial_config)));
}

all_interface_test::~all_interface_test()
{
    delete ui;
    delete serialDialog;
}

void all_interface_test::on_return_2_clicked()
{
    ui->textEdit->clear();
    ui->image->clear();
    serialDialog->close();
    emit Mysignal();
}

void all_interface_test::on_test_clicked()
{
    ui->textEdit->clear();
    ui->textEdit->append(tr("1. Note that the buzzer has or does not have sound!!"));

    int gpio_port = calc_port_num('b',8);
    gpio_export(gpio_port);
    gpio_set_state(gpio_port,(char *)"out");
    gpio_set_value(gpio_port,1);
//    sleep(1);

    gpio_set_value(gpio_port,0);
    gpio_unexport(gpio_port);


    if(get_new_usb() == tr("USB Failed"))
        ui->textEdit->append(QString(tr("%1 %2")).arg("2.").arg(get_new_usb()));
    else
        ui->textEdit->append(tr("2. USB OK"));

    if(get_new_sd() == tr("SD Failed"))
        ui->textEdit->append(QString(tr("%1 %2")).arg("3.").arg(get_new_sd()));
    else
        ui->textEdit->append(tr("3. SD OK"));

    if(get_new_sim() == tr("SIM Failed"))
        ui->textEdit->append(QString(tr("%1 %2")).arg("4.").arg(get_new_sim()));
    else
        ui->textEdit->append(tr("4. SIM OK"));

    if(ping_gateway() == tr("OK"))
        ui->textEdit->append(tr("5. NETWORK OK"));
    else
        ui->textEdit->append(tr("5. NETWORK Failed"));


    camera_interface();
    QImage image = QImage("/data/yuv.jpg");
    ui->image->setScaledContents(true);
    ui->image->setPixmap(QPixmap::fromImage(image));
    ui->image->show();
    ui->textEdit->append(tr("6. Whether the picture is displayed properly?"));

}

void all_interface_test::on_clear_clicked()
{
    ui->textEdit->clear();
    ui->image->clear();
}

void all_interface_test::on_pushButton_3_clicked()
{
    QProcess *proc = new QProcess();

    proc->start("bash", QStringList() << "-c" << "rm /data/yuv.jpg");
    proc->waitForFinished();

    proc->start("bash",QStringList() << "-c" << "cd /data && csi_test_mplane");
    proc->waitForFinished();

    proc->close();
    delete proc;
    proc = 0;
}


void all_interface_test::on_pushButton_4_clicked()
{
    QPixmap pix;
    QImage image = QImage("/data/yuv.jpg");
//    image->load("./*.jpg");
    ui->image->setPixmap(pix.fromImage(image));
    ui->image->setScaledContents(true);
    ui->image->show();
}

void all_interface_test::language_reload()
{
    ui->retranslateUi(this);
}

void all_interface_test::click_str()
{
    if(dex != 0) ui->gpio->setEnabled(false);
    if(dex != 1) ui->wifi->setEnabled(false);
    if(dex != 2) ui->bluetooth->setEnabled(false);
    if(dex != 3) ui->network->setEnabled(false);
    if(dex != 4) ui->sim->setEnabled(false);
    if(dex != 5) ui->usb->setEnabled(false);
    if(dex != 6) ui->sd_card->setEnabled(false);
    if(dex != 7) ui->key->setEnabled(false);
    if(dex != 8) ui->camera->setEnabled(false);
}

void all_interface_test::time_test()
{
    ui->gpio->setEnabled(true);
    if(dex == 1)ui->wifi->setCheckable(true);
    ui->wifi->setEnabled(true);
    if(dex == 2)ui->bluetooth->setCheckable(true);
    ui->bluetooth->setEnabled(true);
    ui->network->setEnabled(true);
    ui->sim->setEnabled(true);
    ui->usb->setEnabled(true);
    ui->sd_card->setEnabled(true);
    ui->key->setEnabled(true);
    ui->camera->setEnabled(true);
    usleep(5000);
    enable_set->stop();
    dex = -1;
}

void all_interface_test::all_checkset()
{
    bool flag = true;
    for(int i = 0; i < 10; i++)
    {
//        qDebug() << "all [" << i << "]:" << check_flag[i] << __LINE__;
        if(!check_flag[i])
        {
            flag = false;
            ui->check_all->setChecked(Qt::Unchecked);
            all_flag = false;
            break;
        }
    }
    if(flag)
    {
        qDebug() << "check_all checked!!!!!"  << all_flag << __LINE__;
        ui->check_all->setChecked(Qt::Checked);
        all_flag = true;

    }
    usleep(5000);
    all_check->stop();
}

void all_interface_test::on_gpio_clicked()
{
    ui->textEdit->clear();
    dex = 0;
    click_str();
    ui->textEdit->append(tr("1. Note that the buzzer has or does not have sound!!"));

    int gpio_port = calc_port_num('b',8);
    gpio_export(gpio_port);
    gpio_set_state(gpio_port,(char *)"out");
    gpio_set_value(gpio_port,1);
    sleep(1);
    gpio_set_value(gpio_port,0);
    gpio_unexport(gpio_port);
    enable_set->start(1000);
}

void all_interface_test::on_wifi_clicked()
{
    ui->wifi->setCheckable(true);
    ui->textEdit->clear();
    dex = 1;

//    click_str();


    if(wifi_test() == tr("OK"))
        ui->textEdit->append(tr("2. WIFI OK"));
    else
        ui->textEdit->append(tr("2. WIFI Failed"));
//    enable_set->start(1000);
//    QTimer *temp = new QTimer();
//    temp->start(1000);
//    temp->stop();

}

void all_interface_test::on_bluetooth_clicked()
{
    ui->textEdit->clear();
    dex = 2;
    click_str();
    enable_set->start(1000);
}

void all_interface_test::on_network_clicked()
{
    ui->textEdit->clear();
    dex = 3;
    click_str();
    if(ping_gateway() == tr("OK"))
        ui->textEdit->append(tr("4. NETWORK OK"));
    else
        ui->textEdit->append(tr("4. NETWORK Failed"));
    enable_set->start(500);
}


void all_interface_test::on_sim_clicked()
{
    ui->textEdit->clear();
    dex = 4;
    click_str();
    if(get_new_sim() == tr("SIM Failed"))
        ui->textEdit->append(QString(tr("%1 %2")).arg("5.").arg(get_new_sim()));
    else
        ui->textEdit->append(tr("5. SIM OK"));

    enable_set->start(500);
}


void all_interface_test::on_usb_clicked()
{
    ui->textEdit->clear();
    dex = 5;
    click_str();
    if(get_new_usb() == tr("USB Failed"))
        ui->textEdit->append(QString(tr("%1 %2")).arg("6.").arg(get_new_usb()));
    else
        ui->textEdit->append(tr("6. USB OK"));


    enable_set->start(1000);
}

void all_interface_test::on_sd_card_clicked()
{
    ui->textEdit->clear();
    dex = 6;
    click_str();
    if(get_new_sd() == tr("SD Failed"))
        ui->textEdit->append(QString(tr("%1 %2")).arg("7.").arg(get_new_sd()));
    else
        ui->textEdit->append(tr("7. SD OK"));
    enable_set->start(1000);
}

void all_interface_test::on_key_clicked()
{
    ui->textEdit->clear();
    dex = 7;
    click_str();

    enable_set->start(1000);
}


void all_interface_test::on_camera_clicked()
{
    ui->textEdit->clear();
    dex = 8;
    click_str();
    camera_interface();
    QImage image = QImage("/data/yuv.jpg");
    ui->image->setScaledContents(true);
    ui->image->setPixmap(QPixmap::fromImage(image));
    ui->image->show();
    ui->textEdit->append(tr("9. Whether the picture is displayed properly?"));
//    QString temp  = ui->image->
//    if(temp.isEmpty())
//    {
//        qDebug() << "temp is empty !!!!";
//    }
    enable_set->start(1000);
}

void all_interface_test::on_check_all_clicked()
{
    static bool flag = true;
    all_flag = false;
    if(flag)
    {
        all_flag = true;
        if(!gpio_flag)on_auto_gpio_clicked();
        if(!wifi_flag)on_auto_wifi_clicked();
        if(!blue_flag)on_auto_bluetooth_clicked();
        if(!usb_flag)on_auto_usb_clicked();
        if(!sd_card_flag)on_auto_sd_card_clicked();
        if(!network_flag)on_auto_network_clicked();
        if(!sim_flag)on_auto_sim_clicked();
        if(!key_flag)on_auto_key_clicked();
        if(!camera_flag)on_auto_camera_clicked();
    }
    else
    {
        if(gpio_flag)on_auto_gpio_clicked();
        if(wifi_flag)on_auto_wifi_clicked();
        if(blue_flag)on_auto_bluetooth_clicked();
        if(usb_flag)on_auto_usb_clicked();
        if(sd_card_flag)on_auto_sd_card_clicked();
        if(network_flag)on_auto_network_clicked();
        if(sim_flag)on_auto_sim_clicked();
        if(key_flag)on_auto_key_clicked();
        if(camera_flag)on_auto_camera_clicked();
    }
    flag = !flag;
}

void all_interface_test::on_auto_gpio_clicked()
{
    all_check->start();
    static bool flag = true;
    gpio_flag = false;
    if(flag)
    {
        gpio_flag = true;
        check_flag[0] = true;
        ui->auto_gpio->setChecked(Qt::Checked);
    }
    else
    {
        check_flag[0] = false;
        ui->auto_gpio->setChecked(Qt::Unchecked);
    }
    flag = !flag;
}

void all_interface_test::on_auto_wifi_clicked()
{
    all_check->start();
    static bool flag = true;
    wifi_flag = false;
    if(flag)
    {
        wifi_flag = true;
        check_flag[1] = true;
        ui->auto_wifi->setChecked(Qt::Checked);
    }
    else
    {
        check_flag[1] = false;
        ui->auto_wifi->setChecked(Qt::Unchecked);
    }
    flag = !flag;
}

void all_interface_test::on_auto_bluetooth_clicked()
{
    all_check->start();
    static bool flag = true;
    blue_flag = false;
    if(flag)
    {
        blue_flag = true;
        check_flag[2] = true;
        ui->auto_bluetooth->setChecked(Qt::Checked);
    }
    else
    {
        check_flag[2] = false;
        ui->auto_bluetooth->setChecked(Qt::Unchecked);
    }
    flag = !flag;
}

void all_interface_test::on_auto_network_clicked()
{
    all_check->start();
    static bool flag = true;
    network_flag = false;
    if(flag)
    {
        check_flag[3] = true;
        network_flag = true;
        ui->auto_network->setChecked(Qt::Checked);
    }
    else
    {
        check_flag[3] = false;
        ui->auto_network->setChecked(Qt::Unchecked);
    }
    flag = !flag;
}

void all_interface_test::on_auto_sim_clicked()
{
    all_check->start();
    static bool flag = true;
    sim_flag = false;
    if(flag)
    {
        check_flag[4] = true;
        sim_flag = true;
        ui->auto_sim->setChecked(Qt::Checked);
    }
    else
    {
        check_flag[4] = false;
        ui->auto_sim->setChecked(Qt::Unchecked);
    }
    flag = !flag;
}


void all_interface_test::on_auto_usb_clicked()
{
    all_check->start();
    static bool flag = true;
    usb_flag = false;
    if(flag)
    {
        check_flag[5] = true;
        usb_flag = true;
        ui->auto_usb->setChecked(Qt::Checked);
    }
    else
    {
        check_flag[5] = false;
        ui->auto_usb->setChecked(Qt::Unchecked);
    }
    flag = !flag;
}

void all_interface_test::on_auto_sd_card_clicked()
{
    all_check->start();
    static bool flag = true;
    sd_card_flag = false;
    if(flag)
    {
        check_flag[6] = true;
        sd_card_flag = true;
        ui->auto_sd_card->setChecked(Qt::Checked);
    }
    else
    {
        check_flag[6] = false;
        ui->auto_sd_card->setChecked(Qt::Unchecked);
    }
    flag = !flag;
}



void all_interface_test::on_auto_key_clicked()
{
    all_check->start();
    static bool flag = true;
    key_flag = false;
    if(flag)
    {
        check_flag[7] = true;
        key_flag = true;
        ui->auto_key->setChecked(Qt::Checked);
    }
    else
    {
        check_flag[7] = false;
        ui->auto_key->setChecked(Qt::Unchecked);
    }
    flag = !flag;
}

void all_interface_test::on_auto_camera_clicked()
{
    all_check->start();
    static bool flag = true;
    camera_flag = false;
    if(flag)
    {
        check_flag[8] = true;
        camera_flag = true;
        ui->auto_camera->setChecked(Qt::Checked);
    }
    else
    {
        check_flag[8] = false;
        ui->auto_camera->setChecked(Qt::Unchecked);
    }
    flag = !flag;
}

void all_interface_test::on_auto_serial_clicked()
{
    all_check->start();
    static bool flag = true;
    serial_flag = false;

#if 0
    for(int i = 0;i < serialPortName.size(); i++)
    {
        tmp = serialPortName.at(i);
        QListWidgetItem * item = new QListWidgetItem(serialDialog->ui->serialPortListWidget);
        QCheckBox *checkbox = new QCheckBox;
        checkbox->setText(QString(tmp));
        //qDebug() << "Line:" << __LINE__<< "FILE:"  << __FILE__ << "tmp:" << tmp;
        serialDialog->ui->serialPortListWidget->addItem(item);
        serialDialog->ui->serialPortListWidget->setItemWidget(item,checkbox);
    }
#endif

    if(flag)
    {
        serialDialog->exec();
        check_flag[9] = true;
        serial_flag = true;
        ui->auto_serial->setChecked(Qt::Checked);
    }
    else
    {
        check_flag[9] = false;
        ui->auto_serial->setChecked(Qt::Unchecked);
    }
    flag = !flag;
}

int all_interface_test::set_prop(int fd)
{
    struct termios newtio;

    memset (&newtio, 0, sizeof (newtio));
    /* Set the bitrate */
    cfsetospeed(&newtio, B115200);
    cfsetispeed(&newtio, B115200);

    newtio.c_cflag |= CS8;

    /* Set the parity */
    newtio.c_cflag &= ~PARENB;

    /* Set the number of stop bits */
    newtio.c_cflag &= (~CSTOPB);

    /* Selects raw (non-canonical) input and output */
    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    newtio.c_oflag &= ~OPOST;
    newtio.c_iflag |= IGNPAR;
    /* Ignore parity errors!!! Windows driver does so why shouldn't I? */
    /* Enable receiber, hang on close, ignore control line */
    newtio.c_cflag |= CREAD | HUPCL | CLOCAL;

    /* Read 1 byte minimun, no timeout specified */
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;

    if (tcsetattr (fd, TCSANOW, &newtio) < 0)
        return 0;

    return 1;
}

void all_interface_test::serial_config_func(serial_config config)
{
    serialConfig = config;
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "name:" << serialConfig.checkedName;
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "mode:" << serialConfig.mode;
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "num:" << serialConfig.count;

    if(serialConfig.mode == "client")
    {
        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "client";

        serial_thread(serialConfig);
    }
    else if(serialConfig.mode == "service")
    {
        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "seriver";

        serial_thread(serialConfig);
    }

}


void all_interface_test::serial_thread(serial_config config)
{
    serialConfig = config;

    pthread_t thread_id = 0;
    pthread_attr_t  attr;
    int	i;

    //int *port_num_ptr = new int();
    if(thread_id)
    {
        pthread_cancel(thread_id);
    }
    pthread_attr_init(&attr);
    for(i = 0; i < serialConfig.count; i++)
    {
        serialConfig.index = i;
        if (pthread_create(&thread_id, &attr, thread_serial_port,(void *)&serialConfig))
        {
            printf("ERROR: can't create read_thread thread!\n");
        }
        else
        {
            pthread_detach(thread_id);
        }
        QThread::msleep(50);
    }
    //sleep(5);

    pthread_attr_destroy(&attr);
    return;
}

void *all_interface_test::thread_serial_port(void * date)
{
    int		fd;
    int		rc;
    int     com;
    char	buf[10];
    char	check_char;
    char	*port;

    serial_config * serialConfigPtr = (serial_config *)date;
    int checkId = serialConfigPtr->checked_id[serialConfigPtr->index];
    QString checkName = serialConfigPtr->checkedBtnList.at(checkId)->text();


    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "mode:" << serialConfigPtr->mode;
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "id:" << checkId;
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "name:" << checkName;

    //qDebug() << "part_num = "  << port_num;
    QString port_name = QString("/dev/%1").arg(checkName);
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "port_name" << port_name;
    port = port_name.toLatin1().data();
    //cout << "port-:" <<  port <<"  line = " << __LINE__ << endl;

    fd = open (port, O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
    if(fd < 0)
    {
        //printf("open serial port (%s) failed\n", port_name_array[port_num]);
        return NULL;
    }

    set_prop (fd);

    usleep(10);

    check_char = checkId + 0x30;
#if 0
    switch(checkId)
    {
    case 0:
        check_char = '0';
        com = 0;
        break;
    case 1:
        check_char = '0';
        com = 1;
        break;
    case 2:
        check_char = '1';
        com = 2;
        break;
    case 3:
        check_char = '3';
        com = 3;
        break;
    }
#endif
    qDebug() << "-------in--------\n";
    while(1)
    {
        if(serialConfigPtr->mode == "service")
        {
            rc = read(fd, buf, 1);
            QString ag = buf;
            //qDebug()<<"ag = "<<ag << port_num;
            //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "ag" << ag ;
            if(rc>0)
            {
                QString readBuf = buf;
                qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "readBuf" << readBuf ;
                if(buf[0] == check_char)
                {
                    //port_check_stat |= (1<<port_num);
                    buf[0] = checkId + 0x30;
                    write(fd, buf, 1);
                    QString ss = buf;
                    qDebug()<<"ss = "<<ss;
                    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "buf:" << ss;
                    //printf("COM=: %d dev=:%s test ok. buf:=%d \n", com, port_name_array[port_num], buf[0]);
                }
            }

        }
        else if(serialConfigPtr->mode == "client")
        {
            buf[0] = checkId + 0x30;
            write(fd, buf, 1);
            QString ag = buf;
            qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "ag" << ag <<"checkId:" << checkId;
            usleep(500000);
            rc = read(fd, buf, 1);

            if(rc>0)
            {
                QString ss = buf;
                qDebug()<<"ss = "<<ss;

                if(buf[0] == check_char)
                {
                    //port_check_stat |= (1<<port_num);
                    qDebug() << "-------ok:"<< port << endl;
                    //printf("COM=: %d dev=:%s test ok. \n", com, port_name_array[port_num]);
                    return NULL;
                }
            }
        }
    }
    return NULL;
}

void all_interface_test::on_begin_clicked()
{
    w_movie->start();

    ui->textEdit->clear();
    ui->image->clear();
    if(gpio_flag)
    {
        ui->textEdit->append(tr("1. Note that the buzzer has or does not have sound!!"));

        qDebug() << "gpio_flag" << __LINE__;
        int gpio_port = calc_port_num('b',8);
        gpio_export(gpio_port);
        gpio_set_state(gpio_port,(char *)"out");
        gpio_set_value(gpio_port,1);
        sleep(1);
        gpio_set_value(gpio_port,0);
        gpio_unexport(gpio_port);
        enable_set->start(1000);
    }
    if(wifi_flag)
    {
        qDebug() << "wifi_flag" << __LINE__;
        if(wifi_test() == tr("OK"))
            ui->textEdit->append(tr("2. WIFI OK"));
        else
            ui->textEdit->append(tr("2. WIFI Failed"));
    }
    if(blue_flag)
    {
        qDebug() << "blue_flag" << __LINE__;
    }
    if(network_flag)
    {
        qDebug() << "network_flag" << __LINE__;
        if(ping_gateway() == tr("OK"))
            ui->textEdit->append(tr("4. NETWORK OK"));
        else
            ui->textEdit->append(tr("4. NETWORK Failed"));
        enable_set->start(1000);
    }
    if(sim_flag)
    {
        if(get_new_sim() == tr("SIM Failed"))
            ui->textEdit->append(QString(tr("%1 %2")).arg("5.").arg(get_new_sim()));
        else
            ui->textEdit->append(tr("5. SIM OK"));
    }
    if(usb_flag)
    {
        qDebug() << "usb_flag" << __LINE__;
        if(get_new_usb() == tr("USB Failed"))
            ui->textEdit->append(QString(tr("%1 %2")).arg("6.").arg(get_new_usb()));
        else
            ui->textEdit->append(tr("6. USB OK"));
    }
    if(sd_card_flag)
    {
        qDebug() << "sd_card_flag" << __LINE__;
        if(get_new_sd() == tr("SD Failed"))
            ui->textEdit->append(QString(tr("%1 %2")).arg("7.").arg(get_new_sd()));
        else
            ui->textEdit->append(tr("7. SD OK"));
    }
    if(key_flag)
    {
        qDebug() << "key_flag" << __LINE__;
    }

    if(camera_flag)
    {
        qDebug() << "camera_flag" << __LINE__;
        camera_interface();
        QImage image = QImage("/data/yuv.jpg");
        ui->image->setScaledContents(true);
        ui->image->setPixmap(QPixmap::fromImage(image));
        ui->image->show();
        ui->textEdit->append(tr("9. Whether the picture is displayed properly?"));
    }

//    w_movie->stop();
    ll->clear();

}

bool all_interface_test::event(QEvent *event)
{
    if(event->type() == QEvent::KeyPress)  //键盘按下处理,其他事件让事件处理器自己处理,不能返回false
    {
        if(key_flag == true)
        {
            qDebug()<<"key pressed!";
            ui->textEdit->append(tr("8. key pressed!"));
            return true;
        }
        else
        {
            return QWidget::event(event);
        }
    }
    else
    {
        return QWidget::event(event);
    }
}



