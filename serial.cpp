#include "serial.h"
#include "ui_serial.h"

static int screenFlag;
static int screenWidth;
static int screenHeight;
static int data1;                 // 1: port1  2:port2
static int data2;
static int openFlagPort1;
static int openFlagport2;
static QString port1;
static QString port2;
static QString baud1;
static QString baud2;
static QString stopBit1;
static QString stopBit2;
static QScreen *screen;

serial::serial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serial)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    portNameList = getPortNameList();
    portNameList.removeAll("ttyS0");
    ui->SerialCb_1->addItems(portNameList);
    ui->SerialCb_2->addItems(portNameList);
    if(ui->SerialCb_2->count()>2)
    {
        ui->SerialCb_1->setCurrentIndex(0);
        ui->SerialCb_2->setCurrentIndex(1);
    }

    ui->SendEdit->installEventFilter(this);
    ui->SendEdit_2->installEventFilter(this);

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line_2->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    serialFont();
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
    if(openFlagPort1 == 0)
    {
        QStringList list = databaseWg.tableShow("serial_port");
        if(!list.isEmpty())
        {
            for(int i=0;i<list.size();i++)
            {
                QString str = list.at(i);
                if(str == port1)
                {
                    QMessageBox mesg(QMessageBox::Information,
                                     tr("QMessageBox::information()"),
                                     tr("The serial is occupied!"),
                                     0,this);
                    mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
                    mesg.setFocusPolicy(Qt::NoFocus);
                    mesg.addButton(tr("OK"),QMessageBox::YesRole);
                    if(screenFlag == 1)
                        mesg.move(screenWidth*2/3,screenHeight/3);
                    else
                        mesg.move(screenWidth/3,screenHeight/3);
                    mesg.exec();
                    return;
                }
            }
        }
        PortA = new serial_thread(COM0,port1, baud1.toInt(),data1,stopBit1,&OpenFlag);
        if(OpenFlag == 1)
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("open success!"),
                             0,this);
            mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
            mesg.setFocusPolicy(Qt::NoFocus);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
            ui->btn_open1->setText(tr("close"));
            openFlagPort1 = 1;
            databaseWg.insertTableOne("serial_port",port1);      //The open serial port is saved to the database
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
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
        }

        //Receives a signal to transfer data from a child thread
        connect(PortA,SIGNAL(receive_data_msg(QString)),this,SLOT(show_port1_data(QString)));
        connect(this,SIGNAL(write_port_msg(int,QByteArray)),PortA,SLOT(write_data(int,QByteArray)));
        connect(this,SIGNAL(close_port_msg(int)),PortA,SLOT(close_port(int)));

    }
}

void serial::openSerialPort2()
{
    int OpenFlag = 0;
    if(openFlagport2 == 0)
    {
        QStringList list = databaseWg.tableShow("serial_port");
        if(!list.isEmpty())
        {
            for(int i=0;i<list.size();i++)
            {
                QString str = list.at(i);
                if(str == port2)
                {
                    QMessageBox mesg(QMessageBox::Information,
                                     tr("QMessageBox::information()"),
                                     tr("The serial is occupied!"),
                                     0,this);
                    mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
                    mesg.setFocusPolicy(Qt::NoFocus);
                    mesg.addButton(tr("OK"),QMessageBox::YesRole);
                    if(screenFlag == 1)
                        mesg.move(screenWidth*2/3,screenHeight/3);
                    else
                        mesg.move(screenWidth/3,screenHeight/3);
                    mesg.exec();
                    return;
                }
            }
        }
        PortB = new serial_thread(COM1,port2,baud2.toInt(),data2,stopBit2,&OpenFlag);

        if(OpenFlag == 1)
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("open success!"),
                             0,this);
            mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
            mesg.setFocusPolicy(Qt::NoFocus);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
            ui->btn_open2->setText(tr("close"));
            openFlagport2 = 1;
            databaseWg.insertTableOne("serial_port",port2);
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
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
        }
        //Receives a signal to transfer data from a child thread
        connect(PortB,SIGNAL(receive_data_msg(QString)),this,SLOT(show_port2_data(QString)));
        connect(this,SIGNAL(write_port_msg(int,QByteArray)),PortB,SLOT(write_data(int,QByteArray)));
        connect(this,SIGNAL(close_port_msg(int)),PortB,SLOT(close_port(int)));
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
    if(openFlagPort1 == 1)
    {
        emit write_port_msg(COM0,ui->SendEdit->text().toUtf8());
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
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
    }
}

void serial::on_btn_send2_clicked()
{
    if(openFlagport2 == 1)
    {
        emit write_port_msg(COM1,ui->SendEdit_2->text().toUtf8());
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
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
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

void serial::serialFont()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = screenWidth / realX * 2.54;
    qreal realHeight = screenHeight / realY *2.54;
    QFont font;
    if(screenFlag)
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
    port2 = ui->SerialCb_2->currentText();
    baud2 = ui->BaudrateCb_2->currentText();
    data2 = ui->DatabitsCb_2->currentText().toInt();
    stopBit2 = ui->StopbitCb_2->currentText();
    openSerialPort2();
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
    stopBit1 = ui->StopbitCb_1->currentText();
    openSerialPort1();
    ui->stackedWidget_2->setCurrentIndex(0);
}

void serial::on_btn_cancel1_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void serial::on_btn_open1_clicked()
{
    if(openFlagPort1 == 0)
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
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            mesg.exec();
            return;
        }
        openSerialPort1();
    }
    else
    {
        //Close the thread
        if(PortA!=nullptr)
        {
            emit close_port_msg(COM0);
        }

        ui->btn_open1->setText(tr("open"));
        openFlagPort1 = 0;
        databaseWg.deleteTableName("serial_port",port1);
    }
}

void serial::on_btn_open2_clicked()
{
    if(openFlagport2 == 0)
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
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            mesg.exec();
            return;
        }
        openSerialPort2();
    }
    else
    {
        //Close the thread
        if(PortB!=nullptr)
        {
            emit close_port_msg(COM1);
        }
        ui->btn_open2->setText(tr("open"));
        openFlagport2 = 0;
        databaseWg.deleteTableName("serial_port",port2);
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
