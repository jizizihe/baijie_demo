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
#include<iostream>

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
    void language_reload();
    void serial_font();
    void serialport1_open();
    void serialport2_open();

signals:
    void Mysignal();
    void closePort_sig(int portId);
    void writePort_sig(int portId,QByteArray buff);

private slots:
    void on_showData(QString buff);//用于显示数据

    void on_showData1(QString buff);//用于显示数据
    void on_SendBtn_clicked();

    void on_SendBtn_2_clicked();

    void on_retBtn_clicked();

    void on_part1_btn_clicked();

    void on_part2_btn_clicked();

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
    QStringList m_portNameList;
    database database_w;

};

#endif // SERIAL_H
