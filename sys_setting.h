#ifndef SYS_SETTING_H
#define SYS_SETTING_H

#include <QWidget>
#include <QTranslator>
#include "backlight.h"
#include "timeset.h"
#include "user_manual.h"
#include "about_board.h"
#include "ctouchbutton.h"

namespace Ui {
class sys_setting;
}

class sys_setting : public QWidget
{
    Q_OBJECT

public:
    explicit sys_setting(QWidget *parent = 0);
    ~sys_setting();
    void sysFont();
    void closeEvent(QCloseEvent *event);
    void catOTGStatus();

private slots:
    void on_btn_ret_clicked();
    void on_btn_backlight_clicked();
    void on_btn_RTC_clicked();
    void on_btn_cn_clicked();
    void on_btn_user_clicked();
    void on_btn_about_clicked();
    void backlight_back();
    void time_back();
    void user_back();
    void board_back();
    void on_btn_OTGHost_clicked();

signals:
    void sys_back_msg();
    void main_cn_msg();

private:
    Ui::sys_setting *ui;
    backlight backlightWg;
    timeset timesetWg;
    about_board boardWg;
    user_manual userWg;
    QTranslator *transl;

};

#endif // SYS_SETTING_H
