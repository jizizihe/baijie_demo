#include "serial.h"
#include "ui_serial.h"

static int g_screenFlag;
static int g_screenWidth;
static int g_screenHeight;
static int g_data1;                 // 1: port1  2:port2
static int g_data2;
static int g_openFlagPort1;
static int g_openFlagport2;
static QString g_port1;
static QString g_port2;
static QString g_baud1;
static QString g_baud2;
static QString g_stopBit1;
static QString g_stopBit2;
static QScreen *g_screen;

serial::serial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serial)
{
    ui->setupUi(this);
    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();
    g_portNameList = getPortNameList();
    g_portNameList.removeAll("ttyS0");
    ui->SerialCb_1->addItems(g_portNameList);
    ui->SerialCb_2->addItems(g_portNameList);
    if(ui->SerialCb_2->count()>2)
    {
        ui->SerialCb_1->setCurrentIndex(0);
        ui->SerialCb_2->setCurrentIndex(1);
    }

    ui->SendEdit->installEventFilter(this);
    ui->SendEdit_2->installEventFilter(this);

    if(g_screenWidth < g_screenHeight)
    {
        g_screenFlag = 1;ui->line_2->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    setSerialFont();
}

serial::~serial()
{
    delete ui;
}

QStringList serial::getPortNameList()
{
    QStringList serialPortName;
    //Find available serial ports
    foreach(const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())//Read serial port information
    {
        serialPortName << Info.portName();
    }
    return serialPortName;
}

void serial::openSerialPort1()
{
    int OpenFlag = 0;
    if(g_openFlagPort1 == 0)
    {
        QStringList list = g_database.tableShow("serial_port");
        if(!list.isEmpty())
        {
            for(int i=0;i<list.size();i++)
            {
                QString str = list.at(i);
                if(str == g_port1)
                {
                    QMessageBox mesg(QMessageBox::Information,
                                     tr("QMessageBox::information()"),
                                     tr("The serial is occupied!"),
                                     0,this);
                    mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
                    mesg.setFocusPolicy(Qt::NoFocus);
                    mesg.addButton(tr("OK"),QMessageBox::YesRole);
                    if(g_screenFlag == 1)
                        mesg.move(g_screenWidth*2/3,g_screenHeight/3);
                    else
                        mesg.move(g_screenWidth/3,g_screenHeight/3);
                    mesg.exec();
                    return;
                }
            }
        }
        g_PortA = new serial_thread(EnumCom0,g_port1, g_baud1.toInt(),g_data1,g_stopBit1,&OpenFlag);
        if(OpenFlag == 1)
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("open success!"),
                             0,this);
            mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
            mesg.setFocusPolicy(Qt::NoFocus);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(g_screenFlag == 1)
                mesg.move(g_screenWidth*2/3,g_screenHeight/3);
            else
                mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
            ui->btn_open1->setText(tr("close"));
            g_openFlagPort1 = 1;
            g_database.insertTableOne("serial_port",g_port1);      //The open serial port is saved to the database
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
            if(g_screenFlag == 1)
                mesg.move(g_screenWidth*2/3,g_screenHeight/3);
            else
                mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
        }

        //Receives a signal to transfer data from a child thread
        connect(g_PortA,SIGNAL(receive_data_msg(QString)),this,SLOT(show_port1_data(QString)));
        connect(this,SIGNAL(write_port_msg(int,QByteArray)),g_PortA,SLOT(write_data(int,QByteArray)));
        connect(this,SIGNAL(close_port_msg(int)),g_PortA,SLOT(close_port(int)));

    }
}

void serial::openSerialPort2()
{
    int OpenFlag = 0;
    if(g_openFlagport2 == 0)
    {
        QStringList list = g_database.tableShow("serial_port");
        if(!list.isEmpty())
        {
            for(int i=0;i<list.size();i++)
            {
                QString str = list.at(i);
                if(str == g_port2)
                {
                    QMessageBox mesg(QMessageBox::Information,
                                     tr("QMessageBox::information()"),
                                     tr("The serial is occupied!"),
                                     0,this);
                    mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
                    mesg.setFocusPolicy(Qt::NoFocus);
                    mesg.addButton(tr("OK"),QMessageBox::YesRole);
                    if(g_screenFlag == 1)
                        mesg.move(g_screenWidth*2/3,g_screenHeight/3);
                    else
                        mesg.move(g_screenWidth/3,g_screenHeight/3);
                    mesg.exec();
                    return;
                }
            }
        }
        g_PortB = new serial_thread(EnumCom1,g_port2,g_baud2.toInt(),g_data2,g_stopBit2,&OpenFlag);

        if(OpenFlag == 1)
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("open success!"),
                             0,this);
            mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
            mesg.setFocusPolicy(Qt::NoFocus);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(g_screenFlag == 1)
                mesg.move(g_screenWidth*2/3,g_screenHeight/3);
            else
                mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
            ui->btn_open2->setText(tr("close"));
            g_openFlagport2 = 1;
            g_database.insertTableOne("serial_port",g_port2);
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
            if(g_screenFlag == 1)
                mesg.move(g_screenWidth*2/3,g_screenHeight/3);
            else
                mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
        }
        //Receives a signal to transfer data from a child thread
        connect(g_PortB,SIGNAL(receive_data_msg(QString)),this,SLOT(show_port2_data(QString)));
        connect(this,SIGNAL(write_port_msg(int,QByteArray)),g_PortB,SLOT(write_data(int,QByteArray)));
        connect(this,SIGNAL(close_port_msg(int)),g_PortB,SLOT(close_port(int)));
    }
}

