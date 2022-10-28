#ifndef SERIAL_H
#define SERIAL_H

#include "serial_thread.h"
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
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

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScreen>
#include "database.h"

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
    void languageReload();
    void serialFont();
    void openSerialPort1();
    void openSerialPort2();

signals:
    void serial_back_msg();
    void close_port_msg(int portId);
    void write_port_msg(int portId,QByteArray buff);

private slots:
    void show_port1_data(QString buff);  //Serial port1 displays data
    void show_port2_data(QString buff);  //Serial port2 displays data
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
    serial_thread *PortA;
    serial_thread *PortB;
    QStringList portNameList;
    database databaseWg;
};

#endif // SERIAL_H
