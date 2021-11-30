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
    //qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< "--------------";

    QString strResult = wifi_bt_w->wifi_scan();
    //qDebug()<< "LINE:" << __LINE__ << "ScanResult" << strResult;

    emit send_msg(wifi_scan_signal,strResult);
}


void wifi_thread::wifi_activation_thread(QString wifi_name)
{
    QString strResult;

    strResult = wifi_bt_w->wifi_activation(wifi_name);
    qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< strResult;
    bool ConnectResult=strResult.contains("Connection successfully activated",Qt::CaseInsensitive);
    qDebug() << ConnectResult;

    if(ConnectResult == 1)
    {
        strResult = QString(1);
    }
    else
    {
        strResult = QString("");
    }
    emit send_msg(wifi_activation_signal,strResult);
}


void wifi_thread::wifi_connect_thread(QString wifi_name,QString wifi_passwd)
{
    QString strResult;
    qDebug() << "FUNC:" << __FUNCTION__<< "--line--: " << __LINE__<< "-------------------------";

    bool flag = wifi_bt_w->wifi_connect_exist(wifi_name);
    if(flag == true)
    {
        strResult = wifi_bt_w->wifi_activation(wifi_name);
        qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< strResult;
        bool ConnectResult=strResult.contains("Connection successfully activated",Qt::CaseInsensitive);
        qDebug() << ConnectResult;

        if(ConnectResult == 1)
        {
            strResult = QString(1);
        }
        else
        {
            strResult = QString("");
        }
        emit send_msg(wifi_connect_signal,strResult);
    }
    else
    {
        strResult = wifi_bt_w->wifi_connect(wifi_name,wifi_passwd);
        qDebug()<< "LINE:" << __LINE__ << "strResult" << strResult;
        emit send_msg(wifi_connect_signal,strResult);
    }
}

void wifi_thread::hotspot_build_thread(QString HtName,QString HtPasswd)
{
    QString strResult;

    strResult = wifi_bt_w->hotspot_connect(HtName,HtPasswd);
    qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;

    emit send_msg(hotspot_build_signal ,strResult);
}

