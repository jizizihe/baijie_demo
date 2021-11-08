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
    explicit serial_thread(int portId,QString port,long baud,int *OpenFlag,QObject *parent = nullptr);

    void show_fun_id();
    void init_port(QString port,long baud,int *OpenFlag);//初始化串口

signals:
    void receive_data(QString buff);//向主线程发送接收到的串口数据
    void thread_sig();

private slots:
    void write_data(int portId,QByteArray buff);//写串口数据 /*QByteArray*/
    void closePort(int portId);

    void read_data();//读取串口数据
    void show_slots_id();

private:
    QThread *my_thread;
    int m_portId;
    QSerialPort *m_port;//实例化一个指向串口的指针，用于访问串口
    QTimer *m_readTimer;
};

#endif // SERIAL_THREAD_H
