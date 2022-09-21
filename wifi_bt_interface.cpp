#include "wifi_bt_interface.h"
static int bluetooth_flag;
static int bluetoothscan_flag;

wifi_bt_interface::wifi_bt_interface(QObject *parent) : QObject(parent)
{

}
wifi_bt_interface::~wifi_bt_interface()
{

}


QString wifi_bt_interface::executeLinuxCmd(QString strCmd)
{

    QProcess p;
    if(strCmd == "kill")
    {
        p.kill();
        p.close();
        return "";
    }
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
//        QThread::msleep(3000);
    }
    else
    {
        strCmd = QString("ifconfig wlan0 down");
        strResult = executeLinuxCmd(strCmd);
 //       qDebug() << "LINE:" << __LINE__ << "ScanResult"<< strCmd;
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

//    QString strCmd = QString("iw dev wlan0 scan|grep SSID|awk '{for(i=2;i<=NF;i++){printf \"%s \", $i}; printf \"\\n\"}' ");
    QString strCmd = QString("nmcli -t  device wifi list|awk -F : '{print $2\":\"$6}' ");
    //qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<<strCmd;
    QString ScanResult = executeLinuxCmd(strCmd);
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "ScanResult:" << ScanResult;

    return ScanResult;
}

wifi_info wifi_bt_interface::get_wifistatus(QString wifi_name)
{
    int index = -1;
    QString tmp;
    QString findString,addressResult;

    WifiStatus.active.clear();
    WifiStatus.name.clear();
    WifiStatus.passwd.clear();
    WifiStatus.secrity.clear();
    WifiStatus.signal.clear();
    WifiStatus.ip_address.clear();
    //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__;

    QString strCmd = QString("nmcli device wifi rescan");
    QString strResult = executeLinuxCmd(strCmd);

    strCmd = QString("nmcli --terse --fields active,ssid,signal,security  device wifi list");
    strResult = executeLinuxCmd(strCmd);
    QStringList scanlist = strResult.split("\n");

    for(int i = 0; i < scanlist.size(); i++)
    {
        tmp = scanlist.at(i);
        findString = tmp.section(':', 1, 1);
        if(findString == wifi_name)
        {
            index = i;break;
            //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "index:" << index;
        }
    }

    if(index == -1)
    {
        //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "not find this name";
        return WifiStatus;
    }

    tmp = scanlist.at(index);

    WifiStatus.active = tmp.section(':', 0, 0);
    WifiStatus.name = wifi_name;
    WifiStatus.passwd = database_w.select_by_name(QString("wifiPasswd"),wifi_name);
//    WifiStatus.passwd = wifi_passwd_read(wifi_name);
    WifiStatus.secrity = tmp.section(':', 3, 3);
    WifiStatus.signal = tmp.section(':', 2, 2);

    if(QString("yes") == WifiStatus.active)
    {
        strCmd = QString("wpa_cli -i wlan0 status | grep ip_address|cut -d '=' -f 2");
        addressResult = executeLinuxCmd(strCmd);
        addressResult.replace("\n","");
        WifiStatus.ip_address = addressResult.replace("="," ");
    }

    return WifiStatus;
}

QString wifi_bt_interface::get_wifisignalquality()
{
    QString strCmd = QString("iw dev wlan0 link | grep SSID |awk '{print $2}'");
    QString QualityResult = executeLinuxCmd(strCmd);

    QString strResult = QString("wifi signal quality:\n");
    strCmd = QString("nmcli device wifi list|grep '%1'|awk '{print $7}'|sed -n '1p' ").arg(QualityResult.remove("\n"));
    QualityResult = executeLinuxCmd(strCmd);
    //qDebug() << "LINE:" << __LINE__ << "QualityResult"<< QualityResult;

    if(QualityResult == QString("BARS\n"))
    {
        QualityResult = QString("please try again!");
    }

    return strResult.append(QualityResult);
}

QString wifi_bt_interface::get_wifisecurity(QString infoname)
{
    QString strCmd = QString("nmcli connection show %1|grep key-mgmt | awk '{printf $2}'").arg(infoname);
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;
    return strResult;
}


QString wifi_bt_interface::wifi_disconnect()
{
    QString strCmd = QString("nmcli device disconnect wlan0");
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;
    return strResult;
}

