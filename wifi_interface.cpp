#include "wifi_interface.h"


wifi_interface::wifi_interface(QObject *parent) : QObject(parent)
{

}

wifi_interface::~wifi_interface()
{

}

QString wifi_interface::executeLinuxCmd(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}

bool wifi_interface::wifiOpenFlag()
{
    QString strCmd;
    QString strResult;

    strCmd = QString("ifconfig |grep wlan |wc -l");
    strResult = executeLinuxCmd(strCmd);
    if(strResult == "0\n")
    {
        return false;
    }
    else
    {
        return true;
    }
    return false;
}

QString wifi_interface::wifiEnableFlag(bool flag)
{
    QString strCmd;
    QString strResult;
    if(flag == true)
    {
        strCmd = QString("ifconfig wlan0 up");
        strResult = executeLinuxCmd(strCmd);
    }
    else
    {
        strCmd = QString("ifconfig wlan0 down");
        strResult = executeLinuxCmd(strCmd);
    }
    return strResult;
}

QString wifi_interface::wifiScan()
{
    bool openFlag = wifiOpenFlag();
    if(openFlag == false)
    {
        wifiEnableFlag(true);
        QThread::msleep(3000);
    }

    QString strCmd = QString("nmcli device wifi rescan");
    QString ScanResult = executeLinuxCmd(strCmd);
    strCmd = QString("nmcli -t  device wifi list|awk -F : '{print $2\":\"$6}' ");
    ScanResult = executeLinuxCmd(strCmd);
    return ScanResult;
}

wifi_info wifi_interface::getWifiStatus(QString wifiName)
{
    int index = -1;
    QString tmp;
    QString findString,addressResult;

    g_WifiStatus.active.clear();
    g_WifiStatus.name.clear();
    g_WifiStatus.passwd.clear();
    g_WifiStatus.secrity.clear();
    g_WifiStatus.signal.clear();
    g_WifiStatus.ipAddress.clear();

    QString strCmd = QString("nmcli device wifi rescan");
    QString strResult = executeLinuxCmd(strCmd);

    strCmd = QString("nmcli --terse --fields active,ssid,signal,security  device wifi list");
    strResult = executeLinuxCmd(strCmd);
    QStringList scanlist = strResult.split("\n");

    for(int i = 0; i < scanlist.size(); i++)
    {
        tmp = scanlist.at(i);
        findString = tmp.section(':', 1, 1);
        if(findString == wifiName)
        {
            index = i;break;
        }
    }

    if(index == -1)
    {
        return g_WifiStatus;
    }

    tmp = scanlist.at(index);
    g_WifiStatus.active = tmp.section(':', 0, 0);
    g_WifiStatus.name = wifiName;
    g_WifiStatus.passwd = g_database.selectTableName(QString("wifiPasswd"),wifiName);
    g_WifiStatus.secrity = tmp.section(':', 3, 3);
    g_WifiStatus.signal = tmp.section(':', 2, 2);

    if(QString("yes") == g_WifiStatus.active)
    {
        strCmd = QString("wpa_cli -i wlan0 status | grep ip_address|cut -d '=' -f 2");
        addressResult = executeLinuxCmd(strCmd);
        addressResult.replace("\n","");
        g_WifiStatus.ipAddress = addressResult.replace("="," ");
    }
    return g_WifiStatus;
}

QString wifi_interface::getWifiSignalQuality()
{
    QString strCmd = QString("iw dev wlan0 link | grep SSID |awk '{print $2}'");
    QString signalQualityResult = executeLinuxCmd(strCmd);

    QString strResult = QString("wifi signal quality:\n");
    strCmd = QString("nmcli device wifi list|grep '%1'|awk '{print $7}'|sed -n '1p' ").arg(signalQualityResult.remove("\n"));
    signalQualityResult = executeLinuxCmd(strCmd);

    if(signalQualityResult == QString("BARS\n"))
    {
        signalQualityResult = QString("please try again!");
    }

    return strResult.append(signalQualityResult);
}

