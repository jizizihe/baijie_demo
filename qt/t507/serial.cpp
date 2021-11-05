#include "serial.h"
#include "ui_serial.h"
#include <QThread>


using namespace std;
//QStringList m_serialPortName;
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

//int port_num = -1;
//static void *thread_serial_port(void *);
//int set_prop(int fd);
//int port_check_stat = 0, count = 0;
//static char *port_name_array[5] = {
////    "/dev/ttyS0",
////    "/dev/ttyS1",
////    "/dev/ttyS2",
////        "/dev/ttyS4",
////        "/dev/ttyS3",
////        "/dev/ttyS2",
////        "/dev/ttyXRUSB0",
////        "/dev/ttyXRUSB3",
////        "/dev/ttyXRUSB2",
////        "/dev/ttyXRUSB1",
////        "/dev/ttyUSB0"
// };
//void serial_thread();

serial::serial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serial)
{
    ui->setupUi(this);

    QTextStream cout(stdout);//绑定cout到标准输出

    retBt = new QPushButton(this);
    retBt->setFixedSize(100,40);
//    retBt->setText(tr("return"));
    retBt->setIcon(QIcon(":/t507_button_image/return.png"));
    retBt->move(10,10);

    TitleLabel = new QLabel(this);
    TitleLabel->setText("Serial Test");
    TitleLabel->resize(200,100);
    TitleLabel->move(400,20);
    QFont ft;
    ft.setPointSize(18);
    TitleLabel->setFont(ft);

    ModeChooseLabel = new QLabel(this);
    ModeChooseLabel->setText("Mode Choose");
    ModeChooseLabel->resize(200,50);
    ModeChooseLabel->move(50,150);

    PortChooseLabel = new QLabel(this);
    PortChooseLabel->setText("Port Choose");
    PortChooseLabel->resize(200,50);
    PortChooseLabel->move(50,250);

    ModeChooseBox = new QComboBox(this);
    ModeChooseBox->resize(250,50);
    ModeChooseBox->move(250,150);
    ModeChooseBox->addItem("MultiBoard Mode");
    ModeChooseBox->addItem("Local Mode");

    PortChooseBox = new QComboBox(this);
    PortChooseBox->resize(250,50);
    PortChooseBox->move(250,250);
    PortChooseBox->addItem("client");
    PortChooseBox->addItem("service");

    QGroupBox *groupBox1 = new QGroupBox("serial port",this);
    groupBox1->move(50,300);
    groupBox1->resize(450,200);
    QVBoxLayout *vLayout1 = new QVBoxLayout;

    QList<QSerialPortInfo> listinfo = QSerialPortInfo::availablePorts();
    auto info = listinfo.at(0);

    for(int i = 0;i < listinfo.size();i++)
    {
       port_count++;
       info = listinfo.at(i);

       m_serialPortName << info.portName();
    }

    for(int n = 0; n < listinfo.size();n++)
    {
       if(m_serialPortName.at(n) == "ttyS0")
       {
           continue;
       }
       //qDebug() << m_serialPortName.at(n);
       SerialCheckBox[n] = new QCheckBox(m_serialPortName.at(n),this);
       vLayout1->addWidget(SerialCheckBox[n]);
    }

    groupBox1->setLayout(vLayout1);

    SerialMsgText = new QTextEdit(this);
    SerialMsgText->resize(400,400);
    SerialMsgText->move(550,150);

    StartBt = new QPushButton("start", this);
    StartBt->resize(100,50);
    StartBt->move(100,500);

    StopBt = new QPushButton("stop", this);
    StopBt->resize(100,50);
    StopBt->move(300,500);

    connect(retBt,SIGNAL(clicked(bool)),this,SLOT(retBt_clicked()));
    connect(&process,SIGNAL(readyReadStandardOutput()),this,SLOT(readyReadStandardOutput()));
    connect(StartBt,SIGNAL(clicked()),this,SLOT(StartBt_clicked()));
    connect(StopBt,SIGNAL(clicked()),this,SLOT(StopBt_clicked()));

}

serial::~serial()
{
    delete ui;
}

void serial::retBt_clicked()
{
    emit Mysignal();
}