/*return exist--true,not exist--false*/
bool wifi_bt_interface::wifi_connect_exist(QString WifiSsid)
{
    QString strCmd = QString("ifconfig wlan0 up");
    QString strResult = executeLinuxCmd(strCmd);

    strCmd = QString("nmcli connection show | grep '%1' | awk '{print $1}'").arg(WifiSsid);
    strResult = executeLinuxCmd(strCmd);

    QString find_str = QString("%1\n").arg(WifiSsid);
    //qDebug() << "--line--: " << __LINE__<<"find_str = " << find_str;
    bool checktResult=strResult.contains(find_str,Qt::CaseInsensitive);

    if(checktResult == 1)
    {
        strResult = "it had connected!";

        return true;
    }

    return false;
}

QString wifi_bt_interface::wifi_activation(QString WifiSsid)
{
    QString strCmd = QString("nmcli connection up '%1' ").arg(WifiSsid);
    //qDebug() << "--line--: " << __LINE__<<"strcmd = " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << "--LINE--: " << __LINE__<< "func:" << __FUNCTION__<< strResult;

    return strResult;
}

QString wifi_bt_interface::wifi_connection_remove(QString WifiSsid)
{
    QString strCmd = QString("nmcli connection delete '%1' ").arg(WifiSsid);
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << "--LINE--: " << __LINE__<< "func:" << __FUNCTION__<< strResult;

    return strResult;
}


