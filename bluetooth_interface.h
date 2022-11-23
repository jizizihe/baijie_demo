#ifndef BLUETOOTH_INTERFACE_H
#define BLUETOOTH_INTERFACE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QMovie>
#include <QScreen>
#include <QScrollBar>
#include <QListWidget>
#include <QInputDialog>
#include <QDialog>
#include <QMessageBox>
#include <QFileInfo>

enum EnumBluetoothSignalType
{
    EnumSiganlNULL = 0,
    EnumBtScanSignal = 1,
    EnumBtPairSignal = 2,
    EnumBtConnectSignal = 3,
    EnumBtDisconnectSignal = 4,
};

class bluetooth_interface: public QObject
{
    Q_OBJECT
public:
    explicit bluetooth_interface(QObject *parent = nullptr);
    ~bluetooth_interface();

    QString executeLinuxCmd(QString strCmd);
    QString executeLinuxCmdBluetoothOpen(QString strCmd);
    QString bluetoothScan();
    QString bluetoothPair(QString btAddress);
    QString bluetoothConnect(QString btAddress);
    QString getBluetoothName();
    QString setBluetoothName(QString bluetoothName);
    QString bluetoothConnectFlag();

    void bluetoothEnableFlag(bool flag);
    void bluetoothShellDelete();
    void bluetoothDisconnect(QString btAddress);
    void bluetoothRemove(QString btAddress);
    void bluetoothScan2();  // Process data before scanning

public slots:
    void rm_bluetooth_shell();
};

#endif // WIFI_INTERFACE_H
