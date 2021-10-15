#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "voice.h"
#include "udev.h"
#include "backlight.h"
#include "gpio.h"
#include "touchscreen.h"
#include "timeset.h"

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
    timeset timeset_w;

private:
    Ui::MainWindow *ui;


private slots:
    void show_main();

    void show_rtc();
    void show_voice();
    void show_udev();
    void show_gpio();
    void show_touch();

    void on_rtc_clicked();
    void on_backlight_clicked();
    void on_vf_clicked();
    void on_udev_clicked();
    void on_gpio_clicked();
};

#endif // MAINWINDOW_H
