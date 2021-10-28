#include "bluetooth_thread.h"

bluetooth_thread::bluetooth_thread(QObject *parent) : QThread(parent)
{
}


void bluetooth_thread::run()
{
    qDebug() << "start bluetooth_thread";

        if(scan_flag == true)
        {
            QString scan_result = bluetooth_scan();
            emit setText(true);
            emit message(scan_result);
        }
        else if(pair_flag == true)
        {
            QString pair_result = bluetooth_pair(BtAddress);
            emit message(pair_result);
        }
        else if(connect_flag == true)
        {
            QString connect_result = bluetooth_connect(BtAddress);
            emit message(connect_result);
        }
        else
        {
            emit setText(false);

        }

}

void bluetooth_thread::flag_set(int signal_type,QString address)
{
    switch(signal_type)
    {
    case 1:
        scan_flag = true;
        pair_flag = false;
        connect_flag = false;
        break;
    case 2:
        scan_flag = false;
        pair_flag = true;
        connect_flag = false;
        BtAddress = address;
        break;
    case 3:
        scan_flag = false;
        pair_flag = false;
        connect_flag = true;
        BtAddress = address;
        break;
    default:
        scan_flag = false;
        pair_flag = false;
        connect_flag = false;
        break;
    }
}

QString bluetooth_thread::bluetooth_scan()
{
    QString strCmd = QString("rfkill unblock all");
    qDebug() << "strCmd == " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;

    strCmd = QString("killall hciattach");
    qDebug() << "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;
    QThread::msleep(100);

    strCmd = QString("hciattach -n -s 1500000 /dev/ttyBT0 sprd 1>/dev/null 2>/dev/null &");
    qDebug() << "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;
    QThread::sleep(2);

    strCmd = QString("hciconfig hci0 up && hciconfig hci0 piscan");
    qDebug() << "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;
    QThread::sleep(1);

    strCmd = QString("hcitool scan > /bluetooth_scan");
    qDebug() << "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;

    strCmd = QString("cat /bluetooth_scan");
    qDebug() << "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;

    return strResult;
}

QString bluetooth_thread::bluetooth_pair(QString BtAddress)
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
            << "send \"remove $address\\r\"     \n"
            << "expect $prompt                  \n"
            << "send \"scan on\\r\"             \n"
            << "expect \"Discovery started\"    \n"
            << "sleep 10                        \n"
            << "send \"scan off\\r\"            \n"
            << "expect \"Discovery stopped\"    \n"
            << "expect $prompt                  \n"
            << "send \"trust $address\\r\"      \n"
            << "expect \"trust succeeded\"      \n"
            << "expect $prompt                  \n"
            << "send \"pair $address\\r\"       \n"
            << "expect \"*yes/no\"              \n"
            << "send \"yes\\r\"                 \n"
            << "expect \"Pairing successful\"   \n"
            << "expect \"Device $address Connected: no\"   \n"
//            << "expect $prompt                  \n"
//            << "send \"connect $address\\r\"    \n"
//            << "expect \"Connection successful\"\n"
            << "expect $prompt                  \n"
            << "send \"quit\\r\"                \n"
            << "expect \"eof\"                  \n";

    //qDebug()<< "list ="<< list;

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
            //qDebug()<<"tmp ="<< tmp;
            data = tmp.toLatin1().data();
            //cout<<data;
            file.write(data);
        }
    }
    file.close();

    QString strCmd = QString("chmod +x /bt_pair.sh");
    qDebug() << "strCmd == " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;

    BtAddress = BtAddress.replace(QString("\n"), QString(""));
    strCmd = QString("/bt_pair.sh %1").arg(BtAddress);
    qDebug() << "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;

    bool PairResult=strResult.contains("Pairing successful",Qt::CaseInsensitive);
    qDebug() << PairResult;

    if(PairResult == 1)  //1 == true
    {
        strResult = "Pairing successful!";
    }
    else
    {
        strResult = "Pairing failed!";
    }

    return strResult;
}

QString bluetooth_thread::bluetooth_connect(QString BtAddress)
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

    //qDebug()<< "list ="<< list;

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
            //qDebug()<<"tmp ="<< tmp;
            char * data = tmp.toLatin1().data();
            //cout<<data;
            file.write(data);
        }
    }
    file.close();

    QString strCmd = QString("chmod +x /bt_connect.sh");
    qDebug() << "strCmd == " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << strResult;

    BtAddress = BtAddress.replace(QString("\n"), QString(""));
    strCmd = QString("/bt_connect.sh %1").arg(BtAddress);
    qDebug() << "strCmd == " << strCmd;

    strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;

    bool ConnectResult=strResult.contains("Connection successful",Qt::CaseInsensitive);
    qDebug() << ConnectResult;

    if(ConnectResult == 1)
    {
        strResult = "Connection successful!";
    }
    else
    {
        strResult = "Connection failed!";
    }

    return strResult;
}


QString bluetooth_thread::executeLinuxCmd(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished();
    QString strResult = p.readAllStandardOutput();
    return strResult;
}
