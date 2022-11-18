#include "wifi_bt_interface.h"

static int btHciattachFlag;
static int bluetoothScanFlag;

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
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}

bool wifi_bt_interface::wifiOpenFlag()
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

QString wifi_bt_interface::wifiEnableFlag(bool flag)
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

QString wifi_bt_interface::wifiScan()
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

wifi_info wifi_bt_interface::getWifiStatus(QString wifiName)
{
    int index = -1;
    QString tmp;
    QString findString,addressResult;

    WifiStatus.active.clear();
    WifiStatus.name.clear();
    WifiStatus.passwd.clear();
    WifiStatus.secrity.clear();
    WifiStatus.signal.clear();
    WifiStatus.ipAddress.clear();

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
        return WifiStatus;
    }

    tmp = scanlist.at(index);
    WifiStatus.active = tmp.section(':', 0, 0);
    WifiStatus.name = wifiName;
    WifiStatus.passwd = databaseWg.selectTableName(QString("wifiPasswd"),wifiName);
    WifiStatus.secrity = tmp.section(':', 3, 3);
    WifiStatus.signal = tmp.section(':', 2, 2);

    if(QString("yes") == WifiStatus.active)
    {
        strCmd = QString("wpa_cli -i wlan0 status | grep ip_address|cut -d '=' -f 2");
        addressResult = executeLinuxCmd(strCmd);
        addressResult.replace("\n","");
        WifiStatus.ipAddress = addressResult.replace("="," ");
    }
    return WifiStatus;
}

QString wifi_bt_interface::getWifiSignalQuality()
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

QString wifi_bt_interface::getWifiSecurity(QString infoName)
{
    QString strCmd = QString("nmcli connection show %1|grep key-mgmt | awk '{printf $2}'").arg(infoName);
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}


