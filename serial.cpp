#include "serial.h"
#include "ui_serial.h"

static int screen_flag;
static int Width;  //屏幕宽
static int Height;

static QGraphicsView *serialp1_view;
static QGraphicsView *serialp2_view;

static int serialp1_flag;
static int serialp2_flag;

static QString port1;
static QString baud1;
static int data1;
static QString stopbit1;
static int data2;
static QString stopbit2;
static QString port2;
static QString baud2;

static int flag_port1;
static int flag_port2;
static int port1_show;
static int port2_show;
static QScreen *screen;

serial::serial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serial)
{
    ui->setupUi(this);

    screen = qApp->primaryScreen();
    Width = screen->size().width();
    Height = screen->size().height();

    m_portNameList = getPortNameList();
//    if(ui->SerialCb_2->count()>2)
//    {
//        ui->SerialCb->setCurrentIndex(0);
//        ui->SerialCb_2->setCurrentIndex(1);
//    }
 ui->SendEdit->installEventFilter(this);

    if(Width < Height)
    {
        screen_flag = 1;
    }
   serial_font();
   connect(&serialset_port1,SIGNAL(port1_set(QString,QString,int,QString)),this,SLOT(rec_port1set(QString,QString,int,QString)));
   connect(&serialset_port2,SIGNAL(port2_set(QString,QString,int,QString)),this,SLOT(rec_port2set(QString,QString,int,QString)));
   connect(&serialset_port1,SIGNAL(port1_re()),this,SLOT(port1_return()));
   connect(&serialset_port2,SIGNAL(port2_re()),this,SLOT(port2_return()));
}

serial::~serial()
{
    delete ui;
}

QStringList serial::getPortNameList()
{
    QStringList serialPortName;
    //查找可用的串口
    foreach(const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())//读取串口信息
    {
        serialPortName << Info.portName();
        //qDebug()<<"portname: "<<Info.portName();//调试时可以看的串口信息
    }
    return serialPortName;
}

void serial::serialport1_open()
{
    int OpenFlag = 0;
    //对串口进行一些初始化
   if(flag_port1 == 0)
    {
         PortA = new serial_thread(COM0,port1, baud1.toInt(),data1,stopbit1,&OpenFlag);
         if(OpenFlag == 1)
         {
//             QMessageBox mesg(QMessageBox::Information,
//                              tr("QMessageBox::information()"),
//                              tr("open success!"),
//                              0,this);
//              mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
//              mesg.setFocusPolicy(Qt::NoFocus);
//              mesg.addButton(tr("OK"),QMessageBox::YesRole);
//              if(screen_flag == 1)
//              mesg.move(Width*2/3,Height/3);
//              else
//              mesg.move(Width/3,Height/3);
//              mesg.exec();
         }
         else
         {
             QMessageBox mesg(QMessageBox::Information,
                              tr("QMessageBox::information()"),
                              tr("open failed!"),
                              0,this);
              mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
              mesg.setFocusPolicy(Qt::NoFocus);
              mesg.addButton(tr("OK"),QMessageBox::YesRole);
              if(screen_flag == 1)
              mesg.move(Width*2/3,Height/3);
              else
              mesg.move(Width/3,Height/3);
              mesg.exec();
         }

        //接收从子线程传输数据的信号
        connect(PortA,SIGNAL(receive_data(QString)),this,SLOT(on_showData(QString)));//,Qt::QueuedConnection
        connect(this,SIGNAL(writePort_sig(int,QByteArray)),PortA,SLOT(write_data(int,QByteArray)));
        connect(this,SIGNAL(closePort_sig(int)),PortA,SLOT(closePort(int)));
        //关闭设置菜单使能
//        ui->SerialCb->setEnabled(false);
//        ui->BaudrateCb->setEnabled(false);
        ui->part1_btn->setText(tr("close port1"));
        flag_port1 = 1;
        qDebug() << "open";
    }

}

