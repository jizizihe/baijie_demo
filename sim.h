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

    void languageReload();
    void simFont();
    void showEvent(QShowEvent *event);
    void switchSetText();

signals:
    void sim_back_msg();
    void sim_disconnect_msg();
    void sim_connect_msg();
    void sim_status_msg();
    void sim_module_status_msg();
    void sim_activation_msg(int);

private slots:
    void on_btn_ret_clicked();
    void recv_msg(int, QString );
    void on_btn_connect_clicked();
    void on_btn_disconnect_clicked();
    void on_btn_status_clicked();
    void module_status_refresh();
    void switch_change_flag(bool);

private:
    Ui::sim_module *ui;
    QLabel *loadLabel;
    QMovie *pMovie;
    QThread * myThread;
    wifi_thread * SimThread;
    QTimer *timer;
    wifi_bt_interface * wifi_bt_interfaceWg;
};

#endif // SIM_MODULE_H
