#ifndef WIFI_THREAD_H
#define WIFI_THREAD_H

#include "wifi_interface.h"
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QProcess>

class wifi_thread : public QObject
{
    Q_OBJECT
public:
    explicit wifi_thread(QObject *parent = nullptr);

signals:
    void send_wifi_msg(int ,QString );
    void wifi_info_refresh_msg(QString);
    void wifi_wait_end_msg();
    void revc_scan_wlan_msg(QString);
    void wifi_modify_pass_msg(bool);

public slots:
    void wifi_scan_thread();
    void wifi_connect_thread(QString,QString);
    void wifi_activation_thread(QString wifiName);
    void hotspot_build_thread(QString HtWlan,QString HtName,QString HtPasswd);

    void scan_wlan();
    void wifi_modyfy_pass(QString wifiSsid, QString passWd);

private:
    wifi_interface * g_wifiInterface;
};

#endif // COM_THREAD_H
