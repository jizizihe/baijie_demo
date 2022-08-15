#include "all_test_interface.h"

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

QString usb_test(int num)
{
    QString strCmd = QString("lsusb| wc -l");
    QString strResult = executeLinuxCmd(strCmd);
//    qDebug() << "LINE:" << __LINE__ << "__FILE__" << __FILE__ << "strResult"<< strResult;

    strResult = strResult.remove("\n");
    int count = 3 + num;

    if(strResult == QString::number(count))
    {
        strResult = "OK";
    }
    else
    {
        strResult = "failed";
    }
    return strResult;
}

QString get_new_sd()
{
    QString strCmd = QString("fdisk -l | grep /dev/mmcblk[1-9] | wc -l");
    QString strResult = executeLinuxCmd(strCmd);
//    qDebug() << "LINE:" << __LINE__ << "__FILE__" << __FILE__ << "strResult"<< strResult;
    if(strResult == QString("0\n"))
    {
        strResult = "failed";
    }
    else
    {
        strResult = "OK";
    }
    return strResult;
}

QString rtc_test()
{
    QString strCmd;
    QString strResult;

    strCmd = QString("`expr substr `hwclock` 12 8` ");
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << "LINE:" << __LINE__ << "__FILE__" << __FILE__ << "strResult"<< strResult;

    return strResult;
}

void camera_test()
{
    QString strCmd = QString("rm /data/yuv.jpg");
    QString strResult = executeLinuxCmd(strCmd);

    strCmd = QString("cd /data && csi_test_mplane");
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << "LINE:" << __LINE__ << "__FILE__" << __FILE__ << "strResult"<< strResult;
}

QString battary_test()
{
    QString strCmd = QString("cat /sys/class/power_supply/axp803-battery/voltage_now");
    QString strResult = executeLinuxCmd(strCmd);
//    qDebug() << "LINE:" << __LINE__ << "__FILE__" << __FILE__ << "strResult"<< strResult;

    if(strResult == QString("0\n"))
    {
        strResult = "failed";
    }
    else
    {
        strResult = "OK";
    }
    return strResult;
}

QString sim_test()
{
    QString strCmd;
    QString strResult;

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
        strCmd = QString("ifconfig | grep ppp0");
        strResult = executeLinuxCmd(strCmd);
        qDebug() << "LINE:" << __LINE__ << "__FILE__" << __FILE__ << "strResult"<< strResult;

        if(!strResult.isEmpty())
        {
            strResult = "OK";
            return strResult;
        }
        else
        {
            strCmd = QString("nmcli connection up ppp0");
            strResult = executeLinuxCmd(strCmd);

            bool ActivateResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
            if(ActivateResult == true)
            {
                strResult = "OK";
            }
            else
            {
                strResult = "failed";
            }
            return strResult;
        }
    }
    else
    {
        strCmd = QString("nmcli connection add con-name ppp0 ifname ttyUSB2 autoconnect yes type gsm apn 3gnet user uninet password uninet");
        //qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
        strResult = executeLinuxCmd(strCmd);
        qDebug() << "--line--: " << __LINE__<< strResult;

        bool ConnectResult=strResult.contains("successfully added",Qt::CaseInsensitive);
        //qDebug() << ConnectResult;

        if(ConnectResult == true)
        {
            strResult = "Connection successful!";
//            qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;
            sleep(4);
            strCmd = QString("ifconfig | grep ppp0");
            strResult = executeLinuxCmd(strCmd);
            qDebug() << "LINE:" << __LINE__ << "__FILE__" << __FILE__ << "strResult"<< strResult;

            if(!strResult.isEmpty())
            {
                strResult = "OK";
                return strResult;
            }

            strResult = "failed";
            return strResult;
        }
        else
        {
            strResult = "Connection failed!";
            qDebug() << "--line--: " << __LINE__<< strResult;
            strCmd = QString("nmcli connection delete ppp0");
            //qDebug() << "--line--: " << __LINE__<< "func:" << __FUNCTION__<< "strCmd == " << strCmd;
            strResult = executeLinuxCmd(strCmd);
            qDebug() << "--line--: " << __LINE__<< strResult;
            strResult = "failed";
            return strResult;
        }
    }
    return strResult;
}

QString audio_test()
{
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ ;

    QString strCmd = QString("rm /data/audio.wav");
    QString strResult = executeLinuxCmd(strCmd);

    strCmd = QString("arecord -d 2 -r 16000 -c 1 -t wav /data/audio.wav");
    strResult = executeLinuxCmd(strCmd);

    strCmd = QString("aplay /usr/helperboard/ceshiluyin.wav");
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ ;
    strResult = executeLinuxCmd(strCmd);

    strCmd = QString("aplay /data/audio.wav");
    strResult = executeLinuxCmd(strCmd);
    return NULL;
}

QString wifi_test()
{
    QString strCmd = QString("nmcli device wifi rescan");
    QString strResult = executeLinuxCmd(strCmd);

    strCmd = QString("nmcli -t  device wifi list|wc -l");
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "ScanResult:" << strResult;

    if(strResult == QString("0\n"))
    {
        strResult = "failed";
    }
    else
    {
        strResult = "OK";
    }
    return strResult;
}

QString bluetooth_test()
{
    QString strCmd;
    QString strResult;
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__;

    strCmd = QString("hciconfig |grep hci0 | wc -l");
    strResult = executeLinuxCmd(strCmd);
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "ScanResult:" << strResult;

    if(strResult == "0\n")
    {
        strCmd = QString("rfkill unblock all");
        strResult = executeLinuxCmd(strCmd);

        strCmd = QString("ps -ax |grep 'hciattach -n -s 1500000 /dev/ttyBT0 sprd' |grep -v grep |wc -l");
        strResult = executeLinuxCmd(strCmd);

        if(strResult == "0\n")
        {
            strCmd = QString("hciattach -n -s 1500000 /dev/ttyBT0 sprd 1>/dev/null 2>/dev/null &");
            strResult = executeLinuxCmd(strCmd);
            //qDebug() << strResult;
            QThread::sleep(2);
        }
    }
    strCmd = QString("hciconfig hci0 up && hciconfig hci0 piscan");
    strResult = executeLinuxCmd(strCmd);

    strCmd = QString("hcitool scan | wc -l");
    strResult = executeLinuxCmd(strCmd);
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "ScanResult:" << strResult;

    if(strResult == QString("0\n"))
    {
        strResult = "failed";
    }
    else
    {
        strResult = "OK";
    }

    return strResult;
}


QString executeLinuxCmd(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}
