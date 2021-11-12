#ifndef WIFI_THREAD_H
#define WIFI_THREAD_H

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
    void send_msg(QString );

public slots:
    void Thread_Fun();
    void wifi_scan_thread();
};

#endif // WIFI_THREAD_H
