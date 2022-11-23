#include "all_test_interface.h"

static int g_cameraFirst;

QString getGateway()
{
    QProcess *proc = new QProcess();
    proc->start("bash",QStringList() << "-c" << "route -n | awk '{print $2}'");
    proc->waitForFinished(-1);

    QString str = proc->readAllStandardOutput();
    QStringList route = str.split("\n");
    proc->close();
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

QString pingGateway()
{
    QString gateway = getGateway();
    QString dex = "1 received, 0% packet loss";
    QString pingStr = QString("ping -c 1 %1 | grep '%2'").arg(gateway).arg(dex);

    QProcess proc;
    proc.start("bash",QStringList() << "-c" << pingStr);
    proc.waitForFinished();
    QString strResult = proc.readAllStandardOutput();
    proc.close();
    if( strResult!= NULL)
        return "OK";
    return "Failed";
}

QString usbTest(int num)
{
    QString strCmd = QString("lsusb| wc -l");
    QString strResult = executeLinuxCmd(strCmd);

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

QString getSdCard()
{
    QString strCmd = QString("fdisk -l | grep /dev/mmcblk[1-9] | wc -l");
    QString strResult = executeLinuxCmd(strCmd);
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

QString rtcTest()
{
    QString strCmd;
    QString strResult;
    strCmd = QString("`expr substr `hwclock` 12 8` ");
    strResult = executeLinuxCmd(strCmd);
    return strResult;
}

void cameraTest()
{
    QString strCmd;
    QString strResult;
    if(g_cameraFirst == 0)
    {
        strResult = executeLinuxCmd("mkdir -p /mnt/sdcard");
        g_cameraFirst++;
    }
    strCmd = QString("cd /data && csi_test_mplane");
    strResult = executeLinuxCmd(strCmd);
}

QString battaryTest()
{
    QString strCmd = QString("cat /sys/class/power_supply/axp803-battery/voltage_now");
    QString strResult = executeLinuxCmd(strCmd);
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

QString simTest()
{
    QString strCmd;
    QString strResult;
    char *state = (char *)"out";
    int portNum = calcPortNum('h',12);
    bool isExist = getFileName(portNum);
    if(isExist == false)
    {
        if(gpioExport(portNum) == 0)
        {
            if(setGpioState(portNum, state) == 0)
            {
                if(setGpioValue(portNum, 1) == 0)
                {
                    sleep(5);
                }
            }
        }
        else
        {
            gpioUnexport(portNum);
            return 0;
        }
    }

    QFileInfo fileInfo("/etc/NetworkManager/system-connections/ppp0");
    if(fileInfo.isFile() == true)
    {
        strCmd = QString("ifconfig | grep ppp0");
        strResult = executeLinuxCmd(strCmd);
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
        strResult = executeLinuxCmd(strCmd);
        bool connectResult=strResult.contains("successfully added",Qt::CaseInsensitive);
        if(connectResult == true)
        {
            strResult = "Connection successful!";
            sleep(4);
            strCmd = QString("ifconfig | grep ppp0");
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
            strCmd = QString("nmcli connection delete ppp0");
            strResult = executeLinuxCmd(strCmd);
            strResult = "failed";
            return strResult;
        }
    }
    return strResult;
}

void audioTest()
{
    QString strCmd = QString("rm /data/audio.wav");
    QString strResult = executeLinuxCmd(strCmd);
    strCmd = QString("arecord -d 2 -r 16000 -c 1 -t wav /data/audio.wav");
    strResult = executeLinuxCmd(strCmd);
    strCmd = QString("aplay /data/audio.wav");
    strResult = executeLinuxCmd(strCmd);
}

QString wifiTest()
{
    QString strCmd = QString("nmcli device wifi rescan");
    QString strResult = executeLinuxCmd(strCmd);
    strCmd = QString("nmcli -t  device wifi list|wc -l");
    strResult = executeLinuxCmd(strCmd);
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

QString bluetoothTest()
{
    QString strCmd;
    QString strResult;
    strCmd = QString("hciconfig |grep hci0 | wc -l");
    strResult = executeLinuxCmd(strCmd);
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
            QThread::sleep(2);
        }
    }
    strCmd = QString("hciconfig hci0 up && hciconfig hci0 piscan");
    strResult = executeLinuxCmd(strCmd);
    strCmd = QString("hcitool scan | wc -l");
    strResult = executeLinuxCmd(strCmd);
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
    QProcess pro;
    pro.start("bash", QStringList() <<"-c" << strCmd);
    pro.waitForFinished(-1);
    QString strResult = pro.readAllStandardOutput();
    pro.close();
    return strResult;
}
