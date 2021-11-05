#include "ipset_interface.h"


QString networkup()
{
    QProcess cmd ;
    cmd.start("dhclient eth0");
    if(!cmd.waitForFinished())
    {
        return "Failed to pull up the network.";
    }
    else
    {
        return "Pulling up the network successfully!";
    }
}


QString addstaticip(QString netcard,QString ipaddr)
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

    QString cmd_addstaticip = QString("nmcli con add con-name static_ip ifname %1 autoconnect yes type ethernet ip4 %2 gw4 %3") \
            .arg(netcard).arg(ipaddr).arg(gateway);
    QProcess cmd_addsip;
    cmd_addsip.start(cmd_addstaticip);
    if(!cmd_addsip.waitForFinished())
    {
        return "Failed to add static IP address!";
    }

    QString cmd_ipup = QString("nmcli connection up static_ip");
    cmd_addsip.start(cmd_ipup);

    if(!cmd_addsip.waitForFinished())
    {
        return "Failed to set static IP address!";
    }

    return QString::fromLocal8Bit( cmd_addsip.readAllStandardOutput().data());
}

QString modstaticip(QString ipaddr)
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
    QString cmd_modstaticip = QString("nmcli con mod static_ip ipv4.address %2,%3").arg(ipaddr).arg(gateway);

    QProcess cmd_modsip;
    cmd_modsip.start(cmd_modstaticip);
    if(!cmd_modsip.waitForFinished())
    {
        return "Failed to modify IP address!";
    }

    QString cmd_ipup = QString("nmcli con up static_ip");
    cmd_modsip.start(cmd_ipup);
    if(!cmd_modsip.waitForFinished())
    {
        return "Failed to modify IP address!";
    }

    return QString::fromLocal8Bit(cmd_modsip.readAllStandardOutput().data());
}

QString delstaticip()
{
    QString cmd_delstaticip = QString("nmcli con del static_ip");
    QProcess cmd_delsip;
    cmd_delsip.start(cmd_delstaticip);
    if(!cmd_delsip.waitForFinished())
    {
        return "Failed to delete IP address!";
    }
    return QString::fromLocal8Bit( cmd_delsip.readAllStandardOutput().data());
}

QString is_staticip()
{
    QString cmd_isstaticip = QString("nmcli con show static_ip");
    QProcess cmd_issip;
    cmd_issip.start(cmd_isstaticip);
    cmd_issip.waitForFinished();
    return QString::fromLocal8Bit( cmd_issip.readAllStandardOutput().data());
}

QString getifconfig() //Obtain an IP connection that can change the address
{
    QProcess getip;
    getip.start("ifconfig");
    getip.waitForFinished();
    QString str = getip.readAllStandardOutput();

    QStringList list,strlist;
    list = str.split("\n");
    for(int i = 0;i < list.count();i++)
    {
        str = list.at(i);
        strlist = str.split(":");
        if(strlist.at(0) == "lo" || strlist.at(0) == "ppp0")
        {
            for(int j = 0;j < 10;j++)
            {
                if(list.at(i) == "")
                {
                    break;
                }
                list.removeAt(i);
            }
        }
    }

    for(int i = 0;i < list.count();i++)
    {
        for (int k = i + 1; k <  list.count(); k++)
        {
            if ( list.at(i) ==  list.at(k))
            {
                list.removeAt(k);
                k--;
            }
        }
    }

    str = list.join("\n");
    return str;
}

QString GetCurrentIp(QString Name)
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
