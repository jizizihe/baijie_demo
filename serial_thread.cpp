#include "serial_thread.h"

serial_thread::serial_thread(int portId,QString port,long baud,int Databit,QString Stopbit,int *OpenFlag,QObject *parent) : QObject(parent)
{
    myThread = new QThread();
    myPort = new QSerialPort();
    myPortId = portId;

    *OpenFlag = 0;
    initPort(port,baud,Databit,Stopbit,OpenFlag);

    this->moveToThread(myThread);
    myPort->moveToThread(myThread);
    myThread->start();
}

void serial_thread::initPort(QString port, long baud, int dataBit, QString stopBit, int *openFlag)
{
    myPort->setPortName(port);
    myPort->setBaudRate(baud);
    initDataBit(dataBit);
    myPort->setParity(QSerialPort::NoParity);
    initStopBit(stopBit);
    myPort->setFlowControl(QSerialPort::NoFlowControl);


    if(myPort->open(QIODevice::ReadWrite) == false)         // Open read and write mode
    {
        *openFlag = 0;
    }
    else
    {
        *openFlag = 1;
    }

    connect(myPort,SIGNAL(readyRead()),this,SLOT(read_data()));//If data is present, it is read
}

void serial_thread::close_port(int portId)
{
    if(portId==myPortId)
    {
        myPort->close();
        myPort->deleteLater();

        myThread->quit();
        myThread->wait();
        myThread->deleteLater();
    }
}

void serial_thread::read_data()
{
    QString buf;
    buf = QString(myPort->readAll());
    emit receive_data_msg(buf);
}

void serial_thread::write_data(int portId,QByteArray buff)
{
    if(portId==myPortId)
    {
        myPort->write(buff);
    }
}

void serial_thread::initDataBit(int dataBit)
{
    switch (dataBit) {
    case 5:myPort->setDataBits(QSerialPort::Data5);break;
    case 6:myPort->setDataBits(QSerialPort::Data6);break;
    case 7:myPort->setDataBits(QSerialPort::Data7);break;
    case 8:myPort->setDataBits(QSerialPort::Data8);break;
    default:break;
    }
}

void serial_thread::initStopBit(QString stopBit)
{
    if(!QString::compare(stopBit,QString("1"),Qt::CaseSensitive)){
        myPort->setStopBits(QSerialPort::OneStop);
    }
    if(!QString::compare(stopBit,QString("2"),Qt::CaseSensitive)){
        myPort->setStopBits(QSerialPort::TwoStop);
    }
}