QString wifi_bt_interface::wifi_connect(QString WifiSsid,QString PassWd)
{
    QString strCmd;
    QString strResult;
    strCmd = QString("nmcli device wifi connect \"%1\" password \"%2\" name \"%3\" ").arg(WifiSsid).arg(PassWd).arg(WifiSsid);
    strResult = executeLinuxCmd(strCmd);

    bool ConnectResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
    //qDebug() << ConnectResult;

    if(ConnectResult == 1)
    {
        strResult = "Connection successful!";
        //qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;

        return QString(1);
    }
    else
    {
        strResult = "Connection failed!";
        strCmd = QString("nmcli connection delete \"%1\" ").arg(WifiSsid);
        //qDebug() << "--line--: " << __LINE__<< "func:" << __FUNCTION__<< "strCmd == " << strCmd;
        strResult = executeLinuxCmd(strCmd);
        //qDebug() << "--line--: " << __LINE__<< strResult;

        bool ConnectResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);
        //qDebug() << ConnectResult;
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

bool wifi_bt_interface::wifi_modify(QString WifiSsid,QString PassWd)
{
    if(WifiSsid.isEmpty() || PassWd.isEmpty())
    {
        return false;
    }

    QString strCmd = QString("nmcli connection modify \"%1\" wifi-sec.psk \"%2\" ").arg(WifiSsid).arg(PassWd);
    QString strResult = executeLinuxCmd_bluetooth_connect(strCmd);
    strCmd = QString("nmcli connection up %1").arg(WifiSsid);
    strResult = executeLinuxCmd_bluetooth_connect(strCmd);//qDebug() << strResult;
   // strCmd = QString("echo $?");
   // strResult = executeLinuxCmd(strCmd);
    if(strResult.contains("successfully",Qt::CaseSensitive))
    {
        return true;
    }
    return false;
}

QString wifi_bt_interface::hotspot_connect(QString HtWlan,QString HtName,QString HtPasswd)
{
    QString strCmd;
    QString strResult;

    bool wifi_open_flag = get_wifiopenorno();
    if(wifi_open_flag == false)
    {
        wifi_enable(true);
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
    //qDebug() << "--line--: " << __LINE__<< "FILE: " << __FILE__<< "FUNC:" << __FUNCTION__<< strResult;

    bool ConnectResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
    if(ConnectResult == true)
    {
        strResult = "build successful!";
        //qDebug() << "--line--: " << __LINE__<< strResult;
        strResult = QString(1);

        HtName = QString("hotspot%1").arg(HtName);
        //wifi_passwd_change(HtName,HtPasswd);
    }
    else
    {
        strResult = "build failed!";
        //qDebug() << "--line--: " << __LINE__<< strResult;
        strResult = QString("");
    }

    return strResult;
}

bool wifi_bt_interface::hotspot_disconnect()
{
    QString strCmd;
    QString strResult;

    strCmd = QString("nmcli con down hotspot");
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "strResult" << strResult;

    bool ConnectResult=strResult.contains("successfully deactivated",Qt::CaseInsensitive);
    if(ConnectResult == 1)
    {
        strResult = tr("successfully deactivated!");
        //qDebug() << "--line--: " << __LINE__<< strResult;
        return true;
    }
    else
    {
        strResult = tr("deactivated failed!");
        //qDebug() << "--line--: " << __LINE__<< strResult;
        return false;
    }

    /*
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

        bool ConnectResult=strResult.contains("successfully deactivated",Qt::CaseInsensitive);
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
    */
}

QString wifi_bt_interface::sim_disconnect()
{
    QString strCmd = QString("nmcli connection down ppp0");
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;

    bool DisconnectResult=strResult.contains("successfully deactivated",Qt::CaseInsensitive);
    //qDebug() << ConnectResult;

    if(DisconnectResult == true)
    {
        strResult = "disconnect successful!";
        //qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;

        return QString(1);
    }
    else
    {
        strResult = "disconnect failed!";
        //qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;
        return 0;
    }

    return 0;
}

void wifi_bt_interface::sim_activation(int flag)
{
   // QString strCmd = QString("nmcli con show --active |grep ppp0 |wc -l");
   // QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;
   //    if(strResult == "1\n")
//    {
//        return QString(1);
//    }
    if(flag == 1)
    {
        QString strCmd = QString("nmcli connection up ppp0");
        executeLinuxCmd(strCmd);
    }
    else if(flag == 0)
    {
        QString strCmd = QString("nmcli connection down ppp0");
        executeLinuxCmd(strCmd);
    }
}

QString wifi_bt_interface::sim_connect()
{
    QString strCmd;
    QString strResult;

    strCmd = QString("nmcli con show --active |grep ppp0 |wc -l");
    //qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << "--line--: " << __LINE__<< strResult;
    if(strResult == "1\n")
    {
        return QString(1);
    }

    char *state = (char *)"out";
    int port_num = calc_port_num('h',12);

    bool isExist = getFileName(port_num);
    if(isExist == false)
    {
        //qDebug() << "--line--: " << __LINE__<< "func:" << __FUNCTION__<< "not Exist";
        if(gpio_export(port_num) == 0)
        {
            if(gpio_set_state(port_num, state) == 0)
            {
                if(gpio_set_value(port_num, 1) == 0)
                {
                    //qDebug() << "--line--: " << __LINE__<< "func:" << __FUNCTION__<< "build connect";
                    sleep(5);
                }
            }
        }
        else
        {
            gpio_unexport(port_num);
            return 0;
        }
    }

    QFileInfo fileInfo("/etc/NetworkManager/system-connections/ppp0");
    if(fileInfo.isFile() == true)
    {
 //       qDebug()<<"File exists";

//        strCmd = QString("nmcli connection up ppp0");
//        strResult = executeLinuxCmd(strCmd);

//        bool ActivateResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
//        if(ActivateResult == true)
//        {
//            strCmd = QString("nmcli con show --active |grep ppp0 |wc -l");
//            strResult = executeLinuxCmd(strCmd);
//            if(strResult == "1\n")
//            {
//                return QString(1);
//            }
//            else
//            return 0;}
        strCmd = QString("nmcli connection delete ppp0");
        strResult = executeLinuxCmd(strCmd);
    }

//        else
//        {
//            return 0;
//        }
//    }
//    else
//    {
//        qDebug()<<"File not exists";

        strCmd = QString("nmcli connection add con-name ppp0 ifname ttyUSB2 autoconnect yes type gsm apn 3gnet user uninet password uninet");
        //qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
        strResult = executeLinuxCmd(strCmd);
        //qDebug() << "--line--: " << __LINE__<< strResult;

        bool ConnectResult=strResult.contains("successfully added",Qt::CaseInsensitive);
        //qDebug() << ConnectResult;

        if(ConnectResult == 1)
        {
            sleep(1);
            strResult = executeLinuxCmd("ifconfig |grep ppp0 |wc -l");
            if(strResult == "1\n")
            {
                return QString(1);
            }
            else
            {
                return 0;
            }
        }
        else
        {
            strResult = "Connection failed!";
            //qDebug() << "--line--: " << __LINE__<< strResult;

            strCmd = QString("nmcli connection delete ppp0");
            //qDebug() << "--line--: " << __LINE__<< "func:" << __FUNCTION__<< "strCmd == " << strCmd;
            strResult = executeLinuxCmd(strCmd);
            //qDebug() << "--line--: " << __LINE__<< strResult;

            bool ConnectResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);
            //qDebug() << ConnectResult;
            if(ConnectResult == 1)
            {
                strResult = "delete successful!";
                //qDebug() << "--line--: " << __LINE__<< strResult;
            }
            else
            {
                strResult = "delete failed!";
                //qDebug() << "--line--: " << __LINE__<< strResult;
            }

            return 0;
        }
//    }
    return 0;
}

QString wifi_bt_interface::sim_status()
{
    QString strCmd;
    QString strResult;
    QString statusResult;

    strCmd = QString("nmcli con show --active |grep ppp0 |wc -l");
    //qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << "--line--: " << __LINE__<< "FILE" << __FILE__<< strResult;
    if(strResult == "0\n")
    {
        return 0;
    }

    strCmd = QString("nmcli con show ppp0 |grep NAME");strCmd = " "+strCmd;
    statusResult.append(executeLinuxCmd(strCmd).simplified()+"\n");
    strCmd = QString("nmcli con show ppp0 |grep STATE");strCmd = " "+strCmd;
    statusResult.append(executeLinuxCmd(strCmd).simplified()+"\n");
    strCmd = QString("nmcli con show ppp0 |grep VPN");strCmd = " "+strCmd;
    statusResult.append(executeLinuxCmd(strCmd).simplified()+"\n");strCmd = " "+strCmd;
    strCmd = QString("nmcli con show ppp0 |grep autoconnect | sed -n 1p");
    statusResult.append(executeLinuxCmd(strCmd).simplified()+"\n");strCmd = " "+strCmd;
    strCmd = QString("nmcli con show ppp0 |grep ADDRESS |sed -n 1p");
    statusResult.append(executeLinuxCmd(strCmd).simplified()+"\n");strCmd = " "+strCmd;

    //qDebug() << "--line--: " << __LINE__<< statusResult;

    return statusResult;
}

QString wifi_bt_interface::sim_4gstatus()
{
    QString strCmd;
    QString strResult;
    QString statusResult;

    strCmd = QString("mmcli --list-modems");
    strResult = executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        return 0;
    }
    strCmd = QString("mmcli -m %1 |grep revision").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1); //strCmd = "revision:"+strCmd;
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep supported").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section("\n",0,0);strCmd = strCmd.section(":",1,1);
    //strCmd = "hardware supported:"+strCmd;
    strCmd = strCmd+"\n";
    statusResult.append(strCmd);

    strCmd = QString("mmcli -m %1 |grep equipment").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
   // strCmd = "equipment id:"+strCmd;
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep ports").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
   // strCmd = "ports:"+strCmd;
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep state").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    QString str1 = QString("nmcli con show --active |grep ppp0 |wc -l");
    str1 = executeLinuxCmd(str1);
    if(str1 == "0\n")
    {
        strCmd = strCmd.section("\n",0,0);strCmd = strCmd.section(":",1,1);strCmd = strCmd+"\n";
    }
    else
    {
        strCmd = strCmd.section("\n",0,0);strCmd = strCmd.section("m",1,1);strCmd.remove(9,strCmd.size()-1);
        strCmd = " "+strCmd+"\n";
    }
    statusResult.append(strCmd);

    strCmd = QString("mmcli -m %1 |grep power").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep access").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    //strCmd = "access tech:"+strCmd;
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep signal").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep IP").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
   // strCmd = "IP supported:"+strCmd;
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep name").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
   // strCmd = "operator name:"+strCmd;
    statusResult.append(strCmd)+"\n";
    return statusResult;
}

