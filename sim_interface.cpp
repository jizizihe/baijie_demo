#include "sim_interface.h"

sim_interface::sim_interface(QObject *parent) : QObject(parent)
{

}
sim_interface::~sim_interface()
{

}

QString sim_interface::executeLinuxCmd(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}

QString sim_interface::simDisconnect()
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

void sim_interface::setSimEnable(int flag)
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

QString sim_interface::simConnect()
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
            if(setGpioState(port_num, state) == 0)
            {
                if(setGpioValue(port_num, 1) == 0)
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

QString sim_interface::getSimStatus()
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

QString sim_interface::getSimModuleStatus()
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

