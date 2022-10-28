#include "ipset_interface.h"

bool automaticallyGetIp()
{
    QProcess cmd;
    cmd.start("dhclient eth0");
    if(!cmd.waitForFinished())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool addStaticIp(QString netName,QString ipAddr,QString mask,QString gate)
{
    QString strCmd = QString("nmcli con delete static_ip");
    QString strResult = executeLinuxCmd(strCmd);
    strCmd = QString("nmcli con add con-name static_ip ifname %1 autoconnect yes type ethernet ip4 %2/%3 gw4 %4") \
            .arg(netName).arg(ipAddr).arg(mask).arg(gate);
    strResult = executeLinuxCmd(strCmd);

    bool result=strResult.contains("successfully added",Qt::CaseInsensitive);
    if(result == true)
    {
        strCmd = QString("nmcli connection up static_ip");
        strResult = executeLinuxCmd(strCmd);
        bool result=strResult.contains("successfully activated",Qt::CaseInsensitive);
        if(result == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool modifyStaticIp(QString ipAddr)
{
    QString gateway;
    for(int i = 9;i < 12;i++)
    {
        if(ipAddr.at(i) == '.')
        {
            gateway = ipAddr.left(i+1);
            gateway.append("1");
            break;
        }
    }
    QString strCmd = QString("nmcli con mod static_ip ipv4.address %2,%3").arg(ipAddr).arg(gateway);
    QString strResult = executeLinuxCmd(strCmd);

    strCmd = QString("nmcli connection up static_ip");
    strResult = executeLinuxCmd(strCmd);
    bool result=strResult.contains("successfully activated",Qt::CaseInsensitive);
    if(result == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool deleteStaticIp()
{
    QString strCmd = QString("nmcli con del static_ip");
    QString strResult = executeLinuxCmd(strCmd);

    bool result=strResult.contains("successfully deleted",Qt::CaseInsensitive);
    if(result == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isStaticIpExist()
{
    QString isExistCmd = QString("nmcli con show |grep static");
    QString retResult = executeLinuxCmd(isExistCmd);

    if(true == retResult.isEmpty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

QString getNetworkInfo()
{
    QString tmp;
    QString strCmd = QString("ifconfig eth0");
    QString retResult = executeLinuxCmd(strCmd);
    QStringList list;
    list = retResult.split("\n");
    for(int i = 0;i < list.count();i++)
    {
        retResult = list.at(i);
        tmp.append(retResult.trimmed());
        tmp.append("\n");
    }
    return tmp;
}

QString getCurrentIp(QString name)
{
    QList<QNetworkInterface> network=QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface net,network)
    {
        QString netName=net.humanReadableName();
        if(netName==name)
        {
            QList<QNetworkAddressEntry> list=net.addressEntries();
            foreach(QNetworkAddressEntry address,list)
            {
                if(address.ip().protocol()==QAbstractSocket::IPv4Protocol)
                {
                    return address.ip().toString();
                }
            }
        }
    }
    return 0;
}

void networkEnable(bool flag)
{
    QString strCmd;
    QString retResult;

    if(flag == true)
    {
        strCmd = QString("ifconfig eth0 up");
        retResult = executeLinuxCmd(strCmd);
    }
    else
    {
        strCmd = QString("ifconfig eth0 down");
        retResult = executeLinuxCmd(strCmd);
    }
}