//bool wifi_bt_interface::wifi_passwd_write(QString WifiSsid,QString PassWd)
//{
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "func:" << __FUNCTION__;


//    QFile file("/wifipasswd.txt");
//    if(!file.open(QIODevice::ReadWrite | QIODevice::Append))
//    {
//        return false;
//    }

//    QString text = QString("%1:%2\n").arg(WifiSsid).arg(PassWd);
//    QByteArray WifiInfo;
//    WifiInfo = text.toUtf8();

//    file.write(WifiInfo);
//    file.close();

//    return true;
//}

//QString wifi_bt_interface::wifi_passwd_read(QString WifiSsid)
//{
//    QString PassWd,tmp;

//    QFile file("/wifipasswd.txt");
//    if(!file.open(QIODevice::ReadOnly))
//    {
//        return 0;
//    }

//    while(file.atEnd() == false)
//    {
//        tmp = file.readLine();
//        if(tmp.section(':',0,0) == WifiSsid)
//        {
//            PassWd = tmp.section(':',1,1);
//            return PassWd;
//        }
//    }

//    file.close();

//    return NULL;
//}

//void wifi_bt_interface::wifi_passwd_delete(QString WifiSsid)
//{
//    QString PassWd,tmp;
//    QStringList infolist;
//    int i =0;

