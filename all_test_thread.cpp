#include "all_test_thread.h"

all_test_thread::all_test_thread(QObject *parent) : QObject(parent)
{
    stopFlag = false;

    serialTimer = new QTimer(this);
    connect(serialTimer, SIGNAL(timeout()), this, SLOT(serialTimer_func()));
}

all_test_thread::~all_test_thread()
{
    if(serialTimer->isActive() == true)
    {
        serialTimer->stop();
    }
}

void all_test_thread::network_test_thread()
{
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"Thread构造函数ID:"<<QThread::currentThreadId();
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"time:"<<QTime::currentTime();

    QString retResult = ping_gateway();
    emit send_test_msg(network_signal,retResult);
}

void all_test_thread::key_test_thread()
{
    QString retResult = tr("---please press the key to test!");
    emit send_test_msg(key_signal,retResult);
}

void all_test_thread::usb_test_thread(int num)
{
    QString retResult = usb_test(num);
    emit send_test_msg(usb_signal,retResult);
}

void all_test_thread::rtc_test_thread()
{
    QString retResult;
    QString time,time1,time2;
    time = rtc_test();
    time1 = time.mid(20,6);
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__ << "time" << time << time1;

    time = rtc_test();
    time2 = time.mid(20,6);
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__ << "time" << time << time2;

    if(time1 == time2)
    {
        retResult = "failed";
    }
    else
    {
        retResult = "OK";
    }

    emit send_test_msg(rtc_signal,retResult);
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"time:"<<QTime::currentTime();
}

void all_test_thread::sdcard_test_thread()
{
    QString retResult = get_new_sd();
    emit send_test_msg(sd_card_signal,retResult);
}

void all_test_thread::camera_test_thread()
{
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"Thread构造函数ID:"<<QThread::currentThreadId();
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"time:"<<QTime::currentTime();
    camera_test();
    emit send_test_msg(camera_signal,NULL);
}

void all_test_thread::battary_test_thread()
{
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"Thread构造函数ID:"<<QThread::currentThreadId();
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"time:"<<QTime::currentTime();
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__;
    QString retResult = battary_test();
    emit send_test_msg(battary_signal,retResult);
}

void all_test_thread::sim_test_thread()
{
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"Thread构造函数ID:"<<QThread::currentThreadId();
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"time:"<<QTime::currentTime();
    QString retResult = sim_test();
    emit send_test_msg(sim_signal,retResult);
}

void all_test_thread::audio_test_thread()
{
    QString retResult = audio_test();
    emit send_test_msg(audio_signal,retResult);
}

void all_test_thread::wifi_test_thread()
{
    QString retResult = wifi_test();
    emit send_test_msg(wifi_signal,retResult);
}

void all_test_thread::bluetooth_test_thread()
{
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"Thread构造函数ID:"<<QThread::currentThreadId();
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"time:"<<QTime::currentTime();

    QString retResult = bluetooth_test();
    emit send_test_msg(bluetooth_signal,retResult);
}

int set_prop(int fd)
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

void all_test_thread::serial_test_thread_client(serial_config serialTest)
{
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"Thread构造函数ID:"<<QThread::currentThreadId();
    char	*port;
    serialConfigPtr = &serialTest;
    int checkId = serialConfigPtr->checked_id[serialConfigPtr->index];
    checkName = serialConfigPtr->checkedBtnList.at(checkId)->text();
//    char    *checkStr = (char *)malloc(sizeof(checkName));
    mode = serialConfigPtr->mode;
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "mode:" << serialConfigPtr->mode;
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "id:" << checkId;
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "name:" << checkName;

    QString port_name = QString("/dev/%1").arg(checkName);
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "port_name" << port_name;
    port = port_name.toLatin1().data();
    fd = open (port, O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
    if(fd < 0)
    {
        return;
    }
    set_prop (fd);
    usleep(10);
    serialTimer->start(10);
}

void all_test_thread::serialTimer_func()
{
    QString retResult;
    int		rc;
    char	buf[32];
    QString recvStr;
    char * checkStr_p;
    char checkStr[16];
    QByteArray tmp = checkName.toLatin1();
    checkStr_p = tmp.data();
    memcpy(checkStr,checkStr_p,16);

    recvStr = checkStr;
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "recvStr:" << recvStr;
    if(stopFlag == true)
        return;

    if(mode == "server")
    {
        rc = read(fd, buf,sizeof(buf));
        if(rc>0)
        {
            recvStr = buf;
        //    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "read" << recvStr ;
            emit send_test_msg(serial_signal,recvStr);
            if(!strcasecmp(checkStr,buf))
            {
                write(fd,checkStr,sizeof(checkStr));
                recvStr = checkStr;
//                    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "checkStr" << recvStr ;
            }
        }
    }
    else if(mode == "client")
    {
//        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"Thread构造函数ID:"<<QThread::currentThreadId();
        write(fd,checkStr,sizeof(checkName));
        recvStr = checkStr;
       // qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "write" << recvStr ;
        usleep(100000);
        rc = read(fd, buf,sizeof(buf));
        if(rc>0)
        {
            if(!strcasecmp(checkStr,buf))
            {
                //qDebug() << "Line:" << __LINE__<< "-------ok:";
                retResult = QString("---serial test: %1 OK").arg(checkName);
                emit send_test_msg(serial_signal,retResult);
//                emit serial_test_msg(retResult);
                serialTimer->stop();
                close(fd);
                return;
            }
        }
    }
}

void all_test_thread::serial_test_thread_stop()
{
   // qDebug() << "Line:" << __LINE__<< "__FUNCTION__" << __FUNCTION__<<"Thread构造函数ID:"<<QThread::currentThreadId();
   //    close(fd);
    stopFlag = true;
    serialTimer->stop();
}

