#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/stat.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>

static int g_screenFlag = 0;                    // 0: width > height  1: width < height
static int g_screenWidth;
static int g_screenHeight;
static int g_voiceFirstFlag = 0;                // 0: First time display   1: Not for the first time
static int g_udevFirstFlag = 0;
static int g_gpioFirstFlag = 0;
static int g_wifiFirstFlag = 0;
static int g_eth0FirstFlag = 0;
static int g_allTestFirstFlag = 0;
static int g_bluetoothFirstFlag = 0;
static int g_serialFirstFlag = 0;
static int g_simFirstFlag = 0;
static int g_sysFirstFlag = 0;
static int g_firstShowFlag = 0;

QGraphicsView *g_voiceView;
QGraphicsView *g_udevView;
QGraphicsView *g_gpioView;
QGraphicsView *g_wifiView;
QGraphicsView *g_eth0View;
QGraphicsView *g_allView;
QGraphicsView *g_bluetoothView;
QGraphicsView *g_serialView;
QGraphicsView *g_simModuleView;
QGraphicsView *g_systemView;

static QScreen *g_screen;
static int g_showFirstFlag;   //0:The MainWindow is displayed for the first time

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    g_screen = qApp->primaryScreen();                      // Width and height of thg_e g_screen
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();

    if(g_screenWidth < g_screenHeight)
    {
        g_screenFlag = 1;
    }
    else
    {
        QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
        shadowEffect->setOffset(4, 4);
        shadowEffect->setColor(QColor (136, 138, 133));
        shadowEffect->setBlurRadius(5);
        ui->lbl_baijieTechnologyFunctionDisplay->setGraphicsEffect(shadowEffect);      //Add shadow to font
    }

    QFont font;
    font.setLetterSpacing(QFont::PercentageSpacing,110);         //word space
    font.setPointSize(15);
    ui->lbl_baijieTechnologyFunctionDisplay->setFont(font);
    setMainWindowFont();

    connect(&g_voiceWg,SIGNAL(voice_back_msg()),this,SLOT(voice_back()));
    connect(&g_udevWg,SIGNAL(udev_back_msg()),this,SLOT(udev_back()));
    connect(&g_gpioWg,SIGNAL(gpio_back_msg()),this,SLOT(gpio_back()));
    connect(&g_touchWg,SIGNAL(touch_back_msg()),this,SLOT(touch_back()));
    connect(this,SIGNAL(wifi_status_msg()),&g_wifiWg,SLOT(wifi_status_show()));
    connect(&g_wifiWg,SIGNAL(wifi_back_msg()),this,SLOT(wifi_back()));
    connect(&g_eth0Wg,SIGNAL(ipset_back_msg()),this,SLOT(eth0_back()));
    connect(&g_allWg,SIGNAL(all_test_back_msg()),this,SLOT(all_back()));
    connect(&g_bluetoothWg,SIGNAL(bluetooth_back_msg()),this,SLOT(bluetooth_back()));
    connect(&g_serialWg,SIGNAL(serial_back_msg()),this,SLOT(serial_back()));
    connect(&g_systemWg,SIGNAL(sys_back_msg()),this,SLOT(sys_back()));
    connect(&g_systemWg,SIGNAL(main_cn_msg()),this,SLOT(cn_main()));
    connect(&g_simModuleWg,SIGNAL(sim_back_msg()),this,SLOT(sim_module_back()));
    connect(this,SIGNAL(cn_msg()),this,SLOT(cn_main()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_voice_clicked()
{
    this->close();
    voiceShow();
}

void MainWindow::on_btn_udev_clicked()
{
    this->hide();
    if(g_screenFlag == 0)
    {
        g_udevWg.resize(g_screenWidth,g_screenHeight);
        g_udevWg.show();
        g_udevWg.activateWindow();g_udevWg.setFocus();
    }
    else                                                        //width < height: Rotate 90 degrees
    {
        if(g_udevFirstFlag == 0)                                  //the interface is displayed for the first time, the interface need to initialize a rotation of 90
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_udevWg);
            w->setRotation(90);
            g_udevView = new QGraphicsView(scene);
            g_udevView->setWindowFlags(Qt::FramelessWindowHint);
            g_udevView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_udevView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            g_udevView->resize(g_screenWidth,g_screenHeight);
            g_udevWg.resize(g_screenHeight,g_screenWidth);
            g_udevWg.show();
            g_udevView->show();
            g_udevView->activateWindow();
            g_udevView->setFocus();
            g_udevWg.activateWindow();g_udevWg.setFocus();
            g_udevFirstFlag++;
        }
        else
        {
            g_udevWg.show();
            g_udevView->show();
            g_udevView->activateWindow();
            g_udevView->setFocus();
            g_udevWg.activateWindow();g_udevWg.setFocus();
        }
    }
}

