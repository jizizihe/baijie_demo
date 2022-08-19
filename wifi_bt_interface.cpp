#include "wifi_bt_interface.h"
static int bluetooth_flag;
static int bluetooth_func;

wifi_bt_interface::wifi_bt_interface(QObject *parent) : QObject(parent)
{

}
wifi_bt_interface::~wifi_bt_interface()
{

}


QString wifi_bt_interface::executeLinuxCmd(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
   // if(bluetooth_func == 1)
   // {
   //     QThread::sleep(1);qDebug() << 11;
   // }
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

//    QString strCmd = QString("iw dev wlan0 scan|grep SSID|awk '{for(i=2;i<=NF;i++){printf \"%s \", $i}; printf \"\\n\"}' ");
    QString strCmd = QString("nmcli -t  device wifi list|awk -F : '{print $2\":\"$7}' ");
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
        //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "strResult:" << tmp;
        findString = tmp.section(':', 1, 1);
        if(findString == wifi_name)
        {
            index = i;break;
            //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "index:" << index;
        }
    }

    if(index == -1)
    {
        qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "not find this name";
        return WifiStatus;
    }

    tmp = scanlist.at(index);
    //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "tmp" << tmp;

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
        //qDebug() << "LINE:" << __LINE__ << "statusResult"<< addressResult;
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
    qDebug() << "LINE:" << __LINE__ << "QualityResult"<< QualityResult;

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
    qDebug() << strResult;
    return strResult;
}


QString wifi_bt_interface::wifi_disconnect()
{
    QString strCmd = QString("nmcli device disconnect wlan0");
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;
    return strResult;
}

