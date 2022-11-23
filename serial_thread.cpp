#include "serial_thread.h"

serial_thread::serial_thread(int portId, QString port, long baud, int dataBit, QString stopBit, int *openFlag, QObject *parent) : QObject(parent)
{
    g_thread = new QThread();
    g_port = new QSerialPort();
    g_portId = portId;

    *openFlag = 0;
    initPort(port,baud,dataBit,stopBit,openFlag);

    this->moveToThread(g_thread);
    g_port->moveToThread(g_thread);
    g_thread->start();
}

void serial_thread::initPort(QString port, long baud, int dataBit, QString stopBit, int *openFlag)
{
    g_port->setPortName(port);
    g_port->setBaudRate(baud);
    initDataBit(dataBit);
    g_port->setParity(QSerialPort::NoParity);
    initStopBit(stopBit);
    g_port->setFlowControl(QSerialPort::NoFlowControl);


    if(g_port->open(QIODevice::ReadWrite) == false)         // Open read and write mode
    {
        *openFlag = 0;
    }
    else
    {
        *openFlag = 1;
    }

    connect(g_port,SIGNAL(readyRead()),this,SLOT(read_data()));  // If data is present, it is read
}

void serial_thread::close_port(int portId)
{
    if(portId == g_portId)
    {
        g_port->close();
        g_port->deleteLater();

        g_thread->quit();
        g_thread->wait();
        g_thread->deleteLater();
    }
}

void serial_thread::read_data()
{
    QString buf;
    buf = QString(g_port->readAll());
    emit receive_data_msg(buf);
}

void serial_thread::write_data(int portId,QByteArray buff)
{
    if(portId == g_portId)
    {
        g_port->write(buff);
    }
}

void serial_thread::initDataBit(int dataBit)
{
    switch (dataBit) {
    case 5:g_port->setDataBits(QSerialPort::Data5);break;
    case 6:g_port->setDataBits(QSerialPort::Data6);break;
    case 7:g_port->setDataBits(QSerialPort::Data7);break;
    case 8:g_port->setDataBits(QSerialPort::Data8);break;
    default:break;
    }
}

void serial_thread::initStopBit(QString stopBit)
{
    if(!QString::compare(stopBit,QString("1"),Qt::CaseSensitive)){
        g_port->setStopBits(QSerialPort::OneStop);
    }
    if(!QString::compare(stopBit,QString("2"),Qt::CaseSensitive)){
        g_port->setStopBits(QSerialPort::TwoStop);
    }
}
