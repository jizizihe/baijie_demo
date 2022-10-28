#ifndef WIFI_BT_INTERFACE_H
#define WIFI_BT_INTERFACE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QMovie>
#include <QScreen>
#include <QScrollBar>
#include <QListWidget>
#include <QInputDialog>
#include <QDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QHBoxLayout>
#include "gpio_interface.h"
#include "switchcontrol.h"
#include "database.h"

enum signal_type
{
    siganlNULL = 0,
    btScanSignal = 1,
    btPairSignal = 2,
    btConnectSignal = 3,
    btDisconnectSignal = 4,
    wifiScanSignal = 5,
    wifiConnectSignal = 6,
    wifiActivationSignal = 7,
    wifiModifySignal = 8,
    hotspotBuildSignal,
    simDisconnectSignal = 11,
    simActivationSignal = 12,
    simDeleteSignal = 13,
    simConnectSignal = 14,
    simStatusSignal = 15,
    simModuleStatusSignal = 16,
};

typedef struct WIFI_INFO
{
    QString name;
    QString passwd;
    QString active;
    QString signal;
    QString secrity;
    QString ipAddress;
}wifi_info;

class wifi_bt_interface : public QObject
{
    Q_OBJECT
public:
    explicit wifi_bt_interface(QObject *parent = nullptr);
    ~wifi_bt_interface();

    QString executeLinuxCmd(QString strCmd);
    QString executeLinuxCmdBluetoothOpen(QString strCmd);
    QString executeLinuxCmdBluetoothConnect(QString strCmd);

    bool wifiOpenFlag();
    QString wifiEnableFlag(bool flag);
    QString wifiScan();
    wifi_info getWifiStatus(QString wifiName);
    QString getWifiSignalQuality();
    QString getWifiSecurity(QString infoName);

    QString wifiDisconnect();
    bool wifiConnectExistFlag(QString );
    QString wifiActivation(QString );
    QString wifiConnectionRemove(QString WifiSsid);
    QString wifiConnect(QString WifiSsid,QString PassWd);
    bool wifiModifyPasswd(QString WifiSsid,QString PassWd);
    QString hotspotConnect(QString HtWlan,QString HtName,QString HtPasswd);
    bool hotspotDisconnect();
    bool hotspotConnectFlag();

    QString simDisconnect();
    void simActivation(int);
    QString simConnect();
    QString simStatus();   //connection network status
    QString simModuleStatus();   //sim module status

    QString bluetoothScan();
    QString bluetoothPair(QString BtAddress);
    QString bluetoothConnect(QString BtAddress);
    void bluetoothEnable(bool);
    QString getBluetoothName();
    QString setBluetoothName(QString bluetoothName);
    QString bluetoothConnectFlag();
    void bluetoothShellDelete();
    void bluetoothDisconnect(QString);
    void bluetoothRemove(QString BtAddress);

    QString scanWlan();
    void bluetoothScan2();  //Process data before scanning


signals:

public slots:
    void rm_bluetooth_shell();

private:
    wifi_info WifiStatus;
    database databaseWg;

};

#endif // WIFI_BT_INTERFACE_H
