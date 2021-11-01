#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "globalapp.h"
#include "voice.h"
#include "udev.h"
#include "backlight.h"
#include "gpio.h"
#include "touchscreen.h"
#include "timeset.h"
#include "wifi.h"
#include "ipset.h"
#include "keytest.h"
#include "about_board.h"
#include "all_interface_test.h"
#include <QTranslator>
#include "bluetooth.h"
#include "user_manual.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    backlight backlight_w;
    voice voice_w;
    udev udev_w;
    gpio gpio_w;
    timeset timeset_w;
    touchscreen touch_w;
    wifi wifi_w;
    ipset eth0_w;
    keytest keytest_w;
    about_board board_w;
    all_interface_test all_w;
    bluetooth bluetooth_w;
    user_manual user_w;

private:
    Ui::MainWindow *ui;
    QTranslator *translator;

private slots:
    void show_main();


    void on_rtc_clicked();
    void on_backlight_clicked();
    void on_vf_clicked();
    void on_udev_clicked();
    void on_gpio_clicked();
    void on_touchca_clicked();
    void on_wifi_clicked();
    void on_ethernet_clicked();
    void on_keytest_clicked();
    void on_auto_2_clicked();
    void on_pushButton_clicked();
    void on_alltest_clicked();
    void on_bluetooth_clicked();
    void on_auto_4_clicked();
};

#endif // MAINWINDOW_H
