#ifndef WIFI_H
#define WIFI_H

#include <QMainWindow>
#include "wifi_bt_interface.h"
#include "wifi_thread.h"
#include "wificondialog.h"

namespace Ui {
class wifi;
}

class wifi : public QMainWindow
{
    Q_OBJECT

public:
    explicit wifi(QWidget *parent = 0);
    ~wifi();

    void language_reload();

signals:
    void Mysignal();
    void ToThread(); // 信号
    void wifi_scan_msg();
    void wifi_connect_msg(QString, QString );
    void wifi_activation_msg(QString);
    void hotspot_build_msg(QString,QString);

private slots:
    void on_ReturnBtn_clicked();

    void on_WifiConnectBtn_clicked();

    void on_HotspotConBtn_clicked();

    void BtnChange_flag(bool);

    void recv_msg(int, QString );

    void on_RefreshBtn_clicked();

    void wifi_info_fresh(QString wifi_name);

    void ListWidgeItem_clicked();

    void on_HotspotBuildBtn_clicked();

    void on_HotspotDownBtn_clicked();

    void on_ChangePasswdBtn_clicked();

    void on_WifiExistRemoveBtn_clicked();

    void on_WifiStatusBtn_clicked();
    void WifiStatus_show();
private:
    Ui::wifi *ui;

    wifi_info WifiInfo;

    QLabel *LoadLabel;
    QMovie *pMovie;

    QThread * myThread;
    wifi_thread * WifiThread;
    wifi_bt_interface * wifi_bt_t;

    WifiConDialog *WifiConnectDialog;
    database wifiDB;
};

#endif // WIFI_H
