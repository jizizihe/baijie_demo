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

enum testSignalType
{
    networkSignal = 0,
    usbSignal = 1,
    rtcSignal = 2,
    sdCardSignal = 3,
    keySignal = 4,
    serialSignal = 5,
    cameraSignal = 6,
    battarySignal = 7,
    simSignal = 8,
    audioSignal = 9,
    wifiSignal = 10,
    bluetoothSignal = 11
};

QString usbTest(int num);
QString getNewSd();
QString getGateway();
QString pingGateway();
QString rtcTest();
void cameraTest();
QString battaryTest();
QString simTest();
QString audioTest();
QString wifiTest();
QString bluetoothTest();
QString executeLinuxCmd(QString strCmd);

#endif // UDEV_INTERFACE_H
