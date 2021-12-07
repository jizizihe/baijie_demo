#include "sys_setting.h"
#include "ui_sys_setting.h"

#include "mainwindow.h"


sys_setting::sys_setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sys_setting)
{
    ui->setupUi(this);

    connect(&backlight_w,SIGNAL(Mysignal()),this,SLOT(show_me()));
    connect(&timeset_w,SIGNAL(Mysignal()),this,SLOT(show_me()));
    connect(&user_w,SIGNAL(Mysignal()),this,SLOT(show_me()));
    connect(&board_w,SIGNAL(Mysignal()),this,SLOT(show_me()));

//    slidebtn = new slideButton(this);
//    slidebtn->setGeometry(720,300,100,100);
//    slidebtn->initflag = 0;
//    connect(slidebtn,SIGNAL(buttonChange()),this,SLOT(out()));

}

sys_setting::~sys_setting()
{
    delete ui;
}

void sys_setting::out()
{
//    if(slidebtn->switchflag)
//    {
//        qDebug() << slidebtn->switchflag << "11111111111" << __LINE__;
//    }
//    else
//    {
//        qDebug() << slidebtn->switchflag << "22222222222" << __LINE__;
//    }
}

void sys_setting::on_return_2_clicked()
{
    this->hide();
    emit sigmain();
}

void sys_setting::show_me()
{
    backlight_w.hide();
    timeset_w.hide();
    user_w.hide();
    board_w.hide();
    this->show();
}

void sys_setting::on_backlight_clicked()
{
    this->hide();
    backlight_w.show();
}

void sys_setting::on_rtc_clicked()
{
    this->hide();
    timeset_w.show();
}

void sys_setting::on_cn_clicked()
{

    static bool flag = 0;
    if(flag)
    {
        delete transl;
        transl = NULL;
    }
    else
    {
        transl = new QTranslator();
        transl->load(":/chinese.qm");
    }
    flag = !flag;
    emit main_cn(); //发送一个信号,翻译主界面的内容
    ui->retranslateUi(this);

    timeset_w.language_reload();
    backlight_w.language_reload();

    board_w.language_reload();
    user_w.language_reload();

}

void sys_setting::on_user_clicked()
{
    this->hide();
    user_w.show();
}

void sys_setting::on_about_clicked()
{
    this->hide();
    board_w.show();
}

