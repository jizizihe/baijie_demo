#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(&voice_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&udev_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&gpio_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&touch_w,SIGNAL(Mysignal()),this,SLOT(show_main()));

    connect(this,SIGNAL(wifi_status_msg()),&wifi_w,SLOT(WifiStatus_show()));
    connect(&wifi_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&eth0_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    //connect(&keytest_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&all_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&bluetooth_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&serial_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(&system_w,SIGNAL(sigmain()),this,SLOT(show_main()));
    connect(&system_w,SIGNAL(main_cn()),this,SLOT(cn_main()));
    connect(&sim_module_w,SIGNAL(Mysignal()),this,SLOT(show_main()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::config_file()
{
    QString localSettingsPath("/data");
    QDir dir(localSettingsPath);
    if (!dir.exists()) {
        dir.mkpath(localSettingsPath);  // mkdir如果上层路径不存在就会创建失败, 因此用mkpath
    }

    QString localSettingsFile = localSettingsPath + QDir::separator()  + "HelperBoard.ini";
    QFile file(localSettingsFile);
    if (!file.exists() && !file.open(QIODevice::WriteOnly|QIODevice::Text)) {  // 如果文件的上层路径不存在就会创建失败
        qDebug()<<"failed";
     }
    file.close();

    setting = new QSettings(localSettingsFile,QSettings::IniFormat);
    setting->SystemScope;
    setting->setIniCodec("UTF-8");
}

void MainWindow::show_main()
{
    voice_w.hide();
    udev_w.hide();
    gpio_w.hide();
    touch_w.hide();
    wifi_w.hide();
    eth0_w.hide();
    //keytest_w.hide();
    all_w.hide();
    bluetooth_w.hide();
    serial_w.hide();
    sim_module_w.hide();
    this->show();
}


void MainWindow::on_vf_clicked()
{
    this->hide();
    voice_w.show();
}

void MainWindow::on_udev_clicked()
{
    this->hide();
    emit again();
    udev_w.show();
}

void MainWindow::on_gpio_clicked()
{
    this->hide();
    gpio_w.show();
}

void MainWindow::on_touchca_clicked()
{
    this->hide();
    touch_w.show();
}

void MainWindow::on_wifi_clicked()
{
    emit wifi_status_msg();
    this->hide();
    wifi_w.show();
}
void MainWindow::on_ethernet_clicked()
{
    this->hide();
    eth0_w.show();
}


//void MainWindow::on_keytest_clicked()
//{
//    this->hide();
//    keytest_w.show();
//}

void MainWindow::cn_main()
{
    static bool languageflag = 0;
    if(languageflag)
    {
        qApp->removeTranslator(translator);
        delete translator;
        translator = NULL;
    }
    else
    {
        translator = new QTranslator();
        translator->load(":/chinese");
        qApp->installTranslator(translator);
    }
    languageflag = !languageflag;
    ui->retranslateUi(this);
    wifi_w.language_reload();
    eth0_w.language_reload();
    gpio_w.language_reload();
    voice_w.language_reload();
    touch_w.language_reload();
    udev_w.language_reload();
    //keytest_w.language_reload();
    bluetooth_w.language_reload();
    all_w.language_reload();
    serial_w.language_reload();
    WifiConDialog_w.language_reload();
    sim_module_w.language_reload();
}

void MainWindow::on_bluetooth_clicked()
{
    this->hide();
    bluetooth_w.show();
}

void MainWindow::on_alltest_clicked()
{
    this->hide();
    all_w.show();
}


void MainWindow::on_serialport_clicked()
{
    this->hide();
    serial_w.show();
}

void MainWindow::on_system_clicked()
{
    this->hide();
    system_w.show();
}

void MainWindow::on_sim_module_clicked()
{
    this->hide();
    sim_module_w.show();
}
