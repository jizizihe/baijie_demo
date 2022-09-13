#include "serial.h"
#include "ui_serial.h"

static int screen_flag;
static int Width;  //屏幕宽
static int Height;
//static int serialp1_flag;
//static int serialp2_flag;

static QString port1;
static QString port2;
static QString baud1;
static QString baud2;
static int data1;
static int data2;
static QString stopbit1;
static QString stopbit2;
static int flag_port1;
static int flag_port2;
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
    ui->SerialCb_1->addItems(m_portNameList);
    ui->SerialCb_2->addItems(m_portNameList);
    if(ui->SerialCb_2->count()>2)
    {
        ui->SerialCb_1->setCurrentIndex(0);
        ui->SerialCb_2->setCurrentIndex(1);
    }

   ui->SendEdit->installEventFilter(this);
   ui->SendEdit_2->installEventFilter(this);

   if(Width < Height)
   {
     screen_flag = 1;
   }
   serial_font();  
   ui->stackedWidget->setCurrentIndex(0);
   ui->stackedWidget_2->setCurrentIndex(0);
  // connect(&serialset_port1,SIGNAL(port1_re()),this,SLOT(port1_return()));
  // connect(&serialset_port2,SIGNAL(port2_re()),this,SLOT(port2_return()));
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
             QMessageBox mesg(QMessageBox::Information,
                              tr("QMessageBox::information()"),
                              tr("open success!"),
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
        ui->btn_open1->setText(tr("close"));
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
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("open success!"),
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
        connect(PortB,SIGNAL(receive_data(QString)),this,SLOT(on_showData1(QString)));//,Qt::QueuedConnection
        connect(this,SIGNAL(writePort_sig(int,QByteArray)),PortB,SLOT(write_data(int,QByteArray)));
        connect(this,SIGNAL(closePort_sig(int)),PortB,SLOT(closePort(int)));

       //关闭设置菜单使能
       ui->btn_open2->setText(tr("close"));
       flag_port2 = 1;
    }

}

void serial::on_showData(QString buff)//用于显示数据
{
    qDebug() << "LINE:" << __LINE__ << buff << endl;
    ui->recvEdit->appendPlainText(buff);
    //ui->textEdit_1->append(buff.toUtf8());
}
void serial::on_showData1(QString buff)//用于显示数据
{
    //qDebug() << "LINE:" << __LINE__ << buff << endl;
    ui->recvEdit_2->appendPlainText(buff);
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
     emit writePort_sig(COM1,ui->SendEdit_2->text().toUtf8());
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

void serial::on_retBtn_clicked()
{
    emit Mysignal();
}

void serial::language_reload()
{
    ui->retranslateUi(this);
    serialset_port1.language_reload();
    serialset_port2.language_reload();
}

void serial::on_part1_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
//    if(flag_port1 == 0)
//    {
//        serialp1_show();
//    }
//    else
//    {
//        //关闭线程
//        if(PortA!=nullptr)
//        {
//            emit closePort_sig(COM0);
//        }
//        //恢复设置菜单使能
//        //        ui->SerialCb->setEnabled(true);
//        //        ui->BaudrateCb->setEnabled(true);
//        ui->part1_btn->setText(tr("open port1"));
//        flag_port1 = 0;
//    }

}

void serial::on_part2_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_3->setCurrentIndex(0);
//    if(flag_port2 == 0)
//    {
//        serialp2_show();
//    }
//    else
//    {
//        //关闭线程
//        if(PortB!=nullptr)
//        {
//           emit closePort_sig(COM1);
//        }
//        //恢复设置菜单使能
//        ui->part2_btn->setEnabled(true);
//        ui->part2_btn->setText(tr("open port2"));
//        flag_port2 = 0;
//    }
}

void serial::port1_return()
{
//    if(screen_flag == 1)
//    {
//      serialset_port1.hide();
//      serialp1_view->hide();
//      this->hide();
//      this->show();
//      this->activateWindow();this->setFocus();port1_show=0;
//    }
//    else
//      serialset_port1.hide();
//    if(serialset_port1.hide_flag == 1)
//    {
//       serialport1_open();
//     }
}

