#ifndef ALL_INTERFACE_TEST_H
#define ALL_INTERFACE_TEST_H

#include <QMainWindow>
#include "serialdialog.h"
#include "ui_serialdialog.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>

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

void camera_interface();

namespace Ui {
class all_interface_test;
}

class all_interface_test : public QMainWindow
{
    Q_OBJECT

public:
    explicit all_interface_test(QWidget *parent = 0);
    ~all_interface_test();
    void click_str();

    bool event(QEvent *event);

    static void *thread_serial_port(void *);
    static int set_prop(int fd);
    void serial_thread(serial_config config);


    void language_reload();
private slots:
    void on_return_2_clicked();

    void on_test_clicked();

    void on_clear_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_gpio_clicked();

    void on_wifi_clicked();

    void on_bluetooth_clicked();

    void on_network_clicked();

    void on_usb_clicked();

    void on_sd_card_clicked();

    void on_camera_clicked();

    void on_check_all_clicked();

    void on_auto_gpio_clicked();

    void on_auto_wifi_clicked();

    void on_auto_bluetooth_clicked();

    void on_auto_network_clicked();

    void on_auto_usb_clicked();

    void on_auto_sd_card_clicked();

    void on_auto_camera_clicked();

    void on_begin_clicked();

    void time_test();

    void all_checkset();

    void on_sim_clicked();

    void on_auto_sim_clicked();

    void on_key_clicked();

    void on_auto_key_clicked();

    void on_auto_serial_clicked();

    void serial_config_func(serial_config);
signals:

    void Mysignal();

private:
    Ui::all_interface_test *ui;
    bool gpio_flag = false;
    bool wifi_flag = false;
    bool blue_flag = false;
    bool usb_flag = false;
    bool sd_card_flag = false;
    bool camera_flag = false;
    bool network_flag = false;
    bool sim_flag = false;
    bool key_flag = false;
    bool serial_flag = false;
    bool all_flag = false;

    serialdialog *serialDialog;
    serial_config serialConfig;

};

#endif // ALL_INTERFACE_TEST_H
