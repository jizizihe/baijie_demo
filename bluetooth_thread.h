#ifndef BLUETOOTH_THREAD_H
#define BLUETOOTH_THREAD_H

#include "wifi_bt_interface.h"
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QFile>

class bluetooth_thread : public QObject
{
    Q_OBJECT
public:
    explicit bluetooth_thread(QObject *parent = nullptr);

signals:
    void send_bluetooth_msg(int ,QString );

public slots:
    QString bluetooth_scan_thread();
    QString bluetooth_pair_thread(QString btAddress);
    QString bluetooth_connect_thread(QString btAddress);

private:
    wifi_bt_interface * wifi_bt_interfaceWg;

};

#endif // BLUETOOTH_THREAD_H
