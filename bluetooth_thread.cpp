#include "bluetooth_thread.h"

bluetooth_thread::bluetooth_thread(QObject *parent) : QObject(parent)
{
}

QString bluetooth_thread::bluetooth_scan_thread()
{
    QString scanResult = g_bluetoothInterface->bluetoothScan();
    emit send_bluetooth_msg(EnumBtScanSignal,scanResult);
    return scanResult;
}

QString bluetooth_thread::bluetooth_pair_thread(QString btAddress)
{
    QString pairResult = g_bluetoothInterface->bluetoothPair(btAddress);
    emit send_bluetooth_msg(EnumBtPairSignal,pairResult);
    return pairResult;
}

QString bluetooth_thread::bluetooth_connect_thread(QString btAddress)
{
    QString connectResult = g_bluetoothInterface->bluetoothConnect(btAddress);
    emit send_bluetooth_msg(EnumBtConnectSignal,connectResult);
    return connectResult;
}