QString wifi_bt_interface::wifiDisconnect()
{
    QString strCmd = QString("nmcli device disconnect wlan0");
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

/*return exist--true,not exist--false*/
bool wifi_bt_interface::wifiConnectExistFlag(QString WifiSsid)
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

QString wifi_bt_interface::wifiActivation(QString WifiSsid)
{
    QString strCmd = QString("nmcli connection up '%1' ").arg(WifiSsid);
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

QString wifi_bt_interface::wifiConnectionRemove(QString WifiSsid)
{
    QString strCmd = QString("nmcli connection delete '%1' ").arg(WifiSsid);
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

QString wifi_bt_interface::wifiConnect(QString WifiSsid,QString PassWd)
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

bool wifi_bt_interface::wifiModifyPasswd(QString WifiSsid,QString PassWd)
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

QString wifi_bt_interface::hotspotConnect(QString HtWlan,QString HtName,QString HtPasswd)
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

bool wifi_bt_interface::hotspotDisconnect()
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

QString wifi_bt_interface::simDisconnect()
{
    QString strCmd = QString("nmcli connection down ppp0");
    QString strResult = executeLinuxCmd(strCmd);
    bool DisconnectResult=strResult.contains("successfully deactivated",Qt::CaseInsensitive);

    if(DisconnectResult == true)
    {
        strResult = "disconnect successful!";
        return QString(1);
    }
    else
    {
        strResult = "disconnect failed!";
        return 0;
    }
    return 0;
}

void wifi_bt_interface::simActivation(int flag)
{
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

QString wifi_bt_interface::simConnect()
{
    QString strCmd;
    QString strResult;

    strCmd = QString("nmcli con show --active |grep ppp0 |wc -l");
    strResult = executeLinuxCmd(strCmd);
    if(strResult == "1\n")
    {
        return QString(1);
    }

    char *state = (char *)"out";
    int port_num = calcPortNum('h',12);

    bool isExist = getFileName(port_num);
    if(isExist == false)
    {
        if(gpioExport(port_num) == 0)
        {
            if(gpioSetState(port_num, state) == 0)
            {
                if(gpioSetValue(port_num, 1) == 0)
                {
                    sleep(5);
                }
            }
        }
        else
        {
            gpioUnexport(port_num);
            return 0;
        }
    }

    QFileInfo fileInfo("/etc/NetworkManager/system-connections/ppp0");
    if(fileInfo.isFile() == true)
    {
        strCmd = QString("nmcli connection delete ppp0");
        strResult = executeLinuxCmd(strCmd);
    }

    strCmd = QString("nmcli connection add con-name ppp0 ifname ttyUSB2 autoconnect yes type gsm apn 3gnet user uninet password uninet");
    strResult = executeLinuxCmd(strCmd);
    bool ConnectResult=strResult.contains("successfully added",Qt::CaseInsensitive);

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
        strCmd = QString("nmcli connection delete ppp0");
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

QString wifi_bt_interface::simStatus()
{
    QString strCmd;
    QString strResult;
    QString statusResult;

    strCmd = QString("nmcli con show --active |grep ppp0 |wc -l");
    strResult = executeLinuxCmd(strCmd);
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

    return statusResult;
}

QString wifi_bt_interface::simModuleStatus()
{
    QString strCmd;
    QString strResult;
    QString statusResult;

    strCmd = QString("mmcli --list-modems");    //Check whether there are modules
    strResult = executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        return "";
    }
    strCmd = QString("mmcli -m %1 |grep revision").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep supported").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section("\n",0,0);strCmd = strCmd.section(":",1,1);
    strCmd = strCmd+"\n";
    statusResult.append(strCmd);

    strCmd = QString("mmcli -m %1 |grep equipment").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep ports").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep state").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    if(strCmd.contains("failed",Qt::CaseInsensitive))
    {
        strCmd = strCmd.section("\n",0,0);strCmd = strCmd.section("m",1,1);
        strCmd = strCmd.remove(6,strCmd.size()-1);
        strCmd = " "+strCmd+"\n";
    }
    else
    {
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
    }
    statusResult.append(strCmd);

    strCmd = QString("mmcli -m %1 |grep power").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep access").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    if(strCmd.isEmpty())
    {
        strCmd = QString(" %1 \n").arg("no");
        statusResult.append(strCmd);
    }
    else
    {
        statusResult.append(strCmd)+"\n";
    }
    strCmd = QString("mmcli -m %1 |grep signal").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep IP").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    statusResult.append(strCmd)+"\n";

    strCmd = QString("mmcli -m %1 |grep name").arg(str);
    strCmd = executeLinuxCmd(strCmd);
    strCmd = strCmd.section(":",1,1);
    if(strCmd.isEmpty())
    {
        strCmd = QString(" %1 \n").arg("no");
        statusResult.append(strCmd);
    }
    else
    {
        statusResult.append(strCmd)+"\n";
    }
    return statusResult;
}

QString wifi_bt_interface::bluetoothScan()
{
    QString strCmd;
    QString strResult;
    QString str;
    char * data;
    int i = 0;
    if(bluetoothScanFlag == 0)       //  function: Initializing Bluetooth
    {
        QString out = executeLinuxCmd("cat /usr/bin/bluetooth_set.sh");
        QStringList list = out.split("\n");
        QStringList BtScanList;

        list.removeAll("");
        for(int i = 0;i<list.size();i++)
        {
            str = QString(list[i]);
            if(str.contains("bluetoothctl",Qt::CaseSensitive))
            {   continue;}

            if(str.contains("hciattach",Qt::CaseSensitive))
                btHciattachFlag = 1;
            BtScanList << str + "\n";
        }

        QFile file("/bt_scan.sh");
        if (file.exists())
        {
            file.remove();
        }
        if(file.open( QIODevice::WriteOnly))
        {
            for(i = 0; i< BtScanList.size();++i)
            {
                QString tmp = BtScanList.at(i);
                data = tmp.toLatin1().data();
                file.write(data);
            }
        }
        file.close();
        strResult = executeLinuxCmd("chmod +x /bt_scan.sh");

        if(btHciattachFlag == 1)
        {
            strCmd = QString("ps -ax |grep 'hciattach -n -s 1500000 /dev/ttyBT0 sprd' |grep -v grep |wc -l");
            strResult = executeLinuxCmd(strCmd);

            if(strResult == "0\n")
            {
                strCmd = QString("/bt_scan.sh");
                strResult = executeLinuxCmdBluetoothOpen(strCmd);
            }
            btHciattachFlag++;
        }
        bluetoothScanFlag++;
        strCmd = QString("rm /bt_scan.sh");
        strResult = executeLinuxCmd(strCmd);
    }
    if(bluetoothScanFlag == 1)
    {
        bluetoothScanFlag++;
    }
    else
    {
        bluetoothScan2();    //function: Delete the scan devices but not the paired devices
    }
    QStringList list1;
    list1  << "#!/usr/bin/expect -f \n"
           << "spawn bluetoothctl              \n"
           << "send \"scan on\\r\"             \n"
           << "expect \"Discovery started\"    \n"
           << "sleep 9                         \n"
           << "send \"scan off\\r\"            \n"
           << "expect \"Discovery stopped\"    \n"
           << "send \"devices\\r\"            \n"
           << "expect $prompt                  \n"
           << "expect \"eof\"                  \n";
    QFile file1("/bt_scan2.sh");         //rescan
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
    strCmd = QString("/bt_scan2.sh ");             //function: Scan devices
    strResult = executeLinuxCmd(strCmd);
    QString strFlag = "devices";
    int strFlagLocate = strResult.indexOf(strFlag);               //Locate the location of the "device" in strResult
    strResult = strResult.remove(0,strFlagLocate);
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
        strFlag = list2.at(i);
        strFlagLocate =strFlag.indexOf("Device"); strFlag = strFlag.remove(0,strFlagLocate);
        address = strFlag.section(' ', 1, 1);
        strFlagLocate = strFlag.indexOf(address);
        strname = strFlag;
        strname = strname.remove(0,strFlagLocate);strname = strname.remove(0,address.size()+1);
        strname = strname.remove(strname.size()-1,1);
        strFlag = address.section(':',2,2);
        tmp2 = strname.section('-',2,2);
        if(!QString::compare(tmp2,strFlag,Qt::CaseSensitive))
            continue;
        list3 << "\t"+address+"\t"+strname;
    }
    list3.removeAll("");
    strResult = list3.join("\n");
    qDebug() << strResult;
    return strResult;
}

QString wifi_bt_interface::bluetoothPair(QString BtAddress)
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
            file.write(data);
        }
    }
    file.close();

    QString strCmd = QString("chmod +x /bt_pair.sh");
    QString strResult = executeLinuxCmd(strCmd);

    BtAddress = BtAddress.replace(QString("\n"), QString(""));
    strCmd = QString("/bt_pair.sh %1").arg(BtAddress);
    strResult = executeLinuxCmd(strCmd);

    bool PairResult=strResult.contains("Pairing successful",Qt::CaseInsensitive);

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