void MainWindow::on_btn_gpio_clicked()
{
    this->close();
    gpioShow();
}

void MainWindow::on_btn_touch_clicked()
{
    this->hide();
    g_touchWg.resize(g_screenWidth,g_screenHeight);
    g_touchWg.show();
    g_touchWg.activateWindow();g_touchWg.setFocus();
}

void MainWindow::on_btn_wifi_clicked()
{
    this->close();
    wifiShow();
}
void MainWindow::on_btn_ethernet_clicked()
{
    this->close();
    ipsetShow();
}

void MainWindow::cn_main()                          //chinese-english switch
{
    static bool languageFlag = 0;
    if(languageFlag)
    {
        qApp->removeTranslator(g_translator);
        delete g_translator;
        g_translator = NULL;
        g_database.updateTableOne("language","English");
    }
    else
    {
        g_translator = new QTranslator();
        g_translator->load(":/chinese");
        qApp->installTranslator(g_translator);
        g_database.updateTableOne("language","Chinese");
    }
    languageFlag = !languageFlag;
    ui->retranslateUi(this);
    g_wifiWg.languageReload();
    g_eth0Wg.languageReload();
    g_gpioWg.languageReload();
    g_voiceWg.languageReload();
    g_udevWg.languageReload();
    g_bluetoothWg.languageReload();
    g_allWg.languageReload();
    g_serialWg.languageReload();
    g_simModuleWg.languageReload();
    g_systemWg.languageReload();
    emit wifi_status_msg();
}

void MainWindow::on_btn_bluetooth_clicked()
{
    this->close();
    if(g_screenFlag == 0)
    {
        g_bluetoothWg.resize(g_screenWidth,g_screenHeight);
        g_bluetoothWg.show();
        g_bluetoothWg.activateWindow();g_bluetoothWg.setFocus();
    }
    else
    {
        if(g_bluetoothFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_bluetoothWg);
            w->setRotation(90);
            g_bluetoothView = new QGraphicsView(scene);
            g_bluetoothView->setWindowFlags(Qt::FramelessWindowHint);
            g_bluetoothView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_bluetoothView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_bluetoothView->resize(g_screenWidth,g_screenHeight);
            g_bluetoothWg.resize(g_screenHeight,g_screenWidth);
            g_bluetoothWg.show();
            g_bluetoothView->show();
            g_bluetoothView->activateWindow();g_bluetoothView->setFocus();
            g_bluetoothWg.activateWindow();g_bluetoothWg.setFocus();
            g_bluetoothFirstFlag++;
        }
        else
        {
            g_bluetoothWg.show();
            g_bluetoothView->show();
            g_bluetoothView->activateWindow();g_bluetoothView->setFocus();
            g_bluetoothWg.activateWindow();g_bluetoothWg.setFocus();
        }
    }
}

void MainWindow::on_btn_allTest_clicked()
{
    this->hide();
    if(g_screenFlag == 0)
    {
        g_allWg.resize(g_screenWidth,g_screenHeight);
        g_allWg.show();
        g_allWg.activateWindow();g_allWg.setFocus();
    }
    else
    {
        if(g_allTestFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_allWg);
            w->setRotation(90);
            g_allView = new QGraphicsView(scene);
            g_allView->setWindowFlags(Qt::FramelessWindowHint);
            g_allView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_allView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_allView->resize(g_screenWidth,g_screenHeight);
            g_allWg.resize(g_screenHeight,g_screenWidth);
            g_allWg.show();
            g_allView->show();
            g_allView->activateWindow();
            g_allView->setFocus();
            g_allWg.activateWindow();g_allWg.setFocus();
            g_allTestFirstFlag++;
        }
        else
        {
            g_allWg.show();
            g_allView->show();
            g_allView->activateWindow();
            g_allView->setFocus();
            g_allWg.activateWindow();g_allWg.setFocus();
        }
    }
}


