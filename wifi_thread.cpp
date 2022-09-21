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
    //qDebug() << "--line--: " << __LINE__<< "FILE:" << __FILE__<< "--------------";

    QString strResult = wifi_bt_w->wifi_scan();
    //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "strResult:" << strResult;

    emit send_msg(wifi_scan_signal,strResult);
}


void wifi_thread::wifi_activation_thread(QString wifi_name)
{
    QString strResult;

    strResult = wifi_bt_w->wifi_activation(wifi_name);
   // qDebug() << "--LINE--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;
    bool ConnectResult=strResult.contains("Connection successfully activated",Qt::CaseInsensitive);
    //qDebug() << ConnectResult;

    if(ConnectResult == 1)
    {
        strResult = QString(1);
    }
    else
    {
        strResult = QString("");
    }
    emit send_msg(wifi_activation_signal,strResult);
    emit wifi_info_fresh_msg(wifi_name);

}

void wifi_thread::wlan_scan()
{
    QString strResult;
    strResult = wifi_bt_w->scan_wlan();
    emit rewlan_scan(strResult);
}

void wifi_thread::wifi_connect_thread(QString wifi_name,QString wifi_passwd)
{
    QString strResult;
    //qDebug() << "FUNC:" << __FUNCTION__<< "--line--: " << __LINE__<< "-------------------------";

    bool flag = wifi_bt_w->wifi_connect_exist(wifi_name);
    if(flag == true)
    {
        strResult = wifi_bt_w->wifi_activation(wifi_name);
        //qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< strResult;
        bool ConnectResult=strResult.contains("Connection successfully activated",Qt::CaseInsensitive);

        if(ConnectResult == 1)
        {
            strResult = QString(1);
        }
        else
        {
            strResult = QString("");
        }
    }
    else
    {
        strResult = wifi_bt_w->wifi_connect(wifi_name,wifi_passwd);
        //qDebug()<< "LINE:" << __LINE__ << "strResult" << strResult;
    }
    emit wifi_wait_end_msg();
    emit send_msg(wifi_connect_signal,strResult);
}

void wifi_thread::hotspot_build_thread(QString hotspot_connect,QString HtName,QString HtPasswd)
{
    QString strResult;

    strResult = wifi_bt_w->hotspot_connect(hotspot_connect,HtName,HtPasswd);
    //qDebug() << "Line:" << __LINE__ << "FUNC:" << __FUNCTION__<< "strResult:" << strResult;

    emit send_msg(hotspot_build_signal ,strResult);
}

void wifi_thread::sim_disconnect_thread_func()
{
    QString strResult = wifi_bt_w->sim_disconnect();

    emit sim_send_msg(sim_disconnect_signal,strResult);
}

void wifi_thread::sim_connect_thread_func()
{
    QString strResult = wifi_bt_w->sim_connect();

    emit sim_send_msg(sim_connect_signal,strResult);
}

void wifi_thread::sim_activation(int flag)
{
    wifi_bt_w->sim_activation(flag);
}

void wifi_thread::sim_status_thread_func()
{
    QString strResult = wifi_bt_w->sim_status();
    emit sim_send_msg(sim_status_signal,strResult);
}

void wifi_thread::sim_4gstatus()
{
    QString strResult = wifi_bt_w->sim_4gstatus();
    emit sim_send_msg(sim_4gstatus_signal,strResult);
}

void wifi_thread::wifi_modyfypass(QString WifiSsid, QString PassWd)
{
    bool strResult = wifi_bt_w->wifi_modify(WifiSsid,PassWd);
    emit wifi_modify(strResult);
}