void serial::serialport2_open()
{
    int OpenFlag = 0;
    //对串口进行一些初始化
    if(flag_port2 == 0)
    {
        PortB = new serial_thread(COM1,port2,baud2.toInt(),data2,stopbit2,&OpenFlag);

        if(OpenFlag == 1)
        {
//            QMessageBox mesg(QMessageBox::Information,
//                             tr("QMessageBox::information()"),
//                             tr("open success!"),
//                             0,this);
//             mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
//             mesg.setFocusPolicy(Qt::NoFocus);
//             mesg.addButton(tr("OK"),QMessageBox::YesRole);
//             if(screen_flag == 1)
//             mesg.move(Width*2/3,Height/3);
//             else
//             mesg.move(Width/3,Height/3);
//             mesg.exec();
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("open failed!"),
                             0,this);
             mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
             mesg.setFocusPolicy(Qt::NoFocus);
             mesg.addButton(tr("OK"),QMessageBox::YesRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             else
             mesg.move(Width/3,Height/3);
             mesg.exec();
        }
       //接收从子线程传输数据的信号
        connect(PortB,SIGNAL(receive_data(QString)),this,SLOT(on_showData(QString)));//,Qt::QueuedConnection
        connect(this,SIGNAL(writePort_sig(int,QByteArray)),PortB,SLOT(write_data(int,QByteArray)));
        connect(this,SIGNAL(closePort_sig(int)),PortB,SLOT(closePort(int)));

       //关闭设置菜单使能
       ui->part2_btn->setText(tr("close port2"));
       flag_port2 = 1;
    }

}

void serial::on_showData(QString buff)//用于显示数据
{
    //qDebug() << "LINE:" << __LINE__ << buff << endl;
    ui->recvEdit->appendPlainText(buff);
}

void serial::on_SendBtn_clicked()
{
    qDebug() << "LINE:" << __LINE__ << "on_sendBtn_clicked" << endl;
    if(flag_port1 == 1)
    {
        emit writePort_sig(COM0,ui->SendEdit->text().toUtf8());
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open port1!"),
                         0,this);
         mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
         mesg.setFocusPolicy(Qt::NoFocus);
         mesg.addButton(tr("OK"),QMessageBox::YesRole);
         if(screen_flag == 1)
         mesg.move(Width*2/3,Height/3);
         else
         mesg.move(Width/3,Height/3);
         mesg.exec();
    }
}

void serial::on_SendBtn_2_clicked()
{
    qDebug() << "LINE:" << __LINE__ << "on_sendBtn_2_clicked" << endl;
    if(flag_port2 == 1)
    {
     emit writePort_sig(COM1,ui->SendEdit->text().toUtf8());
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open port2!"),
                         0,this);
         mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
         mesg.setFocusPolicy(Qt::NoFocus);
         mesg.addButton(tr("OK"),QMessageBox::YesRole);
         if(screen_flag == 1)
         mesg.move(Width*2/3,Height/3);
         else
         mesg.move(Width/3,Height/3);
         mesg.exec();
    }
}

void serial::on_CleanBtn_clicked()
{
    ui->SendEdit->clear();
}

void serial::on_CleanBtn_2_clicked()
{
    ui->recvEdit->clear();
}

void serial::on_retBtn_clicked()
{
    emit Mysignal();
    if(screen_flag == 1)
    {
        if(port1_show == 1)
        {
            serialset_port1.hide();
            serialp1_view->hide();
        }
        if(port2_show == 1)
        {
            serialset_port2.hide();
            serialp2_view->hide();
        }
    }
    else
    {
        serialset_port1.hide();
        serialset_port2.hide();
    }
}

void serial::language_reload()
{
    ui->retranslateUi(this);
    serialset_port1.language_reload();
    serialset_port2.language_reload();
}

void serial::on_part1_btn_clicked()
{
    if(flag_port1 == 0)
    {
        serialp1_show();
    }
    else
    {
        //关闭线程
        if(PortA!=nullptr)
        {
            emit closePort_sig(COM0);
        }
        //恢复设置菜单使能
        //        ui->SerialCb->setEnabled(true);
        //        ui->BaudrateCb->setEnabled(true);
        ui->part1_btn->setText(tr("open port1"));
        flag_port1 = 0;
        qDebug() << "CLOSE 1";
    }

}

void serial::rec_port1set(QString port,QString baud,int Databit,QString Stopbit)
{
    port1 = port;
    baud1 = baud;
    data1 = Databit;
    stopbit1 = Stopbit;
}

