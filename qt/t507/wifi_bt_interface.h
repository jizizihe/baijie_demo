#ifndef WIFI_BT_INTERFACE_H
#define WIFI_BT_INTERFACE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QFile>

class wifi_bt_interface : public QObject
{
    Q_OBJECT
public:
    explicit wifi_bt_interface(QObject *parent = nullptr);

    QString executeLinuxCmd(QString strCmd);

signals:

public slots:
};

#endif // WIFI_BT_INTERFACE_H
