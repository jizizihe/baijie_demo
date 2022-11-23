#ifndef SERIAL_H
#define SERIAL_H

#include "serial_thread.h"
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QScreen>
#include <QApplication>

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
#include <iostream>
#include <QScreen>
#include "database.h"

#define SerialNumMax 10

enum EnumComId
{
    EnumComNull =-1,
    EnumCom0 = 0,
    EnumCom1 = 1,
    EnumCom2= 2
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
    void languageReload();
    void setSerialFont();
    void openSerialPort1();
    void openSerialPort2();

signals:
    void serial_back_msg();
    void close_port_msg(int portId);
    void write_port_msg(int portId,QByteArray buff);

private slots:
    void show_port1_data(QString buff);    // Serial port1 displays data
    void show_port2_data(QString buff);    // Serial port2 displays data
    void on_btn_send1_clicked();
    void on_btn_send2_clicked();
    void on_btn_ret_clicked();
    void on_btn_port1_clicked();
    void on_btn_port2_clicked();
    void on_btn_set1_clicked();
    void on_btn_set1_2_clicked();
    void on_btn_ok2_clicked();
    void on_btn_cancel2_clicked();
    void on_btn_ok1_clicked();
    void on_btn_cancel1_clicked();
    void on_btn_open1_clicked();
    void on_btn_open2_clicked();
    void on_btn_clear1_clicked();
    void on_btn_clear1_2_clicked();

private:
    Ui::serial *ui;
    serial_thread *g_PortA;
    serial_thread *g_PortB;
    QStringList g_portNameList;
    database g_database;
};

#endif // SERIAL_H
