#include "ipset_interface.h"
#include <QDebug>


long int getCurrentDownloadRates(long int * save_rate)
{
    FILE * net_dev_file;
    char buffer[1024];
    size_t bytes_read;
    char * match;
    if ( (net_dev_file=fopen("/proc/net/dev", "r")) == NULL )
    {
        printf("open file /proc/net/dev/ error!\n");
        return -1;
    }
    bytes_read = fread(buffer, 1, sizeof(buffer), net_dev_file);
    fclose(net_dev_file);
    if ( bytes_read == 0 )
    {
        return -1;
    }
    buffer[bytes_read] = '\0';
    match = strstr(buffer, "eth0:");
    if ( match == NULL )
    {
        printf("no eth0 keyword to find!\n");
        return -1;
    }
    sscanf(match, "eth0:%ld", save_rate);
    return *save_rate;
}

float getspeed()
{
    long int start_download_rates;
    long int end_download_rates;
    float speed,var;
    if(getCurrentDownloadRates(&start_download_rates) == -1)
    {
        return -1;
    }
    sleep(WAIT_SECOND);
    if(getCurrentDownloadRates(&end_download_rates) == -1)
    {
        return -1;
    }
    speed = (float)(end_download_rates-start_download_rates)/WAIT_SECOND;

    var = (int)(speed*10+0.5);
    speed = var/10.0;

    return speed;
}

QString getipconfig()
{
    QProcess cmd ;
    cmd.start("ifconfig");
    cmd.waitForFinished();
    return QString::fromLocal8Bit(cmd.readAllStandardOutput().data());
}

QString getallip()
{
    QProcess cmd ;
    cmd.start("nmcli con show");
    cmd.waitForFinished();
    return QString::fromLocal8Bit(cmd.readAllStandardOutput().data());
}

QString setstaticip(QString ipname,QString netcard,QString ipaddr,QString gateway)
{
    QString cmd_addstaticip = QString("nmcli con add con-name %1 ifname %2 autoconnect yes type ethernet ip4 %3 \
                              gw4 %4").arg(ipname).arg(netcard).arg(ipaddr).arg(gateway);
    QProcess cmd_addsip;
    cmd_addsip.start(cmd_addstaticip);
    if(!cmd_addsip.waitForFinished())
    {
        return "Failed to set static IP address!";
    }
    cmd_addsip.start(QString("nmcli con up %1").arg(netcard));
    if(!cmd_addsip.waitForFinished())
    {
        return "Failed to set static IP address!";
    }
    return QString::fromLocal8Bit(cmd_addsip.readAllStandardOutput().data());
}

QString setsip(QString ipname,QString netcard,QString ipaddr,QString gateway)
{
    QString cmd_addstaticip = QString("nmcli con add con-name %1 ifname %2 autoconnect yes type ethernet ip4 \
                        %3 gw4 %4").arg(ipname).arg(netcard).arg(ipaddr).arg(gateway);
    QProcess cmd_addsip;
    cmd_addsip.start(cmd_addstaticip);
    cmd_addsip.waitForFinished();
    cmd_addsip.start(QString("nmcli con up %1").arg(ipname));
    cmd_addsip.waitForFinished();
    return QString::fromLocal8Bit(cmd_addsip.readAllStandardOutput().data());

}

QString addstaticip(QString ipname,QString netcard,QString ipaddr,QString gateway,bool flag)
{
    QString cmd_addstaticip = QString("nmcli con add con-name %1 ifname %2 autoconnect yes type ethernet ip4 %3 gw4 %4") \
            .arg(ipname).arg(netcard).arg(ipaddr).arg(gateway);
    QProcess cmd_addsip;
    cmd_addsip.start(cmd_addstaticip);
    if(!cmd_addsip.waitForFinished())
    {
        return "Failed to add static IP address!";
    }
    if(flag)
    {
        QString cmd_ipup = QString("nmcli connection up %1").arg(ipname);
        cmd_addsip.start(cmd_ipup);

        if(!cmd_addsip.waitForFinished())
        {
            return "Failed to add static IP address!";
        }
    }
    return QString::fromLocal8Bit( cmd_addsip.readAllStandardOutput().data());
}

QString modstaticip(QString ipname,QString ipaddr,QString gateway,bool flag)
{
    QString cmd_modstaticip = QString("nmcli con mod %1 ipv4.address %2,%3").arg(ipname).arg(ipaddr).arg(gateway);

    QProcess cmd_modsip;
    cmd_modsip.start(cmd_modstaticip);
    if(!cmd_modsip.waitForFinished())
    {
        return "Failed to modify IP address!";
    }
    if(flag)
    {
        QString cmd_ipup = QString("nmcli con up %1").arg(ipname);
        cmd_modsip.start(cmd_ipup);
        if(!cmd_modsip.waitForFinished())
        {
            return "Failed to modify IP address!";
        }
    }
    return QString::fromLocal8Bit(cmd_modsip.readAllStandardOutput().data());
}

QString delstaticip(QString ipname)
{
    QString cmd_delstaticip = QString("nmcli con del %1").arg(ipname);
    QProcess cmd_delsip;
    cmd_delsip.start(cmd_delstaticip);
    if(!cmd_delsip.waitForFinished())
    {
        return "Failed to delete IP address!";
    }
    return QString::fromLocal8Bit( cmd_delsip.readAllStandardOutput().data());
}

QString setdip(QString netcard)
{
    QString cmd_delstaticip = QString("dhclient %1").arg(netcard);
    QProcess cmd_delsip;
    cmd_delsip.start(cmd_delstaticip);
    if(!cmd_delsip.waitForFinished())
    {
        return "Failed to set dymanic IP!";
    }
    if(netcard.isEmpty())
    {
        return "";
    }
    return "Setting to dynamic IP successfully";
}
