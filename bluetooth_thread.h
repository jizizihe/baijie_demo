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
    void send_msg(int ,QString );

public slots:
    QString bluetooth_scan_thread();
    QString bluetooth_pair_thread(QString BtAddress);
    QString bluetooth_connect_thread(QString BtAddress);

private:

    wifi_bt_interface * wifi_bt_w;

};

#endif // BLUETOOTH_THREAD_H
