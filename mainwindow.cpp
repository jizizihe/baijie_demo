#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/stat.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>

static int g_screenWidth;
static int g_screenHeight;
static int g_firstShowFlag = 0;
static int g_showFirstFlag;   //0:The MainWindow is displayed for the first time
static QScreen *g_screen;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    g_screen = qApp->primaryScreen();                      // Width and height of thg_e g_screen
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(4, 4);
    shadowEffect->setColor(QColor (136, 138, 133));
    shadowEffect->setBlurRadius(5);
    ui->lbl_baijieTechnologyFunctionDisplay->setGraphicsEffect(shadowEffect);      //Add shadow to font

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
    g_udevWg.resize(g_screenWidth,g_screenHeight);
    g_udevWg.show();
    g_udevWg.activateWindow();g_udevWg.setFocus();
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
    g_bluetoothWg.resize(g_screenWidth,g_screenHeight);
    g_bluetoothWg.show();
    g_bluetoothWg.activateWindow();g_bluetoothWg.setFocus();
}

void MainWindow::on_btn_allTest_clicked()
{
    this->hide();
    g_allWg.resize(g_screenWidth,g_screenHeight);
    g_allWg.show();
    g_allWg.activateWindow();g_allWg.setFocus();
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
    g_simModuleWg.resize(g_screenWidth,g_screenHeight);
    g_simModuleWg.show();
    g_simModuleWg.activateWindow();g_simModuleWg.setFocus();
}

void MainWindow::voice_back()
{
    g_voiceWg.close();
    this->show();
}

void MainWindow::udev_back()
{
    g_udevWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::gpio_back()
{
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
    g_wifiWg.close();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::eth0_back()
{
    g_eth0Wg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::all_back()
{
    g_allWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::bluetooth_back()
{
    g_bluetoothWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::serial_back()
{
    g_serialWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::sim_module_back()
{
    g_simModuleWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::sys_back()
{
    g_systemWg.close();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::setMainWindowFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;

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
    g_voiceWg.resize(g_screenWidth,g_screenHeight);
    g_voiceWg.show();
    g_voiceWg.activateWindow();g_voiceWg.setFocus();
}

void MainWindow::serialShow()
{
    g_serialWg.resize(g_screenWidth,g_screenHeight);
    g_serialWg.show();
    g_serialWg.activateWindow();g_serialWg.setFocus();
}

void MainWindow::ipsetShow()
{
    g_eth0Wg.resize(g_screenWidth,g_screenHeight);
    g_eth0Wg.show();
    g_eth0Wg.activateWindow();g_eth0Wg.setFocus();
}

void MainWindow::gpioShow()
{
    g_gpioWg.resize(g_screenWidth,g_screenHeight);
    g_gpioWg.show();
    g_gpioWg.activateWindow();g_gpioWg.setFocus();
}

void MainWindow::systemShow()
{
    g_systemWg.resize(g_screenWidth,g_screenHeight);
    g_systemWg.show();
    g_systemWg.activateWindow();g_systemWg.setFocus();
}

void MainWindow::wifiShow()
{
    g_wifiWg.resize(g_screenWidth,g_screenHeight);
    g_wifiWg.show();
    g_wifiWg.activateWindow();g_wifiWg.setFocus();
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