void MainWindow::on_btn_serialPort_clicked()
{
    this->close();
    serialShow();
}

void MainWindow::on_btn_system_clicked()
{
    this->close();
    systemShow();
}

void MainWindow::on_btn_simModule_clicked()
{
    this->close();
    if(g_screenFlag == 0)
    {
        g_simModuleWg.resize(g_screenWidth,g_screenHeight);
        g_simModuleWg.show();
        g_simModuleWg.activateWindow();g_simModuleWg.setFocus();
    }
    else
    {
        if(g_simFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_simModuleWg);
            w->setRotation(90);

            g_simModuleView = new QGraphicsView(scene);
            g_simModuleView->setWindowFlags(Qt::FramelessWindowHint);
            g_simModuleView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_simModuleView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_simModuleView->resize(g_screenWidth,g_screenHeight);
            g_simModuleWg.resize(g_screenHeight,g_screenWidth);
            g_simModuleWg.show();
            g_simModuleView->show();
            g_simModuleView->activateWindow();g_simModuleView->setFocus();
            g_simModuleWg.activateWindow();g_simModuleWg.setFocus();
            g_simFirstFlag++;
        }
        else
        {
            g_simModuleWg.show();
            g_simModuleView->show();
            g_simModuleView->activateWindow();g_simModuleView->setFocus();
            g_simModuleWg.activateWindow();g_simModuleWg.setFocus();
        }
    }
}

void MainWindow::voice_back()
{
    if(g_screenFlag == 1)
    {
        g_voiceView->hide();
    }
    g_voiceWg.close();
    this->show();
}

