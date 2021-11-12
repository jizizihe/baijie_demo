#include "wifi_thread.h"

wifi_thread::wifi_thread(QObject *parent) : QObject(parent)
{
    //qDebug()<<"Thread构造函数ID:"<<QThread::currentThreadId();
}

void wifi_thread::Thread_Fun()
{
}


void wifi_thread::wifi_scan_thread()
{
    QString strCmd = QString("iw dev wlan0 scan|grep SSID|awk '{for(i=2;i<=NF;i++){printf \"%s \", $i}; printf \"\\n\"}' ");
    //qDebug() << "strCmd == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    //qDebug() << strResult;

    emit send_msg(strResult);
}
