#include "all_test_thread.h"

all_test_thread::all_test_thread(QObject *parent) : QObject(parent)
{
    g_stopFlag = false;
    g_serialTimer = new QTimer(this);
    connect(g_serialTimer, SIGNAL(timeout()), this, SLOT(serialTimer_func()));
}

all_test_thread::~all_test_thread()
{
    if(g_serialTimer->isActive() == true)
    {
        g_serialTimer->stop();
    }
}

void all_test_thread::network_test_thread()
{
    QString retResult = pingGateway();
    emit send_test_msg(EnumNetworkSignal,retResult);
}

void all_test_thread::key_test_thread()
{
    QString retResult = tr("---please press the key to test!");
    emit send_test_msg(EnumKeySignal,retResult);
}

void all_test_thread::usb_test_thread(int num)
{
    QString retResult = usbTest(num);
    emit send_test_msg(EnumUsbSignal,retResult);
}

void all_test_thread::rtc_test_thread()
{
    QString retResult;
    QString time,timeFirst,timeSecond;
    time = rtcTest();
    timeFirst = time.mid(20,6);
    time = rtcTest();
    timeSecond = time.mid(20,6);

    if(timeFirst == timeSecond)
    {
        retResult = "failed";
    }
    else
    {
        retResult = "OK";
    }
    emit send_test_msg(EnumRtcSignal,retResult);
}

void all_test_thread::sdcard_test_thread()
{
    QString retResult = getSdCard();
    emit send_test_msg(EnumSdCardSignal,retResult);
}

void all_test_thread::camera_test_thread()
{
    cameraTest();
    emit send_test_msg(EnumCameraSignal,NULL);
}

void all_test_thread::battary_test_thread()
{
    QString retResult = battaryTest();
    emit send_test_msg(EnumBattarySignal,retResult);
}

void all_test_thread::sim_test_thread()
{
    QString retResult = simTest();
    emit send_test_msg(EnumSimSignal,retResult);
}

void all_test_thread::audio_test_thread()
{
    audioTest();
    emit send_test_msg(EnumAudioSignal,"NULL");
}

void all_test_thread::wifi_test_thread()
{
    QString retResult = wifiTest();
    emit send_test_msg(EnumWifiSignal,retResult);
}

void all_test_thread::bluetooth_test_thread()
{
    QString retResult = bluetoothTest();
    emit send_test_msg(EnumBluetoothSignal,retResult);
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
    /* Enable receiber, hang on close, ignore control line */
    newtio.c_cflag |= CREAD | HUPCL | CLOCAL;

    /* Read 1 byte minimun, no timeout specified */
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;

    if (tcsetattr (fd, TCSANOW, &newtio) < 0)
        return 0;

    return 1;
}

void all_test_thread::serial_test_thread_start(serial_config serialTest)
{
    char	*port;
    g_serialConfigPtr = &serialTest;
    int checkId = g_serialConfigPtr->checkedId[g_serialConfigPtr->index];
    g_checkName = g_serialConfigPtr->checkedBtnList.at(checkId)->text();
    //    char    *checkStr = (char *)malloc(sizeof(g_checkName));
    g_mode = g_serialConfigPtr->mode;
    QString portName = QString("/dev/%1").arg(g_checkName);
    port = portName.toLatin1().data();
    g_fd = open (port, O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
    if(g_fd < 0)
    {
        return;
    }
    set_prop(g_fd);
    usleep(10);
    g_serialTimer->start(10);
}

void all_test_thread::serialTimer_func()
{
    QString retResult;
    int		rc;
    char	buf[32];
    QString recvStr;
    char * checkNameStr;
    char checkStr[16];
    QByteArray tmp = g_checkName.toLatin1();
    checkNameStr = tmp.data();
    memcpy(checkStr,checkNameStr,16);

    recvStr = checkStr;
    if(g_stopFlag == true)
        return;

    if(g_mode == "server")
    {
        rc = read(g_fd, buf,sizeof(buf));
        if(rc>0)
        {
            recvStr = buf;
            emit send_test_msg(EnumSerialSignal,recvStr);
            if(!strcasecmp(checkStr,buf))
            {
                write(g_fd,checkStr,sizeof(checkStr));
                recvStr = checkStr;
            }
        }
    }
    else if(g_mode == "client")
    {
        write(g_fd,checkStr,sizeof(g_checkName));
        recvStr = checkStr;
        usleep(100000);
        rc = read(g_fd, buf,sizeof(buf));
        if(rc>0)
        {
            if(!strcasecmp(checkStr,buf))
            {
                retResult = QString("---serial test: %1 OK").arg(g_checkName);
                emit send_test_msg(EnumSerialSignal,retResult);
                g_serialTimer->stop();
                close(g_fd);
                return;
            }
        }
    }
}

void all_test_thread::serial_test_thread_stop()
{
    //    close(g_fd);
    g_stopFlag = true;
    g_serialTimer->stop();
}

