#include "udev_interface.h"

//#define DD
#ifdef DD
#define qdebug(format, ...)  qDebug("function:%s\tline:%s\t " format, __func__,__LINE__,##__VA_ARGS__);
#else
#define qdebug(format, ...)
#endif


QString get_gateway()
{
    QProcess *proc = new QProcess();
    proc->start("bash",QStringList() << "-c" << "route -n | awk '{print $2}'");
    proc->waitForFinished(-1);

    QString str = proc->readAllStandardOutput();
    QStringList route = str.split("\n");

    for(int i = 0; i < route.size(); i++)
    {
        if(route.at(i) == "Gateway" && route.at(i+1) != "0.0.0.0")
        {
            str = route.at(i+1);
            return str;
        }
    }
    return NULL;

}

QString ping_gateway()
{
    QString gateway = get_gateway();
    QString dex = "1 received, 0% packet loss";
    QString ping_str = QString("ping -c 1 %1 | grep '%2'").arg(gateway).arg(dex);

    QProcess proc;
    proc.start("bash",QStringList() << "-c" << ping_str);
    proc.waitForFinished();

    if(proc.readAllStandardOutput() != NULL)
        return "OK";

    return "Failed";
}

QString get_new_usb()
{
    bool flag = false;
    QString failed = QObject::tr("USB Failed");

    QProcess *proc = new QProcess();
    QString str = QString("lsusb | awk '{print $6}'");
    proc->start("bash",QStringList() << "-c" << str);
    proc->waitForFinished();
    QString temp = proc->readAllStandardOutput();

    QStringList usb_data = temp.split("\n");
    usb_data.removeAll("");

    for(int x = 0; x < usb_data.size(); x++)
    {
        temp = usb_data.at(x);
        temp = temp.right(4);
        if(temp == "0001" || temp == "0125" || temp == "0101" || temp == "0002" || temp == "772b")
        {
            continue;
        }
        else
        {
            flag = true;
            break;
        }
    }
    proc->close();
    delete proc;
    proc = 0;

    if(flag)
    {
        qdebug("*****usb******true : %s",temp.data());
        return temp;
    }
    else
    {
        qdebug("*****usb******flase: %s",failed.data());
        return failed;
    }

}

QString get_new_sd()
{
    QString failed = QObject::tr("SD Failed");
    QString sd_str = QString("fdisk -l | grep /dev/mmcblk1p1");
    QProcess sd_proc;
    sd_proc.start("bash",QStringList() << "-c" << sd_str);
    sd_proc.waitForFinished();
    QString status = sd_proc.readAllStandardOutput().data();
    if(status == NULL)
    {
        qdebug("*****sd****** false: %s",failed.data());
        return failed;
    }
    else
    {
        qdebug("*****sd****** true: %s",status.data());
        return status;
    }


}

QString get_new_sim()
{
    QString failed = QObject::tr("SIM Failed");
    QString sim_str = QString("ifconfig | grep ppp0");
    QProcess sim_proc;
    sim_proc.start("bash",QStringList() << "-c" << sim_str);
    sim_proc.waitForFinished();
    QString status = sim_proc.readAllStandardOutput().data();
    if(status == NULL)
    {
        qdebug("*****sim****** false: %s",failed.data());
        return failed;
    }
    else
    {
        qdebug("*****sim****** true: %s",status.data());
        return status;
    }
}

QString wifi_test()
{
    QString failed = QObject::tr("WIFI Failed");
    QString wifi_str = QString("iw dev wlan0 scan | grep SSID");
    QProcess wifi_proc;
    wifi_proc.start("bash",QStringList() << "-c" << wifi_str);
    wifi_proc.waitForFinished();
    QString status = wifi_proc.readAllStandardOutput().data();
    if(status == NULL)
    {
        qdebug("*****wifi****** false: %s",failed.data());
        return failed;
    }
    else
    {
        qdebug("*****wifi****** true: %s",status.data());
        return "OK";
    }
}

QString bluetooth()
{

}