void serial::rec_port2set(QString port,QString baud,int Databit,QString Stopbit)
{
    port2 = port;
    baud2 = baud;
    data2 = Databit;
    stopbit2 = Stopbit;
}

void serial::on_part2_btn_clicked()
{
    if(flag_port2 == 0)
    {
        serialp2_show();
    }
    else
    {
        qDebug() << "pot2 close";
        //关闭线程
        if(PortB!=nullptr)
        {
           emit closePort_sig(COM1);
        }
        //恢复设置菜单使能
        ui->part2_btn->setEnabled(true);
        ui->part2_btn->setText(tr("open port2"));
        flag_port2 = 0;
    }
}

void serial::serialp1_show()
{
    if(screen_flag == 0)
    {
        serialset_port1.move(this->size().width()/5,this->size().height()/7);
        serialset_port1.resize(width()*2/5,height()*2/3);
        serialset_port1.show();
    }
    else
    {
        if(serialp1_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&serialset_port1);
            w->setRotation(90);

            serialp1_view = new QGraphicsView(scene);

            serialp1_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            serialp1_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            serialp1_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            serialp1_view->setAttribute(Qt::WA_ShowWithoutActivating,true);
            serialp1_view->setFocusPolicy(Qt::NoFocus);

            serialset_port1.resize(Height/2,Width*2/3);
            serialp1_view->resize(Width*2/3,Height/2);
            serialset_port1.show();
            serialp1_view->show();
            serialp1_view->move(Width/4,Height/4);
            serialp1_flag++;
        }
        else
        {
            serialset_port1.show();
            serialp1_view->show();
            serialp1_view->move(Width/4,Height/4);
        }
        port1_show++;
    }
}

void serial::serialp2_show()
{
    if(screen_flag == 0)
    {
        serialset_port2.move(this->size().width()/5,this->size().height()/7);
        serialset_port2.resize(Width*2/5,Height*2/3);
        serialset_port2.show();
    }
    else
    {
        if(serialp2_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&serialset_port2);
            w->setRotation(90);

            serialp2_view = new QGraphicsView(scene);
            serialp2_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            serialp2_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            serialp2_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            serialp2_view->setAttribute(Qt::WA_ShowWithoutActivating,true);
            serialp2_view->setFocusPolicy(Qt::NoFocus);

            serialset_port2.resize(Height/2,Width*2/3);
            serialp2_view->resize(Width*2/3,Height/2);
            serialset_port2.show();
            serialp2_view->show();
            serialp2_view->move(Width/4,Height/4);
            serialp2_flag++;
        }
        else
        {
            serialset_port2.show();
            serialp2_view->show();
            serialp2_view->move(Width/4,Height/4);
        }
        port2_show++;
    }
}

void serial::port1_return()
{
    if(screen_flag == 1)
    {
      serialset_port1.hide();
      serialp1_view->hide();
      this->hide();
      this->show();
      this->activateWindow();this->setFocus();port1_show=0;
    }
    else
      serialset_port1.hide();
    if(serialset_port1.hide_flag == 1)
    {
       serialport1_open();qDebug() << "ll";
     }
}

void serial::port2_return()
{
    if(screen_flag == 1)
    {
      serialset_port2.hide();
      serialp2_view->hide();
      this->hide();
      this->show();
      this->activateWindow();this->setFocus();port2_show=0;
    }
    else
        serialset_port2.hide();
    if(serialset_port2.hide_flag == 1)
    {
       serialport2_open();
    }
}


void serial::serial_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = Width / realX * 2.54;
    qreal realHeight = Height / realY *2.54;
    QFont font;
    if(screen_flag)
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
    ui->part1_btn->setFont(font);
    ui->part2_btn->setFont(font);
    ui->label_2->setFont(font);
    ui->CleanBtn_2->setFont(font);
    ui->recvEdit->setFont(font);
    ui->label->setFont(font);
    ui->CleanBtn->setFont(font);
    ui->SendEdit->setFont(font);
    ui->SendBtn->setFont(font);
    ui->SendBtn_2->setFont(font);
}

