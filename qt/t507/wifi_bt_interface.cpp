#include "wifi_bt_interface.h"

wifi_bt_interface::wifi_bt_interface(QObject *parent) : QObject(parent)
{

}


QString wifi_bt_interface::executeLinuxCmd(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}

bool wifi_bt_interface::get_wifiopenorno()
{
    QString strCmd;
    QString strResult;

    strCmd = QString("ifconfig |grep wlan |wc -l");
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << "LINE:" << __LINE__ << "ScanResult"<< strResult;
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

QString wifi_bt_interface::wifi_enable(bool flag)
{
    QString strCmd;
    QString strResult;
    if(flag == true)
    {
        strCmd = QString("ifconfig wlan0 up");
        strResult = executeLinuxCmd(strCmd);
        qDebug() << "LINE:" << __LINE__ << "ScanResult"<< strCmd;
//        QThread::msleep(3000);
    }
    else
    {
        strCmd = QString("ifconfig wlan0 down");
        strResult = executeLinuxCmd(strCmd);
        qDebug() << "LINE:" << __LINE__ << "ScanResult"<< strCmd;
    }
    return strResult;
}

QString wifi_bt_interface::wifi_scan()
{
    bool wifi_open_flag = get_wifiopenorno();
    if(wifi_open_flag == false)
    {
        wifi_enable(true);
        QThread::msleep(3000);
    }

    QString strCmd = QString("iw dev wlan0 scan|grep SSID|awk '{for(i=2;i<=NF;i++){printf \"%s \", $i}; printf \"\\n\"}' ");
    //qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<<strCmd;
    QString ScanResult = executeLinuxCmd(strCmd);
    //qDebug() << "LINE:" << __LINE__ << "ScanResult"<< ScanResult;
    return ScanResult;
}

QString wifi_bt_interface::get_wifistatus()
{
    //QString strCmd = QString("nmcli device wifi |awk '{print $8}'");//wifi or hotspot
    QString strCmd = QString("iw dev wlan0 link");
    QString statusResult = executeLinuxCmd(strCmd);
    qDebug() << "LINE:" << __LINE__ << "statusResult" << statusResult;

    if(statusResult == QString("Not connected.\n"))
    {
        statusResult.append("please try again!");
        return statusResult;
    }

    strCmd = QString("wpa_cli -i wlan0 status | grep ip_address");
    QString addressResult = executeLinuxCmd(strCmd);
    qDebug() << "LINE:" << __LINE__ << "statusResult"<< addressResult;

    QString strResult = QString("%1\n%2").arg(statusResult).arg(addressResult);
    return strResult;
}

QString wifi_bt_interface::get_wifisignalquality()
{
    QString strCmd = QString("iw dev wlan0 link | grep SSID |awk '{print $2}'");
    QString QualityResult = executeLinuxCmd(strCmd);

    QString strResult = QString("wifi signal quality:\n");
    strCmd = QString("nmcli device wifi list|grep '%1'|awk '{print $7}'|sed -n '1p' ").arg(QualityResult.remove("\n"));
    QualityResult = executeLinuxCmd(strCmd);
    qDebug() << "LINE:" << __LINE__ << "QualityResult"<< QualityResult;

    if(QualityResult == QString("BARS\n"))
    {
        QualityResult = QString("please try again!");
    }

    return strResult.append(QualityResult);
}

QString wifi_bt_interface::wifi_disconnect()
{
    QString strCmd = QString("nmcli device disconnect wlan0");
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;
    return strResult;
}

bool wifi_bt_interface::wifi_connect_exist(QString WifiSsid)
{
    QString strCmd = QString("ifconfig wlan0 up");
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << "--line--: " << __LINE__<< strResult;

    strCmd = QString("nmcli connection show | grep %1 | awk '{print $1}'").arg(WifiSsid);
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << "--line--: " << __LINE__<< strResult;

    QString find_str = QString("%1\n").arg(WifiSsid);
    //qDebug() << "--line--: " << __LINE__<<"find_str = " << find_str;
    bool checktResult=strResult.contains(find_str,Qt::CaseInsensitive);
    //qDebug() << "--line--: " << __LINE__<< checktResult;

    if(checktResult == 1)
    {
        strResult = "it had connected!";
        qDebug() << "--line--: " << __LINE__<< strResult;

        return true;
    }
    else
    {
        return false;
    }

    return false;
}

QString wifi_bt_interface::wifi_activation(QString WifiSsid)
{
    QString strCmd = QString("nmcli connection up %1").arg(WifiSsid);
    //qDebug() << "--line--: " << __LINE__<<"strcmd = " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << "--LINE--: " << __LINE__<< "func:" << __FUNCTION__<< strResult;

    return strResult;
}

QString wifi_bt_interface::wifi_connection_remove(QString WifiSsid)
{
    QString strCmd = QString("nmcli connection delete %1").arg(WifiSsid);
    //qDebug() << "--line--: " << __LINE__<<"strcmd = " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << "--LINE--: " << __LINE__<< "func:" << __FUNCTION__<< strResult;

    return strResult;
}


QString wifi_bt_interface::wifi_connect(QString WifiSsid,QString PassWd)
{
    QString strCmd;
    QString strResult;

    //qDebug() << "--line--: " << __LINE__<< strResult;

    strCmd = QString("nmcli device wifi connect \"%1\" password \"%2\" name %3").arg(WifiSsid).arg(PassWd).arg(WifiSsid);
    qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    qDebug() << "--line--: " << __LINE__<< strResult;

    bool ConnectResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
    //qDebug() << ConnectResult;

    if(ConnectResult == 1)
    {
        strResult = "Connection successful!";
        qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;

        return QString(1);
    }
    else
    {
        strResult = "Connection failed!";
        qDebug() << "--line--: " << __LINE__<< strResult;

        strCmd = QString("nmcli connection delete %1").arg(WifiSsid);
        qDebug() << "--line--: " << __LINE__<< "func:" << __FUNCTION__<< "strCmd == " << strCmd;
        strResult = executeLinuxCmd(strCmd);
        qDebug() << "--line--: " << __LINE__<< strResult;

        bool ConnectResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);
        qDebug() << ConnectResult;
        if(ConnectResult == 1)
        {
            strResult = "delete successful!";
            qDebug() << "--line--: " << __LINE__<< strResult;
        }
        else
        {
            strResult = "delete failed!";
            qDebug() << "--line--: " << __LINE__<< strResult;
        }

        return 0;
    }

    return 0;
}

