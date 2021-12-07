#ifndef WIFI_BT_INTERFACE_H
#define WIFI_BT_INTERFACE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QLabel>
#include <QFile>
#include <QMovie>
#include<QScreen>
#include <QScrollBar>
#include <QListWidget>
#include <QInputDialog>
#include <QDialog>
#include <QMessageBox>

#include "switchcontrol.h"

enum signal_type
{
    siganl_NULL = 0,
    Bt_scan_signal = 1,
    Bt_pair_signal = 2,
    Bt_connect_signal = 3,
    Bt_disconnect_signal = 4,
    wifi_scan_signal = 5,
    wifi_connect_signal = 6,
    wifi_activation_signal = 7,
    wifi_modify_signal = 8,
    hotspot_build_signal,
};

class wifi_bt_interface : public QObject
{
    Q_OBJECT
public:
    explicit wifi_bt_interface(QObject *parent = nullptr);

    QString executeLinuxCmd(QString strCmd);

    bool get_wifiopenorno();
    QString wifi_enable(bool flag);
    QString wifi_scan();
    QString get_wifistatus();
    QString get_wifisignalquality();
    QString get_wifisecurity(QString infoname);

    QString wifi_disconnect();
    bool wifi_connect_exist(QString );
    QString wifi_activation(QString );
    QString wifi_connection_remove(QString WifiSsid);
    QString wifi_connect(QString WifiSsid,QString PassWd);
    bool wifi_modify(QString WifiSsid,QString PassWd);
    QString hotspot_connect(QString HtName,QString HtPasswd);
    QString hotspot_disconnect();


signals:

public slots:
};

#endif // WIFI_BT_INTERFACE_H
