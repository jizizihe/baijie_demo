#ifndef UDEV_INTERFACE_H
#define UDEV_INTERFACE_H


#include <QString>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QFileInfo>
#include "gpio_interface.h"

enum test_signal_type
{
    network_signal = 0,
    usb_signal = 1,
    rtc_signal = 2,
    sd_card_signal = 3,
    key_signal = 4,
    serial_signal = 5,
    camera_signal = 6,
    battary_signal = 7,
    sim_signal = 8,
    audio_signal = 9,
    wifi_signal = 10,
    bluetooth_signal = 11
};

QString usb_test(int num);
QString get_new_usb();
QString get_new_sd();
QString get_gateway();
QString ping_gateway();
QString rtc_test();
void camera_test();
QString battary_test();
QString sim_test();
QString audio_test();
QString wifi_test();
QString bluetooth_test();


QString executeLinuxCmd(QString strCmd);

#endif // UDEV_INTERFACE_H
