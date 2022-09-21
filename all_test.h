#ifndef ALL_TEST_H
#define ALL_TEST_H

#include <QMainWindow>
#include "all_test_thread.h"
#include "gpio_interface.h"
#include "serialdialog.h"
#include "ui_serialdialog.h"
#include "database.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QButtonGroup>
#include <QScreen>
#include <QMovie>
#include <QScrollBar>
#include <QInputDialog>

#include <QMetaType>

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
#include <QScreen>

static QString serial_ret;

namespace Ui {
class all_test;
}

class all_test : public QMainWindow
{
    Q_OBJECT

public:
    explicit all_test(QWidget *parent = 0);
    ~all_test();
    bool event(QEvent *event);

    void testMsgDisplay(QString type,QString str,int time);

    void language_reload();
    void image_show();
    void all_font();

private slots:

    void serial_config_func(serial_config);
    void serial_test_func();

    void on_testCheckAllBtn_clicked();

    void on_beginBtn_clicked();

    void on_retBtn_clicked();

    void on_usbChk_clicked();

    void on_serialChk_clicked();

    void recv_test_msg(int, QString );

    void serial_stop_deal();


signals:
    void Mysignal();
    void network_test_msg();
    void usb_test_msg(int);
    void rtc_test_msg();
    void sdcard_test_msg();
    void key_test_msg();
    void camera_test_msg();
    void battary_test_msg();
    void sim_test_msg();
    void audio_test_msg();
    void wifi_test_msg();
    void bluetooth_test_msg();
    void audio_aplay_msg();

    void serial_test_client_msg(serial_config);
    void serial_test_stop_msg();
//    void serial_set_testBt_msg();

private:
    Ui::all_test *ui;

    QLabel *waitLbl;
    QMovie *waitMovie;

    serialdialog *serialDialog;
    serial_config serialConfig;
    QButtonGroup * testButtonGroup;

    QThread * mainTestThread;
    all_test_thread * allTestThread;
    QThread * btThread;
    all_test_thread * btTestThead;
    all_test_thread * audioThread;

    QThread * thread_id[16];
    all_test_thread * serial_test_thread[16];

    QTime startTime;
    QTime stopTime;
    int elapsed;
    int usbAddNum = 0;
    QTimer * serialStopTimer;
    int testItemsCount = 0;
    QList<QAbstractButton*> CheckedBtnList;
    database database_w;
};

#endif // ALL_TEST_H