void serial::StartBt_clicked()
{
    port_num_s * port_num_ptr = (port_num_s *)malloc(sizeof(port_num_s));

    qDebug() << "-------StartBt_clicked-------";
    QTextStream cout(stdout);//绑定cout到标准输出

    for(int i = 1;i < port_count; i++)
    {
        if(SerialCheckBox[i]->isChecked())
        {
            QString checktext =  QString("/dev/%1").arg(SerialCheckBox[i]->text()) ;
            //qDebug() << "checktext" << checktext;
            check_flag[i] = 1;
        }
    }

    if(ModeChooseBox->currentText() == "MultiBoard Mode")
    {
        if(PortChooseBox->currentText() == "client")
        {
            port_num_ptr->server_client_flag = 2;
            serial_thread(port_num_ptr);
        }
        else if(PortChooseBox->currentText() == "service")
        {
            port_num_ptr->server_client_flag = 1;
            serial_thread(port_num_ptr);
        }
    }
}


void serial::StopBt_clicked()
{

}


int serial::set_prop(int fd)
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


void *serial::thread_serial_port(void * data)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
    qDebug() << "--LINE--: " << __LINE__<< current_time;

    int		fd;
    int		rc;
    int     com;
    char	buf[10];
    char	check_char;
    char	*port;

    port_num_s * port_num_data = (port_num_s *)data;
    int port_num = port_num_data->port_num;

//    int *port_num_1 = (int *)data;
//    //qDebug() << "porserialt_num_1 = " << *port_num_1;

//    int port_num = *port_num_1;


    //qDebug() << "part_num = "  << port_num;
    QString port_name_1 = QString("/dev/%1").arg(m_serialPortName.at(port_num));
    //qDebug() << port_name_1 <<"  line = " << __LINE__ << endl;
    port = port_name_1.toLatin1().data();
    cout << "port-:" <<  port <<"  line = " << __LINE__ << endl;

    fd = open (port, O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
    if(fd < 0)
    {
        //printf("open serial port (%s) failed\n", port_name_array[port_num]);
        return NULL;
    }

    set_prop (fd);

    usleep(10);
    switch(port_num)
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

    qDebug() << "-------in--------\n";
    while(1)
    {
        if(port_num_data->server_client_flag == 1)
        {
            rc = read(fd, buf, 1);
            //QString ag = buf;
            //qDebug()<<"ag = "<<ag << port_num;

            if(rc>0)
            {
                printf("buf = %s\n",buf);
                if(buf[0] == check_char)
                {
                    //port_check_stat |= (1<<port_num);
                    buf[0] = (port_num-1) + 0x30;
                    write(fd, buf, 1);
                    QString ss = buf;
                    qDebug()<<"ss = "<<ss;

                    //printf("COM=: %d dev=:%s test ok. buf:=%d \n", com, port_name_array[port_num], buf[0]);
                }
            }

        }
        else if(port_num_data->server_client_flag == 2)
        {
            buf[0] = (port_num - 1) + 0x30;
            write(fd, buf, 1);
            QString ag = buf;
            qDebug()<<"ag = "<<ag << port_num;

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

void serial::serial_thread(port_num_s * port_num_ptr)
{
    pthread_t thread_id[SerialNumMax];
    pthread_attr_t  attr;
    int	i;

    //int *port_num_ptr = new int();
//    if(thread_id)
//    {
//        pthread_cancel(thread_id);
//    }

    pthread_attr_init(&attr);
    for(i = 1; i < port_count; i++)
    {
        if(check_flag[i] == 1)
        {
            //printf("%s======%d\n", port_name_array[i], i);
            //*port_num_ptr = i;
            port_num_ptr->port_num = i;
            qDebug() << "pthread : i = " << i;
            if (pthread_create(&thread_id[i], &attr, thread_serial_port,port_num_ptr))
            {
                printf("ERROR: can't create read_thread thread!\n");
            }
            else
            {
                pthread_detach(thread_id[i]);
            }
            QThread::msleep(50);
        }
    }
    sleep(3);

    for(i = 1; i < port_count; i++)
    {
        pthread_cancel(thread_id[i]);
    }
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
    qDebug() << "--LINE--: " << __LINE__<< current_time;

    return;
}

void serial::readyReadStandardOutput()
{

}
