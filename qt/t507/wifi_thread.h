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

public slots:
    void Thread_Fun();
    void wifi_scan_thread();
    void wifi_connect_thread(QString,QString);
    void wifi_activation_thread(QString wifi_name);
    void hotspot_build_thread(QString HtName,QString HtPasswd);


private:
    wifi_bt_interface * wifi_bt_w;
};

#endif // WIFI_THREAD_H
