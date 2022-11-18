#ifndef WIFI_H
#define WIFI_H

#include <QMainWindow>
#include "wifi_interface.h"
#include "wifi_thread.h"
#include "wifi_condialog.h"
#include "ipset_interface.h"

namespace Ui {
class wifi;
}

class wifi : public QMainWindow
{
    Q_OBJECT

public:
    explicit wifi(QWidget *parent = 0);
    ~wifi();
    void languageReload();
    void setWifiFont();
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
    void wifiListShow(QString,int,QString);
    void setSwitchText();

signals:
    void wifi_back_msg();
    void wifi_scan_msg();
    void set_wifi_enble(QString);
    void hotspot_build_msg(QString,QString,QString);
    void scan_wlan_msg();
    void get_hotspot_sql_msg();             //get the hotspot table information in the database

private slots:
    void on_btn_ret_clicked();
    void on_btn_scan_clicked();
    void on_btn_hotspot_clicked();
    void recv_msg(int, QString );
    void revc_scan_wlan(QString);
    void wifi_info_refresh(QString wifiName);
    void ListWidgeItem_clicked();
    void on_btn_hotspotBuild_clicked();
    void on_btn_hotspotDown_clicked();
    void on_btn_wifiChangePasswd_clicked();
    void on_btn_wifiRemove_clicked();
    void wifi_status_show();
    void wifi_scan_refresh();
    void wifidailog_hide();
    void wifi_status_refresh();
    void switch_change_flag(bool);
    void get_hotspot_sql();

private:
    Ui::wifi *ui;
    wifi_info g_wifiInfo;
    QLabel *g_loadLabel;
    QMovie *g_pMovie;
    QThread * g_myThread;
    wifi_thread * g_wifiThread;
    wifi_interface * g_wifiInterface;
    WifiConDialog *g_wifiConnectDialogWg;
    database g_database;
};

#endif // WIFI_H