//    QFile file("/wifipasswd.txt");
//    if(!file.open(QIODevice::ReadOnly))
//        return;
//    tmp = file.readAll();
//    file.close();
//    infolist = tmp.split("\n");
//    infolist.removeAll(QString(""));

//    for(i = 0; i < infolist.size();i++)
//    {
//        tmp = infolist.at(i);
//        //qDebug() << "LINe:" << __LINE__<< "FILE" << __FILE__<< "tmp" << tmp;

//        if(tmp.section(':',0,0) == WifiSsid)
//        {
//            qDebug() << "LINe:" << __LINE__<< "FILE" << __FILE__;
//            //infolist.removeOne(infolist.at(i));
//            infolist.removeAt(i);
//            break;
//        }
//    }
//    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//       return;
//    file.write("\n");
//    file.close();

//    if(file.open(QIODevice::ReadWrite | QIODevice::Append))
//    {
//        for(i = 0; i < infolist.size(); i++)
//        {
//            tmp = QString("%1\n").arg(infolist.at(i));
//            //qDebug() << "LINe:" << __LINE__<< "FILE" << __FILE__<< "tmp" << tmp;

//            file.write(tmp.toUtf8());
//        }
//    }

//    file.close();
//}

//void wifi_bt_interface::wifi_passwd_change(QString WifiSsid,QString PassWd)
//{
    //wifi_passwd_write(WifiSsid);
   // wifi_passwd_write(WifiSsid,PassWd);
//}

QString wifi_bt_interface::bluetooth_scan()
{
    QString strCmd;
    QString strResult;
    QString str;
    char * data;
    int i = 0;
    if(bluetoothscan_flag == 0)
    {    QString out = executeLinuxCmd("cat /usr/bin/bluetooth_set.sh");
        QStringList list = out.split("\n");
        QStringList BtPairList;

        list.removeAll("");
        for(int i = 0;i<list.size();i++)
        {
             str = QString(list[i]);
            if(str.contains("bluetoothctl",Qt::CaseSensitive))
             {   continue;}

            if(str.contains("hciattach",Qt::CaseSensitive))
            bluetooth_flag = 1;
            BtPairList << str + "\n";
        }

        QFile file("/bt_scan.sh");
        if (file.exists())
        {
            file.remove();
        }
        if(file.open( QIODevice::WriteOnly))
        {
            for(i = 0; i< BtPairList.size();++i)
            {
                QString tmp = BtPairList.at(i);
                data = tmp.toLatin1().data();
                file.write(data);
            }
        }
        file.close();
        strResult = executeLinuxCmd("chmod +x /bt_scan.sh");

        if(bluetooth_flag == 1)
        {
            strCmd = QString("ps -ax |grep 'hciattach -n -s 1500000 /dev/ttyBT0 sprd' |grep -v grep |wc -l");
            strResult = executeLinuxCmd(strCmd);

            if(strResult == "0\n")
            {
                strCmd = QString("/bt_scan.sh");
                strResult = executeLinuxCmd_bluetooth(strCmd);
                //strCmd = QString("killall hciattach");
               // strResult = executeLinuxCmd(strCmd);
            }
            bluetooth_flag++;
        }      
        bluetoothscan_flag++;
        strCmd = QString("rm /bt_scan.sh");
        strResult = executeLinuxCmd(strCmd);
     }
        if(bluetoothscan_flag == 1)
        {
           bluetoothscan_flag++;
        }
        else
        {
          bluetooth_scan_2();
        }
         QStringList list1;
         list1  << "#!/usr/bin/expect -f \n"
                 << "spawn bluetoothctl              \n"
                 << "send \"scan on\\r\"             \n"
                 << "expect \"Discovery started\"    \n"
                 << "sleep 8                         \n"
                 << "send \"scan off\\r\"            \n"
                 << "expect \"Discovery stopped\"    \n"
   //              << "expect $prompt                  \n"
                 << "send \"devices\\r\"            \n"
                 << "expect $prompt                  \n"
   //              << "send \"quit\\r\"                \n"
                 << "expect \"eof\"                  \n";
         QFile file1("/bt_scan2.sh");
         if (file1.exists())
         {
             file1.remove();
         }

         if(file1.open( QIODevice::WriteOnly  ))
         {
             for(int i = 0; i< list1.size();++i)
             {
                 QString tmp = list1.at(i);
                 char * data = tmp.toLatin1().data();
                 file1.write(data);
             }
         }
         file1.close();
         strCmd = QString("chmod +x /bt_scan2.sh");
         executeLinuxCmd(strCmd);

         strCmd = QString("/bt_scan2.sh ");
         strResult = executeLinuxCmd(strCmd);

         QString tmp1 = "devices";
         int n = strResult.indexOf(tmp1);
         strResult = strResult.remove(0,n);

         QStringList list2 = strResult.split("\n");

         int flag =0;
         for(int i = 0;i<list2.size();i++)
         {
             if(flag == 1)
             {
                 i--;
                 flag = 0;
             }
             QString str = list2.at(i);

             if(!str.contains("NEW",Qt::CaseInsensitive))
             {
                 list2.removeAt(i);
                 flag =1;
             }
         }
         QStringList list3;
         QString address;
         QString strname;
         QString tmp2;
         list2.removeAt(0);list2.removeAt(0);
         for(int i=0;i<list2.size();i++)
         {
             tmp1 = list2.at(i);
             tmp1 = tmp1.section('\r', 2, 2);//qDebug() << tmp1;
             address = tmp1.section(' ', 2, 2);
             strname = tmp1.section(' ', 3, 3);
             tmp1.indexOf(address);
             //if(strname.contains("-",Qt::CaseSensitive))
             tmp1 = address.section(':',2,2);
             tmp2 = strname.section('-',2,2);
             if(!QString::compare(tmp2,tmp1,Qt::CaseSensitive))
                 continue;
             list3 << "\t"+address+"\t"+strname;
         }
         list3.removeAll("");
         strResult = list3.join("\n");
         strResult.remove(strResult.size()-2,2);
         qDebug() << strResult;
         return strResult;
}

