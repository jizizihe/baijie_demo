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

private slots:
    void on_return_2_clicked();

    void on_backlight_clicked();

    void on_rtc_clicked();

    void on_cn_clicked();

    void on_user_clicked();

    void on_about_clicked();

    void show_me();

    void out();

signals:
    void sigmain();
    void sys_cn();
    void main_cn();

private:
    Ui::sys_setting *ui;
    backlight backlight_w;
    timeset timeset_w;
    about_board board_w;
    user_manual user_w;
    QTranslator *transl;
    slideButton *slidebtn;

};

#endif // SYS_SETTING_H
