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
#include<QScreen>
#include <QScrollBar>
#include <QListWidget>
#include <QInputDialog>
#include <QDialog>
#include <QMessageBox>
#include <QFileInfo>
#include "gpio_interface.h"
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

    sim_disconnect_signal = 11,
    sim_activation_signal = 12,
    sim_delete_signal = 13,
    sim_connect_signal = 14,
};

typedef struct WIFI_INFO
{
    QString name;
    QString passwd;
    QString active;
    QString signal;
    QString secrity;
    QString ip_address;
}wifi_info;

class wifi_bt_interface : public QObject
{
    Q_OBJECT
public:
    explicit wifi_bt_interface(QObject *parent = nullptr);
    ~wifi_bt_interface();

    QString executeLinuxCmd(QString strCmd);
    bool wifi_passwd_write(QString WifiSsid,QString PassWd);
    QString wifi_passwd_read(QString WifiSsid);

    void wifi_passwd_delete(QString info);
    void wifi_passwd_change(QString WifiSsid,QString PassWd);

    bool get_wifiopenorno();
    QString wifi_enable(bool flag);
    QString wifi_scan();
    wifi_info get_wifistatus(QString WifiSsid);
    QString get_wifisignalquality();
    QString get_wifisecurity(QString infoname);

    QString wifi_disconnect();
    bool wifi_connect_exist(QString );
    QString wifi_activation(QString );
    QString wifi_connection_remove(QString WifiSsid);
    QString wifi_connect(QString WifiSsid,QString PassWd);
    bool wifi_modify(QString WifiSsid,QString PassWd);
    QString hotspot_connect(QString HtName,QString HtPasswd);
    bool hotspot_disconnect(QString HtName);

    QString sim_disconnect();
    QString sim_delete();
    QString sim_activation();
    QString sim_connect();

    QString bluetooth_scan();
    QString bluetooth_pair(QString BtAddress);
    QString bluetooth_connect(QString BtAddress);
    void bluetooth_enable(bool);
    QString get_bluetooth_name();
    QString set_bluetooth_name(QString bluetooth_name);


signals:

public slots:

private:
    wifi_info WifiStatus;

};

#endif // WIFI_BT_INTERFACE_H
