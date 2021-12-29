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
    void send_msg(int ,QString );
    void sim_send_msg(int, QString);
    void wifi_info_fresh_msg(QString);
    void wifi_wait_end_msg();

public slots:
    void Thread_Fun();
    void wifi_scan_thread();
    void wifi_connect_thread(QString,QString);
    void wifi_activation_thread(QString wifi_name);
    void hotspot_build_thread(QString HtName,QString HtPasswd);
    void sim_connect_thread_func();
    void sim_disconnect_thread_func();
    void sim_status_thread_func();
private:
    wifi_bt_interface * wifi_bt_w;
};

#endif // WIFI_THREAD_H
