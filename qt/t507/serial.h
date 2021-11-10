#ifndef SERIAL_H
#define SERIAL_H

#include "serial_thread.h"
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include<iostream>

#define SerialNumMax 10

enum COMId
{
    COM_NULL =-1,
    COM0 = 0,
    COM1 = 1,
    COM2= 2
};

namespace Ui {
class serial;
}

class serial : public QMainWindow
{
    Q_OBJECT

public:
    explicit serial(QWidget *parent = 0);
    ~serial();

    QStringList getPortNameList();
    void language_reload();

signals:
    void Mysignal();
    void closePort_sig(int portId);
    void writePort_sig(int portId,QByteArray buff);

private slots:
    void on_showData(QString buff);//用于显示数据

    void on_OpenBtn_clicked();

    void on_OpenBtn_2_clicked();

    void on_SendBtn_clicked();

    void on_SendBtn_2_clicked();

    void on_CleanBtn_clicked();

    void on_CleanBtn_2_clicked();

    void on_retBtn_clicked();

private:
    Ui::serial *ui;

    serial_thread *PortA;
    serial_thread *PortB;
    QStringList m_portNameList;
};

#endif // SERIAL_H
