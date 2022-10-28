#include "wifi_thread.h"

wifi_thread::wifi_thread(QObject *parent) : QObject(parent)
{

}

void wifi_thread::Thread_Fun()
{

}

void wifi_thread::wifi_scan_thread()
{
    QString strResult = wifi_bt_interfaceWg->wifiScan();
    emit send_wifi_msg(wifiScanSignal,strResult);
}

void wifi_thread::wifi_activation_thread(QString wifiName)
{
    QString strResult;
    strResult = wifi_bt_interfaceWg->wifiActivation(wifiName);
    bool ConnectResult=strResult.contains("Connection successfully activated",Qt::CaseInsensitive);

    if(ConnectResult == 1)
    {
        strResult = QString(1);
    }
    else
    {
        strResult = QString("");
    }
    emit send_wifi_msg(wifiActivationSignal,strResult);
    emit wifi_info_fresh_msg(wifiName);
}

void wifi_thread::scan_wlan()
{
    QString strResult;
    strResult = wifi_bt_interfaceWg->scanWlan();
    emit revc_scan_wlan_msg(strResult);
}

void wifi_thread::wifi_connect_thread(QString wifiName,QString wifiPasswd)
{
    QString strResult;
    bool flag = wifi_bt_interfaceWg->wifiConnectExistFlag(wifiName);
    if(flag == true)
    {
        strResult = wifi_bt_interfaceWg->wifiActivation(wifiName);
        bool ConnectResult=strResult.contains("Connection successfully activated",Qt::CaseInsensitive);

        if(ConnectResult == 1)
        {
            strResult = QString(1);
        }
        else
        {
            strResult = QString("");
        }
    }
    else
    {
        strResult = wifi_bt_interfaceWg->wifiConnect(wifiName,wifiPasswd);
    }
    emit wifi_wait_end_msg();
    emit send_wifi_msg(wifiConnectSignal,strResult);
}

void wifi_thread::hotspot_build_thread(QString HtWlan,QString HtName,QString HtPasswd)
{
    QString strResult;
    strResult = wifi_bt_interfaceWg->hotspotConnect(HtWlan,HtName,HtPasswd);
    emit send_wifi_msg(hotspotBuildSignal ,strResult);
}

void wifi_thread::sim_disconnect_thread()
{
    QString strResult = wifi_bt_interfaceWg->simDisconnect();
    emit send_sim_msg(simDisconnectSignal,strResult);
}

void wifi_thread::sim_connect_thread()
{
    QString strResult = wifi_bt_interfaceWg->simConnect();
    emit send_sim_msg(simConnectSignal,strResult);
}

void wifi_thread::sim_activation(int flag)
{
    wifi_bt_interfaceWg->simActivation(flag);
}

void wifi_thread::sim_status_thread()
{
    QString strResult = wifi_bt_interfaceWg->simStatus();
    emit send_sim_msg(simStatusSignal,strResult);
}

void wifi_thread::sim_module_status()
{
    QString strResult = wifi_bt_interfaceWg->simModuleStatus();
    emit send_sim_msg(simModuleStatusSignal,strResult);
}

void wifi_thread::wifi_modyfy_pass(QString wifiSsid, QString passWd)
{
    bool strResult = wifi_bt_interfaceWg->wifiModifyPasswd(wifiSsid,passWd);
    emit wifi_modify_pass_msg(strResult);
}
