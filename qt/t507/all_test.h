#ifndef ALL_TEST_H
#define ALL_TEST_H

#include <QMainWindow>
#include "all_test_thread.h"
#include "gpio_interface.h"
#include "serialdialog.h"
#include "ui_serialdialog.h"
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

    void checkbox_disenable_func(int itemsCount);

//    void flagSet();

    void testMsgDisplay(QString type,QString str,int time);

    void language_reload();

private slots:

    void serial_config_func(serial_config);
    void serial_test_func();
//    void serial_thread();

    void on_testCheckAllBtn_clicked();

    void on_beginBtn_clicked();

    void on_retBtn_clicked();

    void on_usbChk_clicked();
//    void on_networkChk_clicked();

//    void on_rtcChk_clicked();

//    void on_sdcardChk_clicked();

//    void on_cameraChk_clicked();

//    void on_battaryChk_clicked();

//    void on_simChk_clicked();

//    void on_audioChk_clicked();

//    void on_wifiChk_clicked();

//    void on_bluetoothChk_clicked();

    void on_serialChk_clicked();

    void recv_test_msg(int, QString );

//    void serialStopTimer_func();
    void serial_stop_deal();
    void serial_test_recv_func(QString);

signals:

    void Mysignal();
    void network_test_msg();
    void usb_test_msg(int);
    void rtc_test_msg();
    void sdcard_test_msg();
    void key_test_msg();
    void serial_test_msg();
    void camera_test_msg();
    void battary_test_msg();
    void sim_test_msg();
    void audio_test_msg();
    void wifi_test_msg();
    void bluetooth_test_msg();

    void audio_aplay_msg();

//    void server_test_server_msg(serial_config);
    void serial_test_client_msg(serial_config);
    void serial_test_stop_msg();
    void serial_set_testBt_msg();

//    void serial_send_result_msg(QString);
private:
    Ui::all_test *ui;

    bool network_flag = false;
    bool usb_flag = false;
    bool rtc_flag = false;
    bool sd_card_flag = false;
    bool key_flag = false;
    bool serial_flag = false;
    bool camera_flag = false;
    bool battary_flag = false;
    bool sim_flag = false;
    bool audio_flag = false;
    bool wifi_flag = false;
    bool bluetooth_flag = false;

    serialdialog *serialDialog;
    serial_config serialConfig;
    QButtonGroup * testButtonGroup;

    QThread * myThread;
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

};

#endif // ALL_TEST_H