bool wifi_bt_interface::wifi_modify(QString WifiSsid,QString PassWd)
{
    if(WifiSsid == "" || PassWd == "")
    {
        return "0";
    }

    QString strCmd = QString("nmcli connection modify '%1' wifi-sec.psk '%2' ").arg(WifiSsid).arg(PassWd);
    qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << "--line--: " << __LINE__<< strResult;

    strCmd = QString("echo $?");
    qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    qDebug() << "--line--: " << __LINE__<< strResult;

    if(strResult == "0\n")
    {
        return 1;
    }
    return 0;
}

QString wifi_bt_interface::hotspot_connect(QString HtName,QString HtPasswd)
{
    bool wifi_open_flag = get_wifiopenorno();
    if(wifi_open_flag == false)
    {
        wifi_enable(true);
        QThread::msleep(3000);
    }

    QString strCmd = QString("nmcli device wifi hotspot con-name %1 ifname wlan0 ssid \"%2\" password \"%3\" ").arg(HtName).arg(HtName).arg(HtPasswd);
    //QString strCmd = QString("nmcli connection up hotspot1");//activated hotspot
    //qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << "--line--: " << __LINE__<< strResult;

    bool ConnectResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
    //qDebug() << ConnectResult;
    if(ConnectResult == 1)
    {
        strResult = "build successful!";
        qDebug() << "--line--: " << __LINE__<< strResult;
        strResult = QString(1);
    }
    else
    {
        strResult = "build failed!";
        qDebug() << "--line--: " << __LINE__<< strResult;
        strResult = QString("");
    }

    return strResult;
}

QString wifi_bt_interface::hotspot_disconnect()
{
    QString strCmd = QString("nmcli device status | grep wlan0 | awk '{print $3}'");
    qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << "--line--: " << __LINE__<< strResult;

    if(strResult == "connected\n")
    {
        strCmd = QString("nmcli device disconnect wlan0");
        qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
        strResult = executeLinuxCmd(strCmd);
        qDebug() << "--line--: " << __LINE__<< strResult;

        bool ConnectResult=strResult.contains("successfully disconnected.",Qt::CaseInsensitive);
        //qDebug() << ConnectResult;
        if(ConnectResult == 1)
        {
            strResult = "successfully disconnected.!";
            qDebug() << "--line--: " << __LINE__<< strResult;
            strResult = QString(1);
        }
        else
        {
            strResult = "disconnect failed!";
            qDebug() << "--line--: " << __LINE__<< strResult;
            strResult = QString("");
        }
    }
    else
    {
        qDebug() << strResult;
    }

    return strResult;
}



