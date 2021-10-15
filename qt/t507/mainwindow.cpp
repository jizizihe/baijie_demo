#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&backlight_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&voice_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&udev_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&gpio_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&timeset_w,SIGNAL(Mysignal()),this,SLOT(show_main()));

    connect(&touch_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::show_rtc()
{
    backlight_w.show();
}

void MainWindow::show_voice()
{
    voice_w.show();
}

void MainWindow::show_udev()
{
    udev_w.show();
}

void MainWindow::show_gpio()
{
    gpio_w.show();
}

void MainWindow::show_touch()
{
    touch_w.show();
}

void MainWindow::on_rtc_clicked()
{
    this->show();
    timeset_w.show();
}

void MainWindow::show_main()
{
    backlight_w.hide();
    voice_w.hide();
    udev_w.hide();
    gpio_w.hide();
<<<<<<< HEAD
    timeset_w.hide();
=======
    touch_w.hide();
>>>>>>> d54904c1139c5b82f3a76858ad3777a392359f2c
    this->show();
}

void MainWindow::on_backlight_clicked()
{
    this->hide();
    show_rtc();
}

void MainWindow::on_vf_clicked()
{
    this->hide();
    voice_w.show();
}

void MainWindow::on_udev_clicked()
{
    this->hide();
    udev_w.show();
}



void MainWindow::on_gpio_clicked()
{
    this->hide();
    gpio_w.show();
}
<<<<<<< HEAD
=======

void MainWindow::on_touchca_clicked()
{
    this->hide();
    touch_w.show();
}
>>>>>>> d54904c1139c5b82f3a76858ad3777a392359f2c
