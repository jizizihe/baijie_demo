#include "bluetooth_thread.h"

bluetooth_thread::bluetooth_thread(QObject *parent) : QObject(parent)
{
}

QString bluetooth_thread::bluetooth_scan_thread()
{
    QString scanResult = wifi_bt_interfaceWg->bluetoothScan();
    emit send_bluetooth_msg(btScanSignal,scanResult);
    return scanResult;
}

QString bluetooth_thread::bluetooth_pair_thread(QString btAddress)
{
    QString pairResult = wifi_bt_interfaceWg->bluetoothPair(btAddress);
    emit send_bluetooth_msg(btPairSignal,pairResult);
    return pairResult;
}

QString bluetooth_thread::bluetooth_connect_thread(QString btAddress)
{
    QString connectResult = wifi_bt_interfaceWg->bluetoothConnect(btAddress);
    emit send_bluetooth_msg(btConnectSignal,connectResult);
    return connectResult;
}