QString wifi_bt_interface::bluetoothConnect(QString BtAddress)
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
    QString strResult = executeLinuxCmdBluetoothConnect(strCmd);
    BtAddress = BtAddress.replace(QString("\n"), QString(""));
    strCmd = QString("/bt_connect.sh %1").arg(BtAddress);
    strResult = executeLinuxCmdBluetoothConnect(strCmd);
    QString flag = bluetoothConnectFlag();   //Check for Bluetooth connection

    if(flag == "1")
    {
        strResult = "failed";
    }
    else
    {
        strResult = "successful";
    }
    return strResult;
}

void wifi_bt_interface::bluetoothEnable(bool enableFlag)
{
    QString strCmd;
    QString strResult;

    if(false == enableFlag)
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

QString wifi_bt_interface::getBluetoothName()
{
    QString strCmd = QString("hciconfig hci0 name |grep Name |awk '{print $2}'");
    QString strResult = executeLinuxCmd(strCmd);

    strResult.remove(0,1);
    strResult.remove(strResult.length()-2,2);

    return strResult;
}

QString wifi_bt_interface::setBluetoothName(QString bluetoothName)
{
    QString strCmd = QString("hciconfig hci0 name %1").arg(bluetoothName);
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

QString wifi_bt_interface::scanWlan()
{
    QString strCmd = QString("ifconfig | grep 'wlan' \n");
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

QString wifi_bt_interface::executeLinuxCmdBluetoothOpen(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    QThread::sleep(1);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}

bool wifi_bt_interface::hotspotConnectFlag()
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

QString wifi_bt_interface::executeLinuxCmdBluetoothConnect(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}

QString wifi_bt_interface::bluetoothConnectFlag()
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
        return "1";
    }
    else
    {
        QString strFlag = "Agent registered";
        int strFlagLocate = strResult.indexOf(strFlag);
        strResult = strResult.remove(0,strFlagLocate);
        QStringList s = strResult.split("\n");
        str = s.at(3); str = str.section(":",1,1);
        str.remove("\r");str.remove(0,1);
        return str;
    }
}

void wifi_bt_interface::bluetoothDisconnect(QString address)
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

}

