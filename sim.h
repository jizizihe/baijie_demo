#ifndef SIM_MODULE_H
#define SIM_MODULE_H

#include <QMainWindow>
#include "sim_interface.h"
#include "gpio_interface.h"
#include "sim_thread.h"

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
    void setSimFont();
    void showEvent(QShowEvent *event);
    void setSwitchText();

signals:
    void sim_back_msg();
    void sim_disconnect_msg();
    void sim_connect_msg();
    void sim_status_msg();
    void sim_module_status_msg();
    void sim_set_enble_msg(int);

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
    QLabel *g_loadLabel;
    QMovie *g_pMovie;
    QThread * g_myThread;
    sim_thread * g_SimThread;
    QTimer *g_timer;
    sim_interface * g_simInterface;
};

#endif // SIM_MODULE_H