QString wifi_interface::getWifiSecurity(QString infoName)
{
    QString strCmd = QString("nmcli connection show %1|grep key-mgmt | awk '{printf $2}'").arg(infoName);
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

QString wifi_interface::wifiDisconnect()
{
    QString strCmd = QString("nmcli device disconnect wlan0");
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

bool wifi_interface::wifiConnectExistFlag(QString WifiSsid)
{
    QString strCmd = QString("ifconfig wlan0 up");
    QString strResult = executeLinuxCmd(strCmd);
    WifiSsid = WifiSsid + " ";
    strCmd = QString("nmcli connection show |grep 'wifi'| grep '%1' |awk 'NR==1'").arg(WifiSsid);
    strResult = executeLinuxCmd(strCmd);
    WifiSsid.remove(WifiSsid.size()-1,1);strResult.remove(strResult.size()-1,1);
    QString findStr = QString("%1").arg(WifiSsid);

    bool checktResult=strResult.contains(findStr,Qt::CaseInsensitive);
    if(checktResult == 1)
    {
        strResult = "it had connected!";

        return true;
    }
    else
        return false;
}

QString wifi_interface::wifiActivation(QString WifiSsid)
{
    QString strCmd = QString("nmcli connection up '%1' ").arg(WifiSsid);
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

QString wifi_interface::wifiRemove(QString WifiSsid)
{
    QString strCmd = QString("nmcli connection delete '%1' ").arg(WifiSsid);
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

QString wifi_interface::wifiConnect(QString WifiSsid,QString PassWd)
{
    QString strCmd;
    QString strResult;
    strCmd = QString("nmcli device wifi connect '%1' password '%2' name '%3' ").arg(WifiSsid).arg(PassWd).arg(WifiSsid);
    strResult = executeLinuxCmd(strCmd);
    bool ConnectResult=strResult.contains("successfully activated",Qt::CaseInsensitive);

    if(ConnectResult == 1)
    {
        strResult = "Connection successful!";
        return QString(1);
    }
    else
    {
        strResult = "Connection failed!";
        strCmd = QString("nmcli connection delete \"%1\" ").arg(WifiSsid);
        strResult = executeLinuxCmd(strCmd);
        bool ConnectResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);
        if(ConnectResult == 1)
        {
            strResult = "delete successful!";
        }
        else
        {
            strResult = "delete failed!";
        }
        return 0;
    }
    return 0;
}

bool wifi_interface::wifiModifyPasswd(QString WifiSsid,QString PassWd)
{
    if(WifiSsid.isEmpty() || PassWd.isEmpty())
    {
        return false;
    }

    QString strCmd = QString("nmcli connection modify '%1' wifi-sec.psk '%2' ").arg(WifiSsid).arg(PassWd);
    QString strResult = executeLinuxCmd(strCmd);
    strCmd = QString("nmcli connection up '%1'").arg(WifiSsid);qDebug() <<strCmd;
    strResult = executeLinuxCmd(strCmd);
    if(strResult.contains("successfully",Qt::CaseSensitive))
    {
        return true;
    }
    return false;
}

QString wifi_interface::hotspotConnect(QString HtWlan,QString HtName,QString HtPasswd)
{
    QString strCmd;
    QString strResult;

    bool openFlag = wifiOpenFlag();
    if(openFlag == false)
    {
        wifiEnableFlag(true);
        QThread::msleep(3000);
    }

    strCmd = QString("nmcli con show|grep hotspot | awk '{print $1}'");
    QString num = executeLinuxCmd(strCmd);

    if(!num.isEmpty())
    {
        strCmd = QString("nmcli connection modify hotspot ssid '%1' wifi-sec.psk '%2' ").arg(HtName).arg(HtPasswd);
        strResult = executeLinuxCmd(strCmd);

        strCmd = QString("nmcli connection up hotspot");
        strResult = executeLinuxCmd(strCmd);
    }
    else
    {
        strCmd = QString("nmcli device wifi hotspot con-name hotspot ifname '%1' ssid '%2' password '%3' ").arg(HtWlan).arg(HtName).arg(HtPasswd);
        strResult = executeLinuxCmd(strCmd);
    }

    bool ConnectResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
    if(ConnectResult == true)
    {
        strResult = "build successful!";
        strResult = QString(1);
        HtName = QString("hotspot%1").arg(HtName);
    }
    else
    {
        strResult = "build failed!";
        strResult = QString("");
    }

    return strResult;
}

bool wifi_interface::hotspotDisconnect()
{
    QString strCmd;
    QString strResult;

    strCmd = QString("nmcli con down hotspot");
    strResult = executeLinuxCmd(strCmd);

    bool ConnectResult=strResult.contains("successfully deactivated",Qt::CaseInsensitive);
    if(ConnectResult == 1)
    {
        strResult = tr("successfully deactivated!");
        return true;
    }
    else
    {
        strResult = tr("deactivated failed!");
        return false;
    }
}

QString wifi_interface::getWlan()
{
    QString strCmd = QString("ifconfig | grep 'wlan' \n");
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

bool wifi_interface::hotspotConnectFlag()
{
    int num;
    QString strCmd = QString("nmcli connection show | awk '{print $4}' \n");
    QString strResult = executeLinuxCmd(strCmd);
    QStringList list = strResult.split("\n");
    list.removeAll("");

    for(int i = 0;i<list.size();i++)
    {
        QString str = list.at(i);
        if(str.contains("wlan",Qt::CaseInsensitive))
        {
            num = i+1;
            QString strCmd = QString(" nmcli con show | awk 'NR==%1'| awk '{print $1}' \n").arg(num);
            QString strResult = executeLinuxCmd(strCmd);
            if(strResult == "hotspot\n")
                return true;
        }
    }
    return false;
}