void wifi_bt_interface::bluetoothRemove(QString address)
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

void wifi_bt_interface::bluetoothShellDelete()
{
    QString strCmd = QString("rm /bt_connectflag.sh");
    executeLinuxCmd(strCmd);
}

void wifi_bt_interface::bluetoothScan2()
{
    if(bluetoothScanFlag == 2)
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
        bluetoothScanFlag++;
    }
    QString strCmd = QString("/bt_scan1.sh ");
    QString strResult = executeLinuxCmd(strCmd);
    QString str = strResult;
    QString strFlag = "HelperA133";
    int strFalgLocate = str.indexOf(strFlag);
    str = str.remove(0,strFalgLocate);

    strFlag = "Agent registered";
    strFalgLocate = str.indexOf(strFlag);
    str = str.remove(strFalgLocate,str.size()-1);
    QStringList listResult = str.split("\r");                      //Store Bluetooth scans existing devices data
    listResult.removeAt(0);listResult.removeAt(listResult.size()-1);listResult.removeAll(QString(""));
    QStringList listResultDevice;                                  //Store the devices in listResult
    for(int i = 0;i<listResult.size();i++)
    {
        QString strName = listResult.at(i);
        QString address = strName.section(' ', 2, 2);
        strName = strName.section(' ', 3, 3);
        listResultDevice << address;
        listResultDevice << strName;
    }
    listResultDevice.removeAll(QString(""));

    strFlag = "Agent registered";
    strFalgLocate = strResult.indexOf(strFlag);
    strResult = strResult.remove(0,strFalgLocate);

    strFlag = "quit";
    strFalgLocate = strResult.indexOf(strFlag);
    strResult = strResult.remove(strFalgLocate,strResult.size()-0);
    QStringList listResultPair = strResult.split("\r");  //Storage Bluetooth paired existing devices data
    listResultPair.removeAt(0);listResultPair.removeAt(0);
    listResultPair.removeAt(listResultPair.size()-1);listResultPair.removeAll(QString(""));
    QStringList listResultPairDevice;             //Store the devices in listResultPair
    for(int i = 0;i<listResultPair.size();i++)
    {
        QString strName = listResultPair.at(i);
        QString address = strName.section(' ', 1, 1);
        strName = strName.section(' ', 2, 2);
        listResultPairDevice << address;
        listResultPairDevice << strName;
    }
    listResultPairDevice.removeAll(QString(""));

    qDebug() << listResultPairDevice;

    for(int i = 0;i<listResultDevice.size();i++)
    {
        QString strScanDevice = listResultDevice.at(i);
        for(int j = 0;j<listResultPairDevice.size();j++)
        {
            QString strPairDevice = listResultPairDevice.at(j);
            if(!QString::compare(strScanDevice,strPairDevice,Qt::CaseSensitive))      //Delete the scan devices but not the paired devices
            {
                break;
            }
            else
            {
                if(j == (listResultPairDevice.size()-2))
                {
                    bluetoothRemove(strScanDevice);
                }
            }
            j++;
        }
        i++;
    }
}

void wifi_bt_interface::rm_bluetooth_shell()
{
    QString strCmd = QString("rm /bt_connect.sh");
    executeLinuxCmd(strCmd);
    strCmd = QString("rm /bt_pair.sh");
    executeLinuxCmd(strCmd);
}
