#include "ipset_interface.h"

bool automatically_get_ip()
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

bool add_static_ip(QString netname,QString ipaddr)
{
    QString gateway;

    for(int i = 9;i < 12;i++)
    {
        if(ipaddr.at(i) == '.')
        {
            gateway = ipaddr.left(i+1);
            gateway.append("1");
            break;
        }
    }
    QString strCmd = QString("nmcli con add con-name static_ip ifname %1 autoconnect yes type ethernet ip4 %2 gw4 %3") \
            .arg(netname).arg(ipaddr).arg(gateway);
    QString strResult = executeLinuxCmd(strCmd);

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

bool modify_static_ip(QString ipaddr)
{
    QString gateway;
    for(int i = 9;i < 12;i++)
    {
        if(ipaddr.at(i) == '.')
        {
            gateway = ipaddr.left(i+1);
            gateway.append("1");
            break;
        }
    }
    QString strCmd = QString("nmcli con mod static_ip ipv4.address %2,%3").arg(ipaddr).arg(gateway);
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

bool delete_static_ip()
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

bool is_static_ip_exist()
{
    QString isExistCmd = QString("nmcli con show |grep static_ip");
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

QString get_network_info()
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

QString get_current_ip(QString Name)
{
    QList<QNetworkInterface> network=QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface net,network)
    {
        QString netName=net.humanReadableName();
        if(netName==Name)
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

void network_enable(bool flag)
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

