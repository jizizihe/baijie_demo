#include "bluetooth_interface.h"
static int btHciattachFlag;     //0:the Bluetooth dose not uses hciattach  1:the Bluetooth uses hciattach
static int bluetoothScanFlag;

bluetooth_interface::bluetooth_interface(QObject *parent) : QObject(parent)
{

}

bluetooth_interface::~bluetooth_interface()
{

}

QString bluetooth_interface::executeLinuxCmd(QString strCmd)
{
    QProcess pro;
    pro.start("bash", QStringList() <<"-c" << strCmd);
    pro.waitForFinished(-1);
    QString strResult = pro.readAllStandardOutput();
    pro.close();
    return strResult;
}

QString bluetooth_interface::bluetoothScan()
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
    if(bluetoothScanFlag == 1)   //Devices do not need to be deleted the first scan
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

QString bluetooth_interface::bluetoothPair(QString btAddress)
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

    btAddress = btAddress.replace(QString("\n"), QString(""));
    strCmd = QString("/bt_pair.sh %1").arg(btAddress);
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

QString bluetooth_interface::bluetoothConnect(QString btAddress)
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
    btAddress = btAddress.replace(QString("\n"), QString(""));
    strCmd = QString("/bt_connect.sh %1").arg(btAddress);
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

void bluetooth_interface::bluetoothShellDelete()
{
    QString strCmd = QString("rm /bt_connectflag.sh");
    executeLinuxCmd(strCmd);
}

void bluetooth_interface::bluetoothScan2()
{
    if(bluetoothScanFlag == 2)      //Writing script for the first time
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

void bluetooth_interface::rm_bluetooth_shell()
{
    QString strCmd = QString("rm /bt_connect.sh");
    executeLinuxCmd(strCmd);
    strCmd = QString("rm /bt_pair.sh");
    executeLinuxCmd(strCmd);
}

void bluetooth_interface::bluetoothRemove(QString btAddress)
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

    strCmd = QString("/bt_remove.sh %1").arg(btAddress);
    executeLinuxCmd(strCmd);
    strCmd = QString("rm /bt_remove.sh");
    executeLinuxCmd(strCmd);
}

QString bluetooth_interface::bluetoothConnectFlag()
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

void bluetooth_interface::bluetoothDisconnect(QString btAddress)
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

    strCmd = QString("/bt_disconnect.sh %1").arg(btAddress);
    executeLinuxCmd(strCmd);
    strCmd = QString("rm /bt_disconnect.sh");
    executeLinuxCmd(strCmd);
}

QString bluetooth_interface::setBluetoothName(QString bluetoothName)
{
    QString strCmd = QString("hciconfig hci0 name %1").arg(bluetoothName);
    QString strResult = executeLinuxCmd(strCmd);
    return strResult;
}

QString bluetooth_interface::getBluetoothName()
{
    QString strCmd = QString("hciconfig hci0 name |grep Name |awk '{print $2}'");
    QString strResult = executeLinuxCmd(strCmd);

    strResult.remove(0,1);
    strResult.remove(strResult.length()-2,2);

    return strResult;
}

void bluetooth_interface::bluetoothEnableFlag(bool flag)
{
    QString strCmd;
    QString strResult;

    if(false == flag)
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

QString bluetooth_interface::executeLinuxCmdBluetoothConnect(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}

QString bluetooth_interface::executeLinuxCmdBluetoothOpen(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    QThread::sleep(1);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}
