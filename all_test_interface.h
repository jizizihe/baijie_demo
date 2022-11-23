#ifndef ALL_INTERFACE_H
#define ALL_INTERFACE_H
#include <QString>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QFileInfo>
#include "gpio_interface.h"

enum EnumTestSignalType
{
    EnumNetworkSignal = 0,
    EnumUsbSignal = 1,
    EnumRtcSignal = 2,
    EnumSdCardSignal = 3,
    EnumKeySignal = 4,
    EnumSerialSignal = 5,
    EnumCameraSignal = 6,
    EnumBattarySignal = 7,
    EnumSimSignal = 8,
    EnumAudioSignal = 9,
    EnumWifiSignal = 10,
    EnumBluetoothSignal = 11
};

QString usbTest(int num);
QString getSdCard();
QString getGateway();
QString pingGateway();
QString rtcTest();
QString battaryTest();
QString simTest();
QString wifiTest();
QString bluetoothTest();
QString executeLinuxCmd(QString strCmd);
void cameraTest();
void audioTest();

#endif // ALL_INTERFACE_H
