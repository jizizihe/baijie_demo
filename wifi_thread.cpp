#include "wifi_thread.h"

wifi_thread::wifi_thread(QObject *parent) : QObject(parent)
{

}

void wifi_thread::wifi_scan_thread()
{
    QString strResult = g_wifiInterface->wifiScan();
    emit send_wifi_msg(EnumWifiScanSignal,strResult);
}

void wifi_thread::wifi_activation_thread(QString wifiName)
{
    QString strResult;
    strResult = g_wifiInterface->wifiActivation(wifiName);
    bool ConnectResult=strResult.contains("Connection successfully activated",Qt::CaseInsensitive);

    if(ConnectResult == 1)
    {
        strResult = QString(1);
    }
    else
    {
        strResult = QString("");
    }
    emit send_wifi_msg(EnumWifiActivationSignal,strResult);
    emit wifi_info_refresh_msg(wifiName);
}

void wifi_thread::scan_wlan()
{
    QString strResult;
    strResult = g_wifiInterface->getWlan();
    emit revc_scan_wlan_msg(strResult);
}

void wifi_thread::wifi_connect_thread(QString wifiName,QString wifiPasswd)
{
    QString strResult;
    bool flag = g_wifiInterface->wifiConnectExistFlag(wifiName);
    if(flag == true)
    {
        strResult = g_wifiInterface->wifiActivation(wifiName);
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
        strResult = g_wifiInterface->wifiConnect(wifiName,wifiPasswd);
    }
    emit wifi_wait_end_msg();
    emit send_wifi_msg(EnumWifiConnectSignal,strResult);
}

void wifi_thread::hotspot_build_thread(QString HtWlan,QString HtName,QString HtPasswd)
{
    QString strResult;
    strResult = g_wifiInterface->hotspotConnect(HtWlan,HtName,HtPasswd);
    emit send_wifi_msg(EnumHotspotBuildSignal ,strResult);
}

void wifi_thread::wifi_modyfy_pass(QString wifiSsid, QString passWd)
{
    bool strResult = g_wifiInterface->wifiModifyPasswd(wifiSsid,passWd);
    emit wifi_modify_pass_msg(strResult);
}