void serial::port2_return()
{
//    if(screen_flag == 1)
//    {
//      serialset_port2.hide();
//      serialp2_view->hide();
//      this->hide();
//      this->show();
//      this->activateWindow();this->setFocus();port2_show=0;
//    }
//    else
//        serialset_port2.hide();
//    if(serialset_port2.hide_flag == 1)
//    {
//       serialport2_open();
//    }
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
    ui->recvEdit->setFont(font);
    ui->recvEdit_2->setFont(font);
    ui->label->setFont(font);
    ui->SendEdit->setFont(font);
    ui->SendEdit_2->setFont(font);
    ui->SendBtn->setFont(font);
    ui->SendBtn_2->setFont(font);
    ui->btn_set1->setFont(font);
    ui->btn_clear1->setFont(font);
    ui->btn_clear1_2->setFont(font);
    ui->btn_open1->setFont(font);
    ui->btn_open2->setFont(font);
    ui->btn_set1_2->setFont(font);
    ui->label_3->setFont(font);
    ui->label_4->setFont(font);
}

void serial::on_btn_set1_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}

void serial::on_btn_set1_2_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
}

void serial::on_btn_ok2_clicked()
{
    port2 = ui->SerialCb_2->currentText();
    baud2 = ui->BaudrateCb_2->currentText();
    data2 = ui->DatabitsCb_2->currentText().toInt();
    stopbit2 = ui->StopbitCb_2->currentText();

    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("set successfully!"),
                     0,this);
     mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
     mesg.setFocusPolicy(Qt::NoFocus);
     mesg.addButton(tr("OK"),QMessageBox::YesRole);
     if(screen_flag == 1)
     mesg.move(Width*2/3,Height/3);
     mesg.exec();
    ui->stackedWidget_3->setCurrentIndex(0);
}

void serial::on_btn_cancel2_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
}

void serial::on_btn_ok1_clicked()
{
    port1 = ui->SerialCb_1->currentText();
    baud1 = ui->BaudrateCb_1->currentText();
    data1 = ui->DatabitsCb_1->currentText().toInt();
    stopbit1 = ui->StopbitCb_1->currentText();
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("set successfully!"),
                     0,this);
     mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
     mesg.setFocusPolicy(Qt::NoFocus);
     mesg.addButton(tr("OK"),QMessageBox::YesRole);
     if(screen_flag == 1)
     mesg.move(Width*2/3,Height/3);
     mesg.exec();
   ui->stackedWidget_2->setCurrentIndex(0);
}

void serial::on_btn_cancel1_clicked()
{
   ui->stackedWidget_2->setCurrentIndex(0);
}

void serial::on_btn_open1_clicked()
{
    if(flag_port1 == 0)
    {
        if(port1.isEmpty())
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("please set the port!"),
                             0,this);
             mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
             mesg.setFocusPolicy(Qt::NoFocus);
             mesg.addButton(tr("OK"),QMessageBox::YesRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             mesg.exec();
             return;
        }
        serialport1_open();
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
        ui->btn_open1->setText(tr("open"));
        flag_port1 = 0;
    }
}

void serial::on_btn_open2_clicked()
{
    if(flag_port2 == 0)
    {
        if(port2.isEmpty())
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("please set the port!"),
                             0,this);
             mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
             mesg.setFocusPolicy(Qt::NoFocus);
             mesg.addButton(tr("OK"),QMessageBox::YesRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             mesg.exec();
             return;
        }
        serialport2_open();
    }
    else
    {
        //关闭线程
        if(PortB!=nullptr)
        {
           emit closePort_sig(COM1);
        }
        //恢复设置菜单使能
        //ui->part2_btn->setEnabled(true);
        ui->btn_open2->setText(tr("open"));
        flag_port2 = 0;
    }
}

void serial::on_btn_clear1_clicked()
{
    ui->recvEdit->clear();
    ui->SendEdit->clear();
}

void serial::on_btn_clear1_2_clicked()
{
    ui->recvEdit_2->clear();
    ui->SendEdit_2->clear();
}
