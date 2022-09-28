#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/stat.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>

static int screen_flag = 0;
static int Width;  //屏幕宽
static int Height;
static int voice_flag = 0;
static int udev_flag = 0;
static int gpio_flag = 0;
static int wifi_flag = 0;
static int eth0_flag = 0;
static int all_flag = 0;
static int bluetooth_flag = 0;
static int serial_flag = 0;
static int sim_module_flag = 0;
static int sys_flag = 0;

QGraphicsView *voice_view;
QGraphicsView *udev_view;
QGraphicsView *gpio_view;
QGraphicsView *wifi_view;
QGraphicsView *eth0_view;
QGraphicsView *all_view;
QGraphicsView *bluetooth_view;
QGraphicsView *serial_view;
QGraphicsView *sim_module_view;
QGraphicsView *system_view;
QGraphicsView *keyboard_view;

static QScreen *screen;
static int show_num;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    screen = qApp->primaryScreen();
    Width = screen->size().width();
    Height = screen->size().height();

    if(Width < Height)
    {
        screen_flag = 1;
    }
    main_font();
    connect(&voice_w,SIGNAL(Mysignal()),this,SLOT(voice_back()));
    connect(&udev_w,SIGNAL(Mysignal()),this,SLOT(udev_back()));
    connect(&gpio_w,SIGNAL(Mysignal()),this,SLOT(gpio_back()));
    connect(&touch_w,SIGNAL(Mysignal()),this,SLOT(touch_back()));

    connect(this,SIGNAL(wifi_status_msg()),&wifi_w,SLOT(WifiStatus_show()));
    connect(&wifi_w,SIGNAL(Mysignal()),this,SLOT(wifi_back()));
    connect(&eth0_w,SIGNAL(ret_signal()),this,SLOT(eth0_back()));
    connect(&all_w,SIGNAL(Mysignal()),this,SLOT(all_back()));
    connect(&bluetooth_w,SIGNAL(Mysignal()),this,SLOT(bluetooth_back()));
    connect(&serial_w,SIGNAL(Mysignal()),this,SLOT(serial_back()));
    connect(&system_w,SIGNAL(sigmain()),this,SLOT(sys_back()));
    connect(&system_w,SIGNAL(main_cn()),this,SLOT(cn_main()));
    connect(&sim_module_w,SIGNAL(Mysignal()),this,SLOT(sim_module_back()));

//    database_w.create_connection();
//    database_w.create_table();
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(4, 4);
    shadow_effect->setColor(QColor (136, 138, 133));
    shadow_effect->setBlurRadius(5);
    ui->label->setGraphicsEffect(shadow_effect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::config_file()
{
//    QString localSettingsPath("/data");
//    QDir dir(localSettingsPath);
//    if (!dir.exists()) {
//        dir.mkpath(localSettingsPath);  // mkdir如果上层路径不存在就会创建失败, 因此用mkpath
//    }

//    QString localSettingsFile = localSettingsPath + QDir::separator()  + "HelperBoard.ini";
//    QFile file(localSettingsFile);
//    if (!file.exists() && !file.open(QIODevice::WriteOnly|QIODevice::Text)) {  // 如果文件的上层路径不存在就会创建失败
//        qDebug()<<"failed";
//     }
//    file.close();

//    setting = new QSettings(localSettingsFile,QSettings::IniFormat);
//    setting->SystemScope;
//    setting->setIniCodec("UTF-8");
}

void MainWindow::on_vf_clicked()
{
    this->close();
    voice_show();
}

void MainWindow::on_udev_clicked()
{
    this->hide();
    if(screen_flag == 0)
    {
        udev_w.resize(Width,Height);
        udev_w.show();
        udev_w.activateWindow();udev_w.setFocus();
    }
    else
    {
        if(udev_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&udev_w);
            w->setRotation(90);
            udev_view = new QGraphicsView(scene);
            udev_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            udev_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            udev_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            udev_view->resize(Width,Height);
            udev_w.resize(Height,Width);
            udev_w.show();
            udev_view->show();
            udev_view->activateWindow();udev_view->setFocus();
            udev_w.activateWindow();udev_w.setFocus();
            udev_flag++;
        }
        else
        {
            udev_w.show();
            udev_view->show();
            udev_view->activateWindow();udev_view->setFocus();
            udev_w.activateWindow();udev_w.setFocus();
        }
    }
}

