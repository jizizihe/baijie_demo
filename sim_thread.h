#ifndef SIM_THREAD_H
#define SIM_THREAD_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include "sim_interface.h"

class sim_thread: public QObject
{
    Q_OBJECT
public:
    explicit sim_thread(QObject *parent = nullptr);

public slots:
    void sim_connect_thread();
    void sim_disconnect_thread();
    void sim_status_thread();
    void sim_module_status();
    void sim_set_enbale(int);

signals:
    void send_sim_msg(int, QString);

private:
    sim_interface *g_simInterface;
};

#endif // SIM_THREAD_H
