#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "globalapp.h"
#include "voice.h"
#include "udev.h"
#include "gpio.h"
#include "touchscreen.h"
#include "wifi.h"
#include "ipset.h"
#include "keytest.h"
#include "all_test.h"
#include <QTranslator>
#include "bluetooth.h"
#include "serial.h"
#include "sys_setting.h"
#include "wificondialog.h"
#include "sim_module.h"
#include "database.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    voice voice_w;
    udev udev_w;
    gpio gpio_w;
    touchscreen touch_w;
    wifi wifi_w;
    ipset eth0_w;
//    keytest keytest_w;
    all_test all_w;
    bluetooth bluetooth_w;
    serial serial_w;
    sys_setting system_w;
    QSettings *setting;
    WifiConDialog WifiConDialog_w;
    sim_module sim_module_w;
    database wifiDatabase;
    void config_file();

private:
    Ui::MainWindow *ui;
    QTranslator *translator;

private slots:
    void show_main();

    void on_vf_clicked();
    void on_udev_clicked();
    void on_gpio_clicked();
    void on_touchca_clicked();
    void on_wifi_clicked();
    void on_ethernet_clicked();
//    void on_keytest_clicked();

    void on_alltest_clicked();
    void on_bluetooth_clicked();
    void on_serialport_clicked();
    void on_system_clicked();
    void on_sim_module_clicked();

    void cn_main();

signals:
    void again();
    void wifi_status_msg();
};

#endif // MAINWINDOW_H