void MainWindow::on_gpio_clicked()
{
    this->close();
    gpio_show();
}

void MainWindow::on_touchca_clicked()
{
    this->hide();
    touch_w.resize(Width,Height);
    touch_w.show();
    touch_w.activateWindow();touch_w.setFocus();
}

void MainWindow::on_wifi_clicked()
{
//    if(wifi_showf == 0)
//    {

//       emit wifi_status_msg();
//        wifi_showf++;
//    }
    this->close();
    wifi_show();
}
void MainWindow::on_ethernet_clicked()
{
    this->close();
    ipset_show();
}

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
        //emit wifidia_translater(1);
    }
    languageflag = !languageflag;
    ui->retranslateUi(this);
    wifi_w.language_reload();
    eth0_w.language_reload();
    gpio_w.language_reload();
    voice_w.language_reload();
    touch_w.language_reload();
    udev_w.language_reload();
    bluetooth_w.language_reload();
    all_w.language_reload();
    serial_w.language_reload();
    sim_module_w.language_reload();
    emit wifi_status_msg();
    wifi_w.hotspot_sql();
}

void MainWindow::on_bluetooth_clicked()
{
    this->close();
    if(screen_flag == 0)
    {
        bluetooth_w.resize(Width,Height);
        bluetooth_w.show();
        bluetooth_w.activateWindow();bluetooth_w.setFocus();
    }
    else
    {
        if(bluetooth_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&bluetooth_w);
            w->setRotation(90);

            bluetooth_view = new QGraphicsView(scene);

            bluetooth_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            bluetooth_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            bluetooth_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            bluetooth_view->resize(Width,Height);
            bluetooth_w.resize(Height,Width);
            bluetooth_w.show();
            bluetooth_view->show();
            bluetooth_view->activateWindow();bluetooth_view->setFocus();
            bluetooth_w.activateWindow();bluetooth_w.setFocus();
            bluetooth_flag++;
        }
        else
        {
            bluetooth_w.show();
            bluetooth_view->show();
            bluetooth_view->activateWindow();bluetooth_view->setFocus();
            bluetooth_w.activateWindow();bluetooth_w.setFocus();
        }
    }
}

void MainWindow::on_alltest_clicked()
{
    this->hide();
    if(screen_flag == 0)
    {
        all_w.resize(Width,Height);
        all_w.show();
        all_w.activateWindow();all_w.setFocus();
    }
    else
    {
        if(all_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&all_w);
            w->setRotation(90);

            all_view = new QGraphicsView(scene);
            all_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            all_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            all_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            all_view->resize(Width,Height);
            all_w.resize(Height,Width);
            all_w.show();
            all_view->show();
            all_view->activateWindow();all_view->setFocus();
            all_w.activateWindow();all_w.setFocus();
            all_flag++;
        }
        else
        {
            all_w.show();
            all_view->show();
            all_view->activateWindow();all_view->setFocus();
            all_w.activateWindow();all_w.setFocus();
        }
    }
}


void MainWindow::on_serialport_clicked()
{
    this->close();
    serial_show();
}

void MainWindow::on_system_clicked()
{
    this->close();
    system_show();
}

void MainWindow::on_sim_module_clicked()
{
    this->close();
    if(screen_flag == 0)
    {
        sim_module_w.resize(Width,Height);
        sim_module_w.show();
        sim_module_w.activateWindow();sim_module_w.setFocus();
    }
    else
    {
        if(sim_module_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&sim_module_w);
            w->setRotation(90);

            sim_module_view = new QGraphicsView(scene);

            sim_module_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            sim_module_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            sim_module_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            sim_module_view->resize(Width,Height);
            sim_module_w.resize(Height,Width);
            sim_module_w.show();
            sim_module_view->show();
            sim_module_view->activateWindow();sim_module_view->setFocus();
            sim_module_w.activateWindow();sim_module_w.setFocus();
            sim_module_flag++;
        }
        else
        {
            sim_module_w.show();
            sim_module_view->show();
            sim_module_view->activateWindow();sim_module_view->setFocus();
            sim_module_w.activateWindow();sim_module_w.setFocus();
        }
    }
}

