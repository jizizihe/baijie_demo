#include "sim_thread.h"

sim_thread::sim_thread(QObject *parent) : QObject(parent)
{

}

void sim_thread::sim_disconnect_thread()
{
    QString strResult = g_simInterface->simDisconnect();
    emit send_sim_msg(EnumSimDisconnectSignal,strResult);
}

void sim_thread::sim_connect_thread()
{
    QString strResult = g_simInterface->simConnect();
    emit send_sim_msg(EnumSimConnectSignal,strResult);
}

void sim_thread::sim_set_enbale(int flag)
{
    g_simInterface->setSimEnable(flag);
}

void sim_thread::sim_status_thread()
{
    QString strResult = g_simInterface->getSimStatus();
    emit send_sim_msg(EnumSimStatusSignal,strResult);
}

void sim_thread::sim_module_status()
{
    QString strResult = g_simInterface->getSimModuleStatus();
    emit send_sim_msg(EnumSimModuleStatusSignal,strResult);
}