void MainWindow::udev_back()
{
    if(g_screenFlag == 1)
    {
        g_udevView->hide();
    }
    g_udevWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::gpio_back()
{
    if(g_screenFlag == 1)
    {
        g_gpioView->hide();
    }
    g_gpioWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::touch_back()
{
    g_touchWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::wifi_back()
{
    if(g_screenFlag == 1)
    {
        g_wifiView->hide();
    }
    g_wifiWg.close();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::eth0_back()
{
    if(g_screenFlag == 1)
    {
        g_eth0View->hide();
    }
    g_eth0Wg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::all_back()
{
    if(g_screenFlag == 1)
    {
        g_allView->hide();
    }
    g_allWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::bluetooth_back()
{
    if(g_screenFlag == 1)
    {
        g_bluetoothView->hide();
    }
    g_bluetoothWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::serial_back()
{
    if(g_screenFlag == 1)
    {
        g_serialView->hide();
    }
    g_serialWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::sim_module_back()
{
    if(g_screenFlag == 1)
    {
        g_simModuleView->hide();
    }
    g_simModuleWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::sys_back()
{
    if(g_screenFlag == 1)
    {
        g_systemView->hide();
    }
    g_systemWg.close();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::setMainWindowFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realY = g_screen->physicalDotsPerInchY();
    qreal realWidth = g_screenWidth / realX * 2.54;
    qreal realHeight = g_screenHeight / realY *2.54;
    QFont font;
    if(g_screenFlag)
    {
        if(realHeight < 15)
        {
            font.setPointSize(13);
            ui->lbl_baijieTechnologyFunctionDisplay->setFont(font);
            font.setPointSize(12);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(16);
            ui->lbl_baijieTechnologyFunctionDisplay->setFont(font);
            font.setPointSize(15);
        }
        else
        {
            font.setPointSize(19);
            ui->lbl_baijieTechnologyFunctionDisplay->setFont(font);
            font.setPointSize(18);
        }
    }
    else
    {
        if(realWidth < 15)
        {
            font.setPointSize(13);
            ui->lbl_baijieTechnologyFunctionDisplay->setFont(font);
            font.setPointSize(12);
        }
        else if (realWidth < 17)
        {
            font.setPointSize(16);
            ui->lbl_baijieTechnologyFunctionDisplay->setFont(font);
            font.setPointSize(15);
        }
        else
        {
            font.setPointSize(19);
            ui->lbl_baijieTechnologyFunctionDisplay->setFont(font);
            font.setPointSize(18);
        }
    }

    ui->lbl_wifi->setFont(font);
    ui->lbl_bluetooth->setFont(font);
    ui->lbl_ethernet->setFont(font);
    ui->lbl_sim->setFont(font);
    ui->lbl_voice->setFont(font);
    ui->lbl_serial->setFont(font);
    ui->lbl_touch->setFont(font);
    ui->lbl_gpio->setFont(font);
    ui->lbl_allTest->setFont(font);
    ui->lbl_system->setFont(font);
    ui->lbl_udev->setFont(font);
}

void MainWindow::voiceShow()
{
    if(g_screenFlag == 0)
    {
        g_voiceWg.resize(g_screenWidth,g_screenHeight);
        g_voiceWg.show();
        g_voiceWg.activateWindow();g_voiceWg.setFocus();
    }
    else
    {
        if(g_voiceFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_voiceWg);
            w->setRotation(90);

            g_voiceView = new QGraphicsView(scene);

            g_voiceView->setWindowFlags(Qt::FramelessWindowHint);
            g_voiceView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_voiceView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            g_voiceView->resize(g_screenWidth,g_screenHeight);
            g_voiceWg.resize(g_screenHeight,g_screenWidth);
            g_voiceWg.show();
            g_voiceView->show();
            g_voiceView->activateWindow();
            g_voiceView->setFocus();
            g_voiceWg.activateWindow();
            g_voiceWg.setFocus();
            g_voiceFirstFlag++;
        }
        else
        {
            g_voiceWg.show();
            g_voiceView->show();
            g_voiceView->activateWindow();
            g_voiceView->setFocus();
            g_voiceWg.activateWindow();g_voiceWg.setFocus();
        }
    }
}

void MainWindow::serialShow()
{
    if(g_screenFlag == 0)
    {
        g_serialWg.resize(g_screenWidth,g_screenHeight);
        g_serialWg.show();
        g_serialWg.activateWindow();g_serialWg.setFocus();
    }
    else
    {
        if(g_serialFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_serialWg);
            w->setRotation(90);

            g_serialView = new QGraphicsView(scene);

            g_serialView->setWindowFlags(Qt::FramelessWindowHint);
            g_serialView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_serialView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            g_serialView->resize(g_screenWidth,g_screenHeight);
            g_serialWg.resize(g_screenHeight,g_screenWidth);
            g_serialWg.show();
            g_serialView->show();
            g_serialView->activateWindow();g_serialView->setFocus();
            g_serialWg.activateWindow();g_serialWg.setFocus();
            g_serialFirstFlag++;
        }
        else
        {
            g_serialWg.resize(g_screenHeight,g_screenWidth);
            g_serialWg.show();
            g_serialView->resize(g_screenWidth,g_screenHeight);
            g_serialView->show();
            g_serialView->activateWindow();g_serialView->setFocus();
            g_serialWg.activateWindow();g_serialWg.setFocus();
        }
    }
}

void MainWindow::ipsetShow()
{
    if(g_screenFlag == 0)
    {
        g_eth0Wg.resize(g_screenWidth,g_screenHeight);
        g_eth0Wg.show();
        g_eth0Wg.activateWindow();g_eth0Wg.setFocus();
    }
    else
    {
        if(g_eth0FirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_eth0Wg);
            w->setRotation(90);

            g_eth0View = new QGraphicsView(scene);

            g_eth0View->setWindowFlags(Qt::FramelessWindowHint);
            g_eth0View->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_eth0View->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            g_eth0View->resize(g_screenWidth,g_screenHeight);
            g_eth0Wg.resize(g_screenHeight,g_screenWidth);
            g_eth0Wg.show();
            g_eth0View->show();
            g_eth0View->activateWindow();
            g_eth0View->setFocus();
            g_eth0Wg.activateWindow();g_eth0Wg.setFocus();
            g_eth0FirstFlag++;
        }
        else
        {
            g_eth0Wg.resize(g_screenHeight,g_screenWidth);
            g_eth0Wg.show();
            g_eth0View->resize(g_screenWidth,g_screenHeight);
            g_eth0View->show();
            g_eth0View->activateWindow();
            g_eth0View->setFocus();
            g_eth0Wg.activateWindow();g_eth0Wg.setFocus();
        }
    }
}

void MainWindow::gpioShow()
{
    if(g_screenFlag == 0)
    {
        g_gpioWg.resize(g_screenWidth,g_screenHeight);
        g_gpioWg.show();
        g_gpioWg.activateWindow();g_gpioWg.setFocus();
    }
    else
    {
        if(g_gpioFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_gpioWg);
            w->setRotation(90);

            g_gpioView = new QGraphicsView(scene);

            g_gpioView->setWindowFlags(Qt::FramelessWindowHint);
            g_gpioView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_gpioView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            g_gpioView->resize(g_screenWidth,g_screenHeight);
            g_gpioWg.resize(g_screenHeight,g_screenWidth);
            g_gpioWg.show();
            g_gpioView->lower();
            g_gpioView->show();
            g_gpioView->activateWindow();
            g_gpioView->setFocus();
            g_gpioWg.activateWindow(); g_gpioWg.setFocus();
            g_gpioFirstFlag++;
        }
        else
        {
            g_gpioWg.show();
            g_gpioView->show();
            g_gpioView->activateWindow();
            g_gpioView->setFocus();
            g_gpioWg.activateWindow(); g_gpioWg.setFocus();
        }
    }
}

void MainWindow::systemShow()
{
    if(g_screenFlag == 0)
    {
        g_systemWg.resize(g_screenWidth,g_screenHeight);
        g_systemWg.show();
        g_systemWg.activateWindow();g_systemWg.setFocus();
    }
    else
    {
        if(g_sysFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_systemWg);
            w->setRotation(90);

            g_systemView = new QGraphicsView(scene);
            g_systemView->setWindowFlags(Qt::FramelessWindowHint);
            g_systemView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_systemView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            g_systemView->resize(g_screenWidth,g_screenHeight);
            g_systemWg.resize(g_screenHeight,g_screenWidth);
            g_systemWg.show();
            g_systemView->show();
            g_systemView->activateWindow();g_systemView->setFocus();
            g_systemWg.activateWindow();g_systemWg.setFocus();
            g_sysFirstFlag++;
        }
        else
        {
            g_systemWg.show();
            g_systemView->show();
            g_systemView->activateWindow();g_systemView->setFocus();
            g_systemWg.activateWindow();g_systemWg.setFocus();
        }
    }
}

void MainWindow::wifiShow()
{
    if(g_screenFlag == 0)
    {
        g_wifiWg.resize(g_screenWidth,g_screenHeight);
        g_wifiWg.show();
        g_wifiWg.activateWindow();g_wifiWg.setFocus();
    }
    else
    {
        if(g_wifiFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_wifiWg);
            w->setRotation(90);

            g_wifiView = new QGraphicsView(scene);

            g_wifiView->setWindowFlags(Qt::FramelessWindowHint);
            g_wifiView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_wifiView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            g_wifiView->resize(g_screenWidth,g_screenHeight);
            g_wifiWg.resize(g_screenHeight,g_screenWidth);
            g_wifiWg.show();
            g_wifiView->show();
            g_wifiView->activateWindow();
            g_wifiView->setFocus();
            g_wifiWg.activateWindow(); g_wifiWg.setFocus();
            g_wifiFirstFlag++;
        }
        else
        {
            g_wifiWg.show();
            g_wifiView->show();
            g_wifiView->activateWindow();
            g_wifiView->setFocus();
            g_wifiWg.activateWindow(); g_wifiWg.setFocus();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(g_showFirstFlag == 0)
    {
        wifiShow();wifi_back();
        serialShow();serial_back();
        ipsetShow();eth0_back();
        gpioShow();gpio_back();
        voiceShow();voice_back();
        systemShow();sys_back();
        g_showFirstFlag++;
    }
    QWidget::closeEvent(event);
}

void MainWindow::showEvent(QShowEvent *event)
{
    if(g_firstShowFlag == 0)
    {
        QStringList list = g_database.tableShow("language");
        if(!list.isEmpty())
        {
            if(list.at(0) == "Chinese")
            {
                emit cn_msg();
            }
        }
        g_firstShowFlag++;
    }
}
