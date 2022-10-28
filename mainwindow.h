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
#include "all_test.h"
#include <QTranslator>
#include "bluetooth.h"
#include "serial.h"
#include "sys_setting.h"
#include "sim_module.h"
#include "database.h"
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    database databaseWg;
    voice voiceWg;
    udev udevWg;
    gpio gpioWg;
    touchscreen touchWg;
    wifi wifiWg;
    ipset eth0Wg;
    all_test allWg;
    bluetooth bluetoothWg;
    serial serialWg;
    sys_setting systemWg;
    sim_module simModuleWg;

    void mainFont();
    void voiceShow();
    void gpioShow();
    void wifiShow();
    void ipsetShow();
    void serialShow();
    void systemShow();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QTranslator *translator;

private slots:
    void on_btn_voice_clicked();
    void on_btn_udev_clicked();
    void on_btn_gpio_clicked();
    void on_btn_touch_clicked();
    void on_btn_wifi_clicked();
    void on_btn_ethernet_clicked();
    void on_btn_allTest_clicked();
    void on_btn_bluetooth_clicked();
    void on_btn_serialPort_clicked();
    void on_btn_system_clicked();
    void on_btn_simModule_clicked();
    void cn_main();
    void voice_back();
    void udev_back();
    void gpio_back();
    void touch_back();
    void wifi_back();
    void eth0_back();
    void all_back();
    void bluetooth_back();
    void serial_back();
    void sim_module_back();
    void sys_back();

signals:
    void wifi_status_msg();
};

#endif // MAINWINDOW_H