/*return exist--true,not exist--false*/
bool wifi_bt_interface::wifi_connect_exist(QString WifiSsid)
{
    QString strCmd = QString("ifconfig wlan0 up");
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << "--line--: " << __LINE__<< strResult;

    strCmd = QString("nmcli connection show | grep '%1' | awk '{print $1}'").arg(WifiSsid);
    qDebug() << "--line--: " << __LINE__<< strCmd;
    strResult = executeLinuxCmd(strCmd);
    qDebug() << "--line--: " << __LINE__<< strResult;

    QString find_str = QString("%1\n").arg(WifiSsid);
    qDebug() << "--line--: " << __LINE__<<"find_str = " << find_str;
    bool checktResult=strResult.contains(find_str,Qt::CaseInsensitive);
    qDebug() << "--line--: " << __LINE__<< checktResult;

    if(checktResult == 1)
    {
        strResult = "it had connected!";
        qDebug() << "--line--: " << __LINE__<< strResult;

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

    strCmd = QString("nmcli device wifi connect \"%1\" password \"%2\" name \"%3\" ").arg(WifiSsid).arg(PassWd).arg(WifiSsid);
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

        strCmd = QString("nmcli connection delete \"%1\" ").arg(WifiSsid);
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
    if(WifiSsid.isEmpty() || PassWd.isEmpty())
    {
        return false;
    }

    QString strCmd = QString("nmcli connection modify \"%1\" wifi-sec.psk \"%2\" ").arg(WifiSsid).arg(PassWd);
    qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << "--line--: " << __LINE__<< strResult;


    strCmd = QString("echo $?");
    qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    qDebug() << "--line--: " << __LINE__<< strResult;

    if(strResult == "0\n")
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
    qDebug() << "--line--: " << __LINE__<< "FILE: " << __FILE__<< "FUNC:" << __FUNCTION__<< strResult;

    bool ConnectResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
    if(ConnectResult == true)
    {
        strResult = "build successful!";
        qDebug() << "--line--: " << __LINE__<< strResult;
        strResult = QString(1);

        HtName = QString("hotspot%1").arg(HtName);
        wifi_passwd_change(HtName,HtPasswd);
    }
    else
    {
        strResult = "build failed!";
        qDebug() << "--line--: " << __LINE__<< strResult;
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
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "strResult" << strResult;

    bool ConnectResult=strResult.contains("successfully deactivated",Qt::CaseInsensitive);
    if(ConnectResult == 1)
    {
        strResult = tr("successfully deactivated.!");
        qDebug() << "--line--: " << __LINE__<< strResult;
        return true;
    }
    else
    {
        strResult = tr("deactivated failed!");
        qDebug() << "--line--: " << __LINE__<< strResult;
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
    qDebug() << strResult;

    bool DisconnectResult=strResult.contains("successfully deactivated",Qt::CaseInsensitive);
    //qDebug() << ConnectResult;

    if(DisconnectResult == true)
    {
        strResult = "disconnect successful!";
        qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;

        return QString(1);
    }
    else
    {
        strResult = "disconnect failed!";
        qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;
        return 0;
    }

    return 0;
}

QString wifi_bt_interface::sim_activation()
{
    QString strCmd = QString("nmcli con show --active |grep ppp0 |wc -l");
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;
    if(strResult == "1\n")
    {
        return QString(1);
    }

    strCmd = QString("nmcli connection up ppp0");
    strResult = executeLinuxCmd(strCmd);
    qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;

    bool ActivateResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
    if(ActivateResult == true)
    {
        return QString(1);
    }

    return 0;
}

QString wifi_bt_interface::sim_connect()
{
    QString strCmd;
    QString strResult;

    strCmd = QString("nmcli con show --active |grep ppp0 |wc -l");
    //qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    qDebug() << "--line--: " << __LINE__<< strResult;
    if(strResult == "1\n")
    {
        return QString(1);
    }

    char *state = (char *)"out";
    int port_num = calc_port_num('h',12);
    qDebug() << "--line--: " << __LINE__<< "func:" << __FUNCTION__<< "port_num == " << port_num;

    bool isExist = getFileName(port_num);
    if(isExist == false)
    {
        qDebug() << "--line--: " << __LINE__<< "func:" << __FUNCTION__<< "not Exist";
        if(gpio_export(port_num) == 0)
        {
            if(gpio_set_state(port_num, state) == 0)
            {
                if(gpio_set_value(port_num, 1) == 0)
                {
                    qDebug() << "--line--: " << __LINE__<< "func:" << __FUNCTION__<< "build connect";
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
        qDebug()<<"File exists";

        strCmd = QString("nmcli connection up ppp0");
        strResult = executeLinuxCmd(strCmd);
        qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;

        bool ActivateResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
        if(ActivateResult == true)
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
        qDebug()<<"File not exists";

        strCmd = QString("nmcli connection add con-name ppp0 ifname ttyUSB2 autoconnect yes type gsm apn 3gnet user uninet password uninet");
        //qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
        strResult = executeLinuxCmd(strCmd);
        qDebug() << "--line--: " << __LINE__<< strResult;

        bool ConnectResult=strResult.contains("successfully added",Qt::CaseInsensitive);
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

            strCmd = QString("nmcli connection delete ppp0");
            //qDebug() << "--line--: " << __LINE__<< "func:" << __FUNCTION__<< "strCmd == " << strCmd;
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
    }
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
    qDebug() << "--line--: " << __LINE__<< "FILE" << __FILE__<< strResult;
    if(strResult == "0\n")
    {
        return 0;
    }

    strCmd = QString("nmcli con show ppp0 |grep NAME");
    statusResult.append(executeLinuxCmd(strCmd).simplified()+"\n");
    strCmd = QString("nmcli con show ppp0 |grep STATE");
    statusResult.append(executeLinuxCmd(strCmd).simplified()+"\n");
    strCmd = QString("nmcli con show ppp0 |grep VPN");
    statusResult.append(executeLinuxCmd(strCmd).simplified()+"\n");
    strCmd = QString("nmcli con show ppp0 |grep autoconnect | sed -n 1p");
    statusResult.append(executeLinuxCmd(strCmd).simplified()+"\n");
    strCmd = QString("nmcli con show ppp0 |grep ADDRESS |sed -n 1p");
    statusResult.append(executeLinuxCmd(strCmd).simplified()+"\n");

    qDebug() << "--line--: " << __LINE__<< statusResult;

    return statusResult;
}

bool wifi_bt_interface::wifi_passwd_write(QString WifiSsid,QString PassWd)
{
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "func:" << __FUNCTION__;


    QFile file("/wifipasswd.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        return false;
    }

    QString text = QString("%1:%2\n").arg(WifiSsid).arg(PassWd);
    QByteArray WifiInfo;
    WifiInfo = text.toUtf8();

    file.write(WifiInfo);
    file.close();

    return true;
}

QString wifi_bt_interface::wifi_passwd_read(QString WifiSsid)
{
    QString PassWd,tmp;

    QFile file("/wifipasswd.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        return 0;
    }

    while(file.atEnd() == false)
    {
        tmp = file.readLine();
        if(tmp.section(':',0,0) == WifiSsid)
        {
            PassWd = tmp.section(':',1,1);
            return PassWd;
        }
    }

    file.close();

    return NULL;
}

void wifi_bt_interface::wifi_passwd_delete(QString WifiSsid)
{
    QString PassWd,tmp;
    QStringList infolist;
    int i =0;

    QFile file("/wifipasswd.txt");
    if(!file.open(QIODevice::ReadOnly))
        return;
    tmp = file.readAll();
    file.close();
    infolist = tmp.split("\n");
    infolist.removeAll(QString(""));

    for(i = 0; i < infolist.size();i++)
    {
        tmp = infolist.at(i);
        //qDebug() << "LINe:" << __LINE__<< "FILE" << __FILE__<< "tmp" << tmp;

        if(tmp.section(':',0,0) == WifiSsid)
        {
            qDebug() << "LINe:" << __LINE__<< "FILE" << __FILE__;
            //infolist.removeOne(infolist.at(i));
            infolist.removeAt(i);
            break;
        }
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
       return;
    file.write("\n");
    file.close();

    if(file.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        for(i = 0; i < infolist.size(); i++)
        {
            tmp = QString("%1\n").arg(infolist.at(i));
            //qDebug() << "LINe:" << __LINE__<< "FILE" << __FILE__<< "tmp" << tmp;

            file.write(tmp.toUtf8());
        }
    }

    file.close();
}

void wifi_bt_interface::wifi_passwd_change(QString WifiSsid,QString PassWd)
{
    wifi_passwd_delete(WifiSsid);
    wifi_passwd_write(WifiSsid,PassWd);
}

QString wifi_bt_interface::bluetooth_scan()
{
    QString strCmd;
    QString strResult;

    char * data;
    int i = 0;
        QString out = executeLinuxCmd("cat /usr/bin/bluetooth_set.sh");
        QStringList list = out.split("\n");
        QStringList BtPairList;

        list.removeAll("");
        for(int i = 0;i<list.size();i++)
        {
            QString str = QString(list[i]);
            if(str.contains("bluetoothctl",Qt::CaseSensitive))
                continue;
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
        executeLinuxCmd("chmod +x /bt_scan.sh");
        if(bluetooth_flag == 1)
        {
            strCmd = QString("killall hciattach");
            strResult = executeLinuxCmd(strCmd);
        }
        strCmd = QString("/bt_scan.sh");
        strResult = executeLinuxCmd_bluetooth(strCmd);
        bluetooth_flag=0;

    strCmd = QString("hcitool scan | sed \"1d\" ");
    strResult = executeLinuxCmd(strCmd);
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
            << "send \"remove $address\\r\"     \n"
            << "expect $prompt                  \n"
            << "send \"scan on\\r\"             \n"
            << "expect \"Discovery started\"    \n"
            << "sleep 10                        \n"
            << "send \"scan off\\r\"            \n"
            << "expect \"Discovery stopped\"    \n"
            << "expect $prompt                  \n"
            << "send \"trust $address\\r\"      \n"
            << "expect \"trust succeeded\"      \n"
            << "expect $prompt                  \n"
            << "send \"pair $address\\r\"       \n"
            << "expect \"*yes/no\"              \n"
            << "send \"yes\\r\"                 \n"
            << "expect \"Pairing successful\"   \n"
            << "expect \"Device $address Connected: no\"   \n"
//            << "expect $prompt                  \n"
//            << "send \"connect $address\\r\"    \n"
//            << "expect \"Connection successful\"\n"
            << "expect $prompt                  \n"
            << "send \"quit\\r\"                \n"
            << "expect \"eof\"                  \n";

    //qDebug()<< "list ="<< list;

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
            //qDebug()<<"tmp ="<< tmp;
            data = tmp.toLatin1().data();
            //cout<<data;
            file.write(data);
        }
    }
    file.close();

    QString strCmd = QString("chmod +x /bt_pair.sh");
    qDebug() << "strCmd == " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;

    BtAddress = BtAddress.replace(QString("\n"), QString(""));
    strCmd = QString("/bt_pair.sh %1").arg(BtAddress);
    qDebug() << "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;

    bool PairResult=strResult.contains("Pairing successful",Qt::CaseInsensitive);
    qDebug() << PairResult;

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
            << "send \"connect $address\\r\"    \n"
            << "expect \"Connection successful\"\n"
            << "expect $prompt                  \n"
            << "send \"quit\\r\"                \n"
            << "expect \"eof\"                  \n";

    //qDebug()<< "list ="<< list;

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
            //qDebug()<<"tmp ="<< tmp;
            char * data = tmp.toLatin1().data();
            //cout<<data;
            file.write(data);
        }
    }
    file.close();

    QString strCmd = QString("chmod +x /bt_connect.sh");
    qDebug() << "strCmd == " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;

    BtAddress = BtAddress.replace(QString("\n"), QString(""));
    strCmd = QString("/bt_connect.sh %1").arg(BtAddress);
    qDebug() << "strCmd == " << strCmd;

    strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;

    bool ConnectResult=strResult.contains("Connection successful",Qt::CaseInsensitive);
    qDebug() << ConnectResult;

    if(ConnectResult == 1)
    {
        strResult = "successful";
    }
    else
    {
        strResult = "failed";
    }

    return strResult;
}

void wifi_bt_interface::bluetooth_enable(bool power_flag)
{
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "power_flag:" << power_flag;

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
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;

    strResult.remove(0,1);
    strResult.remove(strResult.length()-2,2);
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;

    return strResult;
}

QString wifi_bt_interface::set_bluetooth_name(QString bluetooth_name)
{
    QString strCmd = QString("hciconfig hci0 name %1").arg(bluetooth_name);
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;

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
