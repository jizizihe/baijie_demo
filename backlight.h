#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <QMainWindow>
#include <QMainWindow>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <QProcess>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define	DISP_LCD_SET_BRIGHTNESS  0x102
#define DISP_LCD_GET_BRIGHTNESS  0x103
#define	DISP_LCD_BACKLIGHT_ENABLE   0x104
#define	DISP_LCD_BACKLIGHT_DISABLE  0x105

#include "globalapp.h"
#include "wifi_bt_interface.h"

int setBacklight(int value);
int getBacklight();

namespace Ui {
class backlight;
}

class backlight : public QMainWindow
{
    Q_OBJECT

public:
    explicit backlight(QWidget *parent = 0);
    ~backlight();
    QTimer *timing;
    void languageReload();
    void backlightFont();

private:
    Ui::backlight *ui;

signals:
    void backlight_back_msg();

private slots:
    void on_horizontalSlider_light_valueChanged(int value);
    void on_cmb_sleep_time_currentIndexChanged(int index);
    void on_btn_ret_clicked();
    void light_screen();
    void timer_up();       //check whether events are generated
};

#endif // BACKLIGHT_H
