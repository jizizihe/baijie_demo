#ifndef SIM_MODULE_H
#define SIM_MODULE_H

#include <QMainWindow>
#include "wifi_bt_interface.h"
#include "gpio_interface.h"
#include "wifi_thread.h"

namespace Ui {
class sim_module;
}

class sim_module : public QMainWindow
{
    Q_OBJECT

public:
    explicit sim_module(QWidget *parent = 0);
    ~sim_module();

    QString sim_disconnect();
    QString sim_connect();

    void language_reload();
    void sim_font();
    void showEvent(QShowEvent *event);

signals:
    void Mysignal();
    void sim_disconnect_msg();
    void sim_connect_msg();
    void sim_status_msg();
    void sim_4gstatus();
    void sim_activation(int);

private slots:
    void on_retBtn_clicked();
    void recv_msg(int, QString );
    void on_SimConnectBtn_clicked();
    void on_SimDisconnectBtn_clicked();
    void on_statusBtn_clicked();
    void status_4g_refresh();
    void btnChangeFlag(bool);

private:
    Ui::sim_module *ui;
    QLabel *LoadLabel;
    QMovie *pMovie;
    QThread * myThread;
    wifi_thread * SimThread;
    QTimer *timer;
    wifi_bt_interface * wifi_bt_interface_w;
};

#endif // SIM_MODULE_H
