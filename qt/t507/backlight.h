#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <QMainWindow>
#include <QMainWindow>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QDateTime>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

//include/video/sunxi_display2.h)
#define	DISP_LCD_SET_BRIGHTNESS  0x102
#define DISP_LCD_GET_BRIGHTNESS  0x103
#define	DISP_LCD_BACKLIGHT_ENABLE   0x104
#define	DISP_LCD_BACKLIGHT_DISABLE  0x105

#include "globalapp.h"

int set_backlight(int value);
int get_backlight();

namespace Ui {
class backlight;
}

class backlight : public QMainWindow
{
    Q_OBJECT

public:
    explicit backlight(QWidget *parent = 0);
    ~backlight();
    void timerUpDate(int secs);
    QTimer *timing;

private:
    Ui::backlight *ui;


signals:
    void Mysignal();

private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_normal_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_sure_clicked();
    void on_return_2_clicked();

    void light_screen();

    void timerUp();


};

#endif // BACKLIGHT_H
