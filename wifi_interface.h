#ifndef WIFI_INTERFACE_H
#define WIFI_INTERFACE_H

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
#include "database.h"

enum EnumSignalType
{
    EnumWifiScanSignal = 1,
    EnumWifiConnectSignal = 2,
    EnumWifiActivationSignal = 3,
    EnumWifiModifySignal = 4,
    EnumHotspotBuildSignal = 5,
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

class wifi_interface : public QObject
{
    Q_OBJECT
public:
    explicit wifi_interface(QObject *parent = nullptr);
    ~wifi_interface();

    QString executeLinuxCmd(QString strCmd);   
    QString wifiEnableFlag(bool flag);
    QString wifiScan();
    QString getWifiSignalQuality();
    QString getWifiSecurity(QString infoName);
    QString wifiDisconnect();
    QString wifiActivation(QString );
    QString wifiRemove(QString WifiSsid);
    QString wifiConnect(QString WifiSsid,QString PassWd);  
    QString getWlan();
    wifi_info getWifiStatus(QString wifiName);
    bool wifiModifyPasswd(QString WifiSsid,QString PassWd);
    bool wifiConnectExistFlag(QString );
    bool wifiOpenFlag();
    QString hotspotConnect(QString HtWlan,QString HtName,QString HtPasswd);
    bool hotspotDisconnect();
    bool hotspotConnectFlag();

private:
    wifi_info g_WifiStatus;
    database g_database;

};

#endif // WIFI_BT_INTERFACE_H