void MainWindow::voice_back()
{
    if(screen_flag == 1)
    {
       voice_view->hide();
    }
    voice_w.close();
    this->show();
}

void MainWindow::udev_back()
{
    if(screen_flag == 1)
    {
       udev_view->hide();
    }
    udev_w.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::gpio_back()
{
    if(screen_flag == 1)
    {
        gpio_view->hide();
    }
    gpio_w.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::touch_back()
{
   touch_w.hide();
   this->show();
   this->activateWindow();this->setFocus();
}

void MainWindow::wifi_back()
{
    if(screen_flag == 1)
    {
       wifi_view->hide();
    }
    wifi_w.close();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::eth0_back()
{
    if(screen_flag == 1)
    {
       eth0_view->hide();
    }
    eth0_w.hide();    
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::all_back()
{
    if(screen_flag == 1)
    {
        all_view->hide();
    }
    all_w.hide();   
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::bluetooth_back()
{
    if(screen_flag == 1)
    {
       bluetooth_view->hide();
    }
    bluetooth_w.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::serial_back()
{
    if(screen_flag == 1)
    {
      serial_view->hide();
    }
    serial_w.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::sim_module_back()
{
    if(screen_flag == 1)
    {
      sim_module_view->hide();
    }
    sim_module_w.hide();    
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::sys_back()
{
    if(screen_flag == 1)
    {
      system_view->hide();
    }
    system_w.close();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::main_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = Width / realX * 2.54;
    qreal realHeight = Height / realY *2.54;
    QFont font;
    if(screen_flag)
    {
        if(realHeight < 15)
        {
            font.setPointSize(13);
            ui->label->setFont(font);
            font.setPointSize(12);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(16);
            ui->label->setFont(font);
            font.setPointSize(15);
        }
        else
        {
            font.setPointSize(19);
            ui->label->setFont(font);
            font.setPointSize(18);
        }
    }
    else
    {
        if(realWidth < 15)
        {
            font.setPointSize(13);
            ui->label->setFont(font);
            font.setPointSize(12);
        }
        else if (realWidth < 17)
        {
            font.setPointSize(16);
            ui->label->setFont(font);
            font.setPointSize(15);
        }
        else
        {
            font.setPointSize(19);
            ui->label->setFont(font);
            font.setPointSize(18);
        }
    }

    ui->label_1->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->label_4->setFont(font);
    ui->label_5->setFont(font);
    ui->label_6->setFont(font);
    ui->label_7->setFont(font);
    ui->label_8->setFont(font);
    ui->label_9->setFont(font);
    ui->label_10->setFont(font);
    ui->label_11->setFont(font);
  //  ui->label->setFont(font);
}

void MainWindow::voice_show()
{
    if(screen_flag == 0)
    {
        voice_w.resize(Width,Height);
        voice_w.show();
        voice_w.activateWindow();voice_w.setFocus();
    }
    else
    {
        if(voice_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&voice_w);
            w->setRotation(90);

            voice_view = new QGraphicsView(scene);

            voice_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            voice_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            voice_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            voice_view->resize(Width,Height);
            voice_w.resize(Height,Width);
            voice_w.show();
            voice_view->show();
            voice_view->activateWindow();voice_view->setFocus();
            voice_w.activateWindow();voice_w.setFocus();
            voice_flag++;
        }
        else
        {
            voice_w.show();
            voice_view->show();
            voice_view->activateWindow();voice_view->setFocus();
            voice_w.activateWindow();voice_w.setFocus();
        }
    }
}

void MainWindow::serial_show()
{
    if(screen_flag == 0)
    {
        serial_w.resize(Width,Height);
        serial_w.show();
        serial_w.activateWindow();serial_w.setFocus();
    }
    else
    {
        if(serial_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&serial_w);
            w->setRotation(90);

            serial_view = new QGraphicsView(scene);

            serial_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            serial_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            serial_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            serial_view->resize(Width,Height);
            serial_w.resize(Height,Width);
            serial_w.show();
            serial_view->show();
            serial_view->activateWindow();serial_view->setFocus();
            serial_w.activateWindow();serial_w.setFocus();
            serial_flag++;
        }
        else
        {
            serial_w.resize(Height,Width);
            serial_w.show();
            serial_view->resize(Width,Height);
            serial_view->show();
            serial_view->activateWindow();serial_view->setFocus();
            serial_w.activateWindow();serial_w.setFocus();
        }
    }
}

void MainWindow::ipset_show()
{
    if(screen_flag == 0)
    {
        eth0_w.resize(Width,Height);
        eth0_w.show();
        eth0_w.activateWindow();eth0_w.setFocus();
    }
    else
    {
        if(eth0_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&eth0_w);
            w->setRotation(90);

            eth0_view = new QGraphicsView(scene);

            eth0_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            eth0_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            eth0_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            eth0_view->resize(Width,Height);
            eth0_w.resize(Height,Width);
            eth0_w.show();
            eth0_view->show();
            eth0_view->activateWindow();eth0_view->setFocus();
            eth0_w.activateWindow();eth0_w.setFocus();
            eth0_flag++;
        }
        else
        {
            eth0_w.resize(Height,Width);
            eth0_w.show();
            eth0_view->resize(Width,Height);
            eth0_view->show();
            eth0_view->activateWindow();eth0_view->setFocus();
            eth0_w.activateWindow();eth0_w.setFocus();
        }
    }
}

void MainWindow::gpio_show()
{
    if(screen_flag == 0)
    {
        gpio_w.resize(Width,Height);
        gpio_w.show();
        gpio_w.activateWindow();gpio_w.setFocus();
    }
    else
    {
        if(gpio_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&gpio_w);
            w->setRotation(90);

            gpio_view = new QGraphicsView(scene);

            gpio_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            gpio_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            gpio_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            gpio_view->resize(Width,Height);
            gpio_w.resize(Height,Width);
            gpio_w.show();
            gpio_view->lower();
            gpio_view->show();
            gpio_view->activateWindow();gpio_view->setFocus();
            gpio_w.activateWindow(); gpio_w.setFocus();
            gpio_flag++;
        }
        else
        {
            gpio_w.show();
            gpio_view->show();
            gpio_view->activateWindow();gpio_view->setFocus();
            gpio_w.activateWindow(); gpio_w.setFocus();
        }
    }
}

void MainWindow::system_show()
{
    if(screen_flag == 0)
    {
        system_w.resize(Width,Height);
        system_w.show();
        system_w.activateWindow();system_w.setFocus();
    }
    else
    {
        if(sys_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&system_w);
            w->setRotation(90);

            system_view = new QGraphicsView(scene);
            system_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            system_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            system_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            system_view->resize(Width,Height);
            system_w.resize(Height,Width);
            system_w.show();
            system_view->show();
            system_view->activateWindow();system_view->setFocus();
            system_w.activateWindow();system_w.setFocus();
            sys_flag++;
        }
        else
        {
            system_w.show();
            system_view->show();
            system_view->activateWindow();system_view->setFocus();
            system_w.activateWindow();system_w.setFocus();
        }
    }
}

void MainWindow::wifi_show()
{
    if(screen_flag == 0)
    {
        wifi_w.resize(Width,Height);
        wifi_w.show();
        wifi_w.activateWindow();wifi_w.setFocus();
    }
    else
    {
        if(wifi_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&wifi_w);
            w->setRotation(90);

            wifi_view = new QGraphicsView(scene);

            wifi_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            wifi_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            wifi_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            wifi_view->resize(Width,Height);
            wifi_w.resize(Height,Width);
            wifi_w.show();
            wifi_view->show();
            wifi_view->activateWindow();wifi_view->setFocus();
            wifi_w.activateWindow(); wifi_w.setFocus();
            wifi_flag++;
        }
        else
        {
            wifi_w.show();
            wifi_view->show();
            wifi_view->activateWindow();wifi_view->setFocus();
            wifi_w.activateWindow(); wifi_w.setFocus();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(show_num == 0)
    {
        wifi_show();wifi_back();
        serial_show();serial_back();
        ipset_show();eth0_back();
        gpio_show();gpio_back();
        voice_show();voice_back();
        system_show();sys_back();
        show_num++;
    }
    QWidget::closeEvent(event);
}
