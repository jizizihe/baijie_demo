#ifndef ALL_TEST_H
#define ALL_TEST_H

#include <QMainWindow>
#include "all_test_thread.h"
#include "gpio_interface.h"
#include "serial_dialog.h"
#include "ui_serial_dialog.h"
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
#include <iostream>
#include <QScreen>

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
    void languageReload();
    void imageShow();
    void setAllTestFont();

private slots:
    void serial_config_func(serial_config);
    void serial_test_func();
    void on_btn_testCheckAll_clicked();
    void on_btn_begin_clicked();
    void on_btn_ret_clicked();
    void on_usbChk_clicked();
    void on_serialChk_clicked();
    void recv_test_msg(int, QString );
    void serial_stop_deal();

signals:
    void all_test_back_msg();
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
    void serial_test_start_msg(serial_config);
    void serial_test_stop_msg();

private:
    Ui::all_test *ui;
    serialdialog *g_serialDialog;
    serial_config g_serialConfig;
    QLabel *g_waitLbl;
    QMovie *g_waitMovie;
    QButtonGroup * g_testButtonGroup;
    QThread * g_mainTestThread;
    QThread * g_threadId[16];
    QThread * g_bluetoothThread;
    QList<QAbstractButton*> g_checkedBtnList;
    QTime g_startTime;
    QTime g_stopTime;
    QTimer * g_stopSerialTimer;
    all_test_thread * g_allTestThread;    
    all_test_thread * g_btTestThead;    
    all_test_thread * g_serialTestThread[16];
    int g_elapsed;
    int g_usbAddNum = 0;
    int g_testItemsCount = 0;
};

#endif // ALL_TEST_H