QString wifi_bt_interface::bluetooth_pair(QString BtAddress)
{
    char * data;
    int i = 0;
    QStringList BtPairList;
    BtPairList << "#!/usr/bin/expect -f \n"
            << "set address [lindex $argv 0]    \n"
            << "set prompt \"#\"                \n"
            << "log_user 1                      \n"
            << "spawn bluetoothctl              \n"
            << "expect $prompt                  \n"
            << "send \"trust $address\\r\"      \n"
//            << "expect \"trust succeeded\"      \n"
            << "expect $prompt                  \n"
            << "send \"pair $address\\r\"       \n"
            << "expect \"*yes/no\"              \n"
            << "send \"yes\\r\"                 \n"
            << "expect \"Pairing successful\"   \n"
            << "expect $prompt                  \n"
            << "send \"quit\\r\"                \n"
            << "expect \"eof\"                  \n";

    QFile file("/bt_pair.sh");
    if (file.exists())
    {
        file.remove();
    }

    if(file.open( QIODevice::WriteOnly  ))
    {
        for(i = 0; i< BtPairList.size();++i)
        {
            QString tmp = BtPairList.at(i);
            data = tmp.toLatin1().data();
            //cout<<data;
            file.write(data);
        }
    }
    file.close();

    QString strCmd = QString("chmod +x /bt_pair.sh");
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;

    BtAddress = BtAddress.replace(QString("\n"), QString(""));
    strCmd = QString("/bt_pair.sh %1").arg(BtAddress);
    strResult = executeLinuxCmd(strCmd);

    bool PairResult=strResult.contains("Pairing successful",Qt::CaseInsensitive);
    //qDebug() << PairResult;

    if(PairResult == 1)  //1 == true
    {
        strResult = "successful";
    }
    else
    {
        strResult = "failed";
    }
    return strResult;
}

QString wifi_bt_interface::bluetooth_connect(QString BtAddress)
{
    QStringList list;
    list << "#!/usr/bin/expect -f \n"
            << "set address [lindex $argv 0]    \n"
            << "set prompt \"#\"                \n"
            << "log_user 1                      \n"
            << "spawn bluetoothctl              \n"
            << "expect $prompt                  \n"
//            << "send \"info $address\\r\"       \n"
//            << "expect {                        \n"
//            << "\"Connected: yes\" { exit; exp_continue } \n"
//            << "\"Connected: no\" { send \"connect $address\\r\" }     \n"
//            << "}                                \n"
            << "send \"connect $address\\r\"    \n"
            << "expect \"Connection successful\"\n"
            << "expect $prompt                  \n"
            << "send \"quit\\r\"                \n"
            << "expect \"eof\"                  \n";

    QFile file("/bt_connect.sh");
    if (file.exists())
    {
        file.remove();
    }

    if(file.open( QIODevice::WriteOnly  ))
    {
        for(int i = 0; i< list.size();++i)
        {
            QString tmp = list.at(i);
            char * data = tmp.toLatin1().data();
            file.write(data);
        }
    }
    file.close();

    QString strCmd = QString("chmod +x /bt_connect.sh");
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;

    BtAddress = BtAddress.replace(QString("\n"), QString(""));
    strCmd = QString("/bt_connect.sh %1").arg(BtAddress);
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;
    QString str = QString("Device %1 Connected: yes").arg(BtAddress);
    bool ConnectResult=strResult.contains(str,Qt::CaseInsensitive);

    if(ConnectResult == 1)
    {
        strResult = "successful";
    }
    else
    {
        strResult = "failed";
    }
   // strCmd = QString("rm /bt_connect.sh");
   // strResult = executeLinuxCmd(strCmd);
    return strResult;
}

