#ifndef SERIAL_THREAD_H
#define SERIAL_THREAD_H

#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QThread>
#include <QByteArray>
#include <QTimer>

class serial_thread : public QObject
{
    Q_OBJECT
public:
    explicit serial_thread(int portId,QString port,long baud,int dataBit,QString stopBit,int *openFlag,QObject *parent = nullptr);
    void initPort(QString port, long baud, int dataBit, QString stopBit, int *openFlag);  // Initialize the serial ports
    void initDataBit(int dataBit);
    void initStopBit(QString stopBit);

signals:
    void receive_data_msg(QString buff);  //  Sends the received serial port data to the main thread

private slots:
    void write_data(int portId,QByteArray buff); // Writing serial port Data
    void close_port(int portId);
    void read_data();  // Reading serial port data

private:
    QThread *g_thread;
    int g_portId;
    QSerialPort *g_port;
};

#endif // SERIAL_THREAD_H
