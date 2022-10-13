#include "bluetooth_thread.h"

bluetooth_thread::bluetooth_thread(QObject *parent) : QObject(parent)
{
}

QString bluetooth_thread::bluetooth_scan_thread()
{
    QString scanResult = wifi_bt_w->bluetooth_scan();
    emit send_msg(Bt_scan_signal,scanResult );
    return scanResult;
}

QString bluetooth_thread::bluetooth_pair_thread(QString BtAddress)
{
    QString pairResult = wifi_bt_w->bluetooth_pair(BtAddress);
    emit send_msg(Bt_pair_signal,pairResult);
    return pairResult;
}

QString bluetooth_thread::bluetooth_connect_thread(QString BtAddress)
{
    QString connectResult = wifi_bt_w->bluetooth_connect(BtAddress);
    emit send_msg(Bt_connect_signal,connectResult);
    return connectResult;
}

/*

void bluetooth_thread::run()
{
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ ;

    //qDebug()<<"子线程功能函数ID:"<<QThread::currentThreadId();

    if(true == scan_flag)
    {
        QString scan_result = bluetooth_scan();
        //emit setText(true);
        emit message(1,scan_result);
    }
    else if(true == pair_flag)
    {
        QString pair_result = bluetooth_pair(BtMsg);
        emit message(2,pair_result);
    }
    else if(true == connect_flag)
    {
        QString connect_result = bluetooth_connect(BtMsg);
        emit message(3,connect_result);
    }
    else if(true == enable_flag)
    {
        bluetooth_enable(BtMsg);
    }
    else
    {

    }
}

void bluetooth_thread::flag_set(int signal_type,QString msg)
{
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ ;

    scan_flag = false;
    pair_flag = false;
    connect_flag = false;
    enable_flag = false;
    switch(signal_type)
    {
    case 1:
        scan_flag = true;
        break;
    case 2:
        pair_flag = true;
        BtMsg = msg;
        break;
    case 3:
        connect_flag = true;
        BtMsg = msg;
        break;
    case 4:
        enable_flag = true;
        BtMsg = msg;
        break;
    default:
        break;
    }
}



QString bluetooth_thread::bluetooth_scan()
{
    QString strCmd = QString("rfkill unblock all");
    //qDebug() << "strCmd == " << strCmd;
    QString strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;

//    strCmd = QString("killall hciattach");
//    //qDebug() << "strCmd == " << strCmd;
//    strResult = executeLinuxCmd(strCmd);
//    //qDebug() << strResult;
//    QThread::msleep(100);

    strCmd = QString("ps -ax |grep 'hciattach -n -s 1500000 /dev/ttyBT0 sprd' |grep -v grep |wc -l");
    strResult = executeLinuxCmd(strCmd);

    if(strResult == "0\n")
    {
        strCmd = QString("hciattach -n -s 1500000 /dev/ttyBT0 sprd 1>/dev/null 2>/dev/null &");
        strResult = executeLinuxCmd(strCmd);
        //qDebug() << strResult;
        QThread::sleep(2);
    }

    strCmd = QString("hciconfig hci0 up && hciconfig hci0 piscan");
    //qDebug() << "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;
    QThread::sleep(1);

    strCmd = QString("hcitool scan | sed \"1d\" ");
    //qDebug() << "strCmd == " << strCmd;
    strResult = executeLinuxCmd(strCmd);
    //qDebug() << strResult;

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
        strResult = "successful";
    }
    else
    {
        strResult = "failed";
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
        strResult = "successful";
    }
    else
    {
        strResult = "failed";
    }

    return strResult;
}

void bluetooth_thread::bluetooth_enable(QString power_flag)
{
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "power_flag:" << power_flag;

    QString strCmd;
    QString strResult;

    if(0 == power_flag)
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

QString bluetooth_thread::get_bluetooth_name()
{
    QString strCmd = QString("hciconfig hci0 name |grep Name |awk '{print $2}'");
    QString strResult = executeLinuxCmd(strCmd);
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;

    strResult.remove(0,1);
    strResult.remove(strResult.length()-2,2);
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;

    return strResult;
}

QString bluetooth_thread::executeLinuxCmd(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    return strResult;
}
*/