void wifi_bt_interface::bluetooth_enable(bool power_flag)
{
    //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "power_flag:" << power_flag;

    QString strCmd;
    QString strResult;

    if(false == power_flag)
    {
        strCmd = QString("hciconfig hci0 down");
        strResult = executeLinuxCmd(strCmd);
    }
    else
    {
        strCmd = QString("hciconfig hci0 up");
        strResult = executeLinuxCmd(strCmd);
    }
}

QString wifi_bt_interface::get_bluetooth_name()
{
    QString strCmd = QString("hciconfig hci0 name |grep Name |awk '{print $2}'");
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;

    strResult.remove(0,1);
    strResult.remove(strResult.length()-2,2);
    //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;

    return strResult;
}

QString wifi_bt_interface::set_bluetooth_name(QString bluetooth_name)
{
    QString strCmd = QString("hciconfig hci0 name %1").arg(bluetooth_name);
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;

    return strResult;
}

QString wifi_bt_interface::scan_wlan()
{
    QString strCmd = QString("ifconfig | grep 'wlan' \n");
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

QString wifi_bt_interface::executeLinuxCmd_bluetooth(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    QThread::sleep(1);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}

bool wifi_bt_interface::hotspot_sql()
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
            //strResult.trimmed();
            if(strResult == "hotspot\n")
                return true;
        }
    }
    return false;
}

QString wifi_bt_interface::executeLinuxCmd_bluetooth_connect(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}

int wifi_bt_interface::bluetooth_connectflag()
{
    QStringList list;
    list << "#!/usr/bin/expect -f \n"
         << "spawn bluetoothctl              \n"
         << "send \"info \\r\"       \n"
         << "send \"quit\\r\"                \n"
         << "expect \"eof\"                  \n";

    QFile file("/bt_connectflag.sh");
    if (file.exists())
    {
        file.remove();
    }

    if(file.open( QIODevice::WriteOnly  ))
    {
        for(int i = 0; i< list.size();++i)
        {
            QString tmp = list.at(i);
            char * data = tmp.toLatin1().data();
            file.write(data);
        }
    }
    file.close();
    QString strCmd = QString("chmod +x /bt_connectflag.sh");
    executeLinuxCmd(strCmd);

    strCmd = QString("/bt_connectflag.sh");
    QString strResult = executeLinuxCmd(strCmd);
    QString str = QString("Missing device address argument");
    if(strResult.contains(str,Qt::CaseInsensitive))
    {
       return 1;
    }
    else
    {
       return 0;
    }
}

void wifi_bt_interface::bluetooth_disconnect(QString address)
{
    QStringList list;
    list << "#!/usr/bin/expect -f \n"
         << "set address [lindex $argv 0]    \n"
         << "set prompt \"#\"                \n"
         << "log_user 1                      \n"
         << "spawn bluetoothctl              \n"
         << "send \"disconnect $address\\r\" \n"
         << "send \"quit\\r\"                \n"
         << "expect \"eof\"                  \n";

    QFile file("/bt_disconnect.sh");
    if (file.exists())
    {
        file.remove();
    }

    if(file.open( QIODevice::WriteOnly  ))
    {
        for(int i = 0; i< list.size();++i)
        {
            QString tmp = list.at(i);
            char * data = tmp.toLatin1().data();
            file.write(data);
        }
    }
    file.close();
    QString strCmd = QString("chmod +x /bt_disconnect.sh");
    executeLinuxCmd(strCmd);

    strCmd = QString("/bt_disconnect.sh %1").arg(address);
    executeLinuxCmd(strCmd);
    strCmd = QString("rm /bt_disconnect.sh");
    executeLinuxCmd(strCmd);
   // qDebug() << strResult;

}

