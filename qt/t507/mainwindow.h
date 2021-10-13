#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "voice.h"
#include "udev.h"
#include "backlight.h"
#include "gpio.h"
#include "touchscreen.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    backlight backlight_w;
    voice voice_w;
    udev udev_w;
    gpio gpio_w;
    touchscreen touchscreen_w;

private slots:
    void show_main();

    void show_rtc();
    void show_voice();
    void show_udev();

    void on_rtc_clicked();
    void on_backlight_clicked();
    void on_vf_clicked();
    void on_udev_clicked();
    void on_gpio_clicked();
    void on_touchca_clicked();
};

#endif // MAINWINDOW_H
