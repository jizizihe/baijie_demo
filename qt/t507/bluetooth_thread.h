#ifndef BLUETOOTH_THREAD_H
#define BLUETOOTH_THREAD_H

#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QFile>

class bluetooth_thread : public QThread
{
    Q_OBJECT
public:
    explicit bluetooth_thread(QObject *parent = nullptr);

    virtual void run();
    QString bluetooth_scan();
    QString executeLinuxCmd(QString strCmd);
    QString bluetooth_pair(QString BtAddress);
    QString bluetooth_connect(QString BtAddress);

signals:
    void message(int, QString);
    void setText(bool);

public slots:
    void flag_set(int signal_type,QString address);

private:
    bool scan_flag = false;
    bool pair_flag = false;
    bool connect_flag = false;
    QString BtAddress;
};

#endif // BLUETOOTH_THREAD_H
