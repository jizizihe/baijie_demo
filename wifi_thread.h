#ifndef WIFI_THREAD_H
#define WIFI_THREAD_H

#include "wifi_bt_interface.h"
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
    void send_sim_msg(int, QString);
    void wifi_info_fresh_msg(QString);
    void wifi_wait_end_msg();
    void revc_scan_wlan_msg(QString);
    void wifi_modify_pass_msg(bool);

public slots:
    void Thread_Fun();
    void wifi_scan_thread();
    void wifi_connect_thread(QString,QString);
    void wifi_activation_thread(QString wifiName);
    void hotspot_build_thread(QString HtWlan,QString HtName,QString HtPasswd);
    void sim_connect_thread();
    void sim_disconnect_thread();
    void sim_status_thread();
    void sim_module_status();
    void sim_activation(int);
    void scan_wlan();
    void wifi_modyfy_pass(QString wifiSsid, QString passWd);

private:
    wifi_bt_interface * wifi_bt_interfaceWg;
};

#endif // WIFI_THREAD_H