void wifi_bt_interface::bluetooth_remove(QString address)
{
    QStringList list;
    list << "#!/usr/bin/expect -f \n"
         << "set address [lindex $argv 0]    \n"
         << "set prompt \"#\"                \n"
         << "log_user 1                      \n"
         << "spawn bluetoothctl              \n"
         << "expect $prompt                  \n"
         << "send \"remove $address\\r\" \n"
         << "send \"quit\\r\"                \n"
         << "expect \"eof\"                  \n";

    QFile file("/bt_remove.sh");
    if (file.exists())
    {
        file.remove();
    }

    if(file.open( QIODevice::WriteOnly  ))
    {
        for(int i = 0; i< list.size();++i)
        {
            QString tmp = list.at(i);
            char * data = tmp.toLatin1().data();
            file.write(data);
        }
    }
    file.close();
    QString strCmd = QString("chmod +x /bt_remove.sh");
    executeLinuxCmd(strCmd);

    strCmd = QString("/bt_remove.sh %1").arg(address);
    executeLinuxCmd(strCmd);
    strCmd = QString("rm /bt_remove.sh");
    executeLinuxCmd(strCmd);
}

void wifi_bt_interface::bluetooth_sh_delete()
{
    QString strCmd = QString("rm /bt_connectflag.sh");
    executeLinuxCmd(strCmd);
}

void wifi_bt_interface::bluetooth_scan_2()
{

    if(bluetoothscan_flag == 2)
    {
        QStringList list;
        list << "#!/usr/bin/expect -f \n"
             << "spawn bluetoothctl              \n"
             << "send \"paired-devices     \\r\" \n"
             << "send \"quit\\r\"                \n"
             << "expect \"eof\"                  \n";

        QFile file("/bt_scan1.sh");
        if (file.exists())
        {
            file.remove();
        }

        if(file.open( QIODevice::WriteOnly  ))
        {
            for(int i = 0; i< list.size();++i)
            {
                QString tmp = list.at(i);
                char * data = tmp.toLatin1().data();
                file.write(data);
            }
        }
        file.close();
        QString strCmd = QString("chmod +x /bt_scan1.sh");
        executeLinuxCmd(strCmd);
        bluetoothscan_flag++;
    }
    QString strCmd = QString("/bt_scan1.sh ");
    QString str = executeLinuxCmd(strCmd);
    QString str1 = str;
    QString p = "HelperA133";
    int n = str1.indexOf(p);
    str1 = str1.remove(0,n);

    p = "Agent registered";
    n = str1.indexOf(p);
    str1 = str1.remove(n,str1.size()-1);
    QStringList list1 = str1.split("\r");list1.removeAt(0);list1.removeAt(list1.size()-1);list1.removeAll(QString(""));
    QStringList list2;
    for(int i = 0;i<list1.size();i++)
    {
        QString strname = list1.at(i);
        QString address = strname.section(' ', 2, 2);
        strname = strname.section(' ', 3, 3);
        list2 << address;
        list2 << strname;
    }
    list2.removeAll(QString(""));

   p = "Agent registered";
   n = str.indexOf(p);
   str = str.remove(0,n);

   p = "quit";
   n = str.indexOf(p);
   str = str.remove(n,str.size()-0);
   QStringList list3 = str.split("\r");list3.removeAt(0);list3.removeAt(0);
   list3.removeAt(list3.size()-1);list3.removeAll(QString(""));
   QStringList list4;
   for(int i = 0;i<list3.size();i++)
   {
       QString strname = list3.at(i);
       QString address = strname.section(' ', 1, 1);
       strname = strname.section(' ', 2, 2);
       list4 << address;
       list4 << strname;
   }
   list4.removeAll(QString(""));
  // qDebug() << list2;
   qDebug() << list4;//qDebug() << s;

    for(int i = 0;i<list2.size();i++)
    {
        QString list2_str = list2.at(i);
        for(int j = 0;j<list4.size();j++)
        {
            QString list4_str = list4.at(j);
            if(!QString::compare(list2_str,list4_str,Qt::CaseSensitive))
            {
               break;
            }
            else
            {
                if(j == (list4.size()-2))
                {
                    bluetooth_remove(list2_str);//qDebug() << list2.at(i+1);
                }
            }
            j++;
        }
        i++;
    }
}

void wifi_bt_interface::rm_bluetoothshell()
{
    QString strCmd = QString("rm /bt_connect.sh");
    executeLinuxCmd(strCmd);
    strCmd = QString("rm /bt_pair.sh");
    executeLinuxCmd(strCmd);
}