void serial::show_port1_data(QString buff)  //Display serial port1 data
{
    ui->recvEdit->appendPlainText(buff);
}
void serial::show_port2_data(QString buff)  //Display serial port2 data
{
    ui->recvEdit_2->appendPlainText(buff);
}

void serial::on_btn_send1_clicked()
{
    if(g_openFlagPort1 == 1)
    {
        emit write_port_msg(EnumCom0,ui->SendEdit->text().toUtf8());
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
        if(g_screenFlag == 1)
            mesg.move(g_screenWidth*2/3,g_screenHeight/3);
        else
            mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
    }
}

void serial::on_btn_send2_clicked()
{
    if(g_openFlagport2 == 1)
    {
        emit write_port_msg(EnumCom1,ui->SendEdit_2->text().toUtf8());
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
        if(g_screenFlag == 1)
            mesg.move(g_screenWidth*2/3,g_screenHeight/3);
        else
            mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
    }
}

void serial::on_btn_ret_clicked()
{
    emit serial_back_msg();
}

void serial::languageReload()
{
    ui->retranslateUi(this);
}

void serial::on_btn_port1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
}

void serial::on_btn_port2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_3->setCurrentIndex(0);
}

void serial::setSerialFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realY = g_screen->physicalDotsPerInchY();
    qreal realWidth = g_screenWidth / realX * 2.54;
    qreal realHeight = g_screenHeight / realY *2.54;
    QFont font;
    if(g_screenFlag)
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
    ui->btn_port1->setFont(font);
    ui->btn_port2->setFont(font);
    ui->lbl_port1->setFont(font);
    ui->recvEdit->setFont(font);
    ui->recvEdit_2->setFont(font);
    ui->lbl_port2->setFont(font);
    ui->SendEdit->setFont(font);
    ui->SendEdit_2->setFont(font);
    ui->btn_send1->setFont(font);
    ui->btn_send2->setFont(font);
    ui->btn_set1->setFont(font);
    ui->btn_clear1->setFont(font);
    ui->btn_clear1_2->setFont(font);
    ui->btn_open1->setFont(font);
    ui->btn_open2->setFont(font);
    ui->btn_set1_2->setFont(font);
    ui->lbl_send1->setFont(font);
    ui->lbl_send2->setFont(font);
    ui->lbl_serialPort->setFont(font);
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
    g_port2 = ui->SerialCb_2->currentText();
    g_baud2 = ui->BaudrateCb_2->currentText();
    g_data2 = ui->DatabitsCb_2->currentText().toInt();
    g_stopBit2 = ui->StopbitCb_2->currentText();
    openSerialPort2();
    ui->stackedWidget_3->setCurrentIndex(0);
}

void serial::on_btn_cancel2_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
}

void serial::on_btn_ok1_clicked()
{
    g_port1 = ui->SerialCb_1->currentText();
    g_baud1 = ui->BaudrateCb_1->currentText();
    g_data1 = ui->DatabitsCb_1->currentText().toInt();
    g_stopBit1 = ui->StopbitCb_1->currentText();
    openSerialPort1();
    ui->stackedWidget_2->setCurrentIndex(0);
}

void serial::on_btn_cancel1_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void serial::on_btn_open1_clicked()
{
    if(g_openFlagPort1 == 0)
    {
        if(g_port1.isEmpty())
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("please set the port!"),
                             0,this);
            mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
            mesg.setFocusPolicy(Qt::NoFocus);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(g_screenFlag == 1)
                mesg.move(g_screenWidth*2/3,g_screenHeight/3);
            mesg.exec();
            return;
        }
        openSerialPort1();
    }
    else
    {
        //Close the thread
        if(g_PortA!=nullptr)
        {
            emit close_port_msg(EnumCom0);
        }

        ui->btn_open1->setText(tr("open"));
        g_openFlagPort1 = 0;
        g_database.deleteTableName("serial_port",g_port1);
    }
}

void serial::on_btn_open2_clicked()
{
    if(g_openFlagport2 == 0)
    {
        if(g_port2.isEmpty())
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("please set the port!"),
                             0,this);
            mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
            mesg.setFocusPolicy(Qt::NoFocus);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(g_screenFlag == 1)
                mesg.move(g_screenWidth*2/3,g_screenHeight/3);
            mesg.exec();
            return;
        }
        openSerialPort2();
    }
    else
    {
        //Close the thread
        if(g_PortB!=nullptr)
        {
            emit close_port_msg(EnumCom1);
        }
        ui->btn_open2->setText(tr("open"));
        g_openFlagport2 = 0;
        g_database.deleteTableName("serial_port",g_port2);
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
