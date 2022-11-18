#include "serial_thread.h"

serial_thread::serial_thread(int portId,QString port,long baud,int Databit,QString Stopbit,int *OpenFlag,QObject *parent) : QObject(parent)
{
    g_myThread = new QThread();
    g_myPort = new QSerialPort();
    g_myPortId = portId;

    *OpenFlag = 0;
    initPort(port,baud,Databit,Stopbit,OpenFlag);

    this->moveToThread(g_myThread);
    g_myPort->moveToThread(g_myThread);
    g_myThread->start();
}

void serial_thread::initPort(QString port, long baud, int dataBit, QString stopBit, int *openFlag)
{
    g_myPort->setPortName(port);
    g_myPort->setBaudRate(baud);
    initDataBit(dataBit);
    g_myPort->setParity(QSerialPort::NoParity);
    initStopBit(stopBit);
    g_myPort->setFlowControl(QSerialPort::NoFlowControl);


    if(g_myPort->open(QIODevice::ReadWrite) == false)         // Open read and write mode
    {
        *openFlag = 0;
    }
    else
    {
        *openFlag = 1;
    }

    connect(g_myPort,SIGNAL(readyRead()),this,SLOT(read_data()));//If data is present, it is read
}

void serial_thread::close_port(int portId)
{
    if(portId==g_myPortId)
    {
        g_myPort->close();
        g_myPort->deleteLater();

        g_myThread->quit();
        g_myThread->wait();
        g_myThread->deleteLater();
    }
}

void serial_thread::read_data()
{
    QString buf;
    buf = QString(g_myPort->readAll());
    emit receive_data_msg(buf);
}

void serial_thread::write_data(int portId,QByteArray buff)
{
    if(portId==g_myPortId)
    {
        g_myPort->write(buff);
    }
}

void serial_thread::initDataBit(int dataBit)
{
    switch (dataBit) {
    case 5:g_myPort->setDataBits(QSerialPort::Data5);break;
    case 6:g_myPort->setDataBits(QSerialPort::Data6);break;
    case 7:g_myPort->setDataBits(QSerialPort::Data7);break;
    case 8:g_myPort->setDataBits(QSerialPort::Data8);break;
    default:break;
    }
}

void serial_thread::initStopBit(QString stopBit)
{
    if(!QString::compare(stopBit,QString("1"),Qt::CaseSensitive)){
        g_myPort->setStopBits(QSerialPort::OneStop);
    }
    if(!QString::compare(stopBit,QString("2"),Qt::CaseSensitive)){
        g_myPort->setStopBits(QSerialPort::TwoStop);
    }
}
