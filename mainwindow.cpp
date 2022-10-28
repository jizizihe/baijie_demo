#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/stat.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>

static int screenFlag = 0;                    // 0: width > height  1: width < height
static int screenWidth;
static int screenHeight;
static int voiceFirstFlag = 0;                // 0: First time display   1: Not for the first time
static int udevFirstFlag = 0;
static int gpioFirstFlag = 0;
static int wifiFirstFlag = 0;
static int eth0FirstFlag = 0;
static int allTestFirstFlag = 0;
static int bluetoothFirstFlag = 0;
static int serialFirstFlag = 0;
static int simFirstFlag = 0;
static int sysFirstFlag = 0;

QGraphicsView *voiceView;
QGraphicsView *udevView;
QGraphicsView *gpioView;
QGraphicsView *wifiView;
QGraphicsView *eth0View;
QGraphicsView *allView;
QGraphicsView *bluetoothView;
QGraphicsView *serialView;
QGraphicsView *simModuleView;
QGraphicsView *systemView;

static QScreen *screen;
static int showFirstFlag;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    screen = qApp->primaryScreen();                      // Width and height of the screen
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;
    }
    else
    {
        QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
        shadowEffect->setOffset(4, 4);
        shadowEffect->setColor(QColor (136, 138, 133));
        shadowEffect->setBlurRadius(5);
        ui->lbl_baijieTechnologyFunctionDisplay->setGraphicsEffect(shadowEffect);      //Add shadow to font
    }
    mainFont();
    connect(&voiceWg,SIGNAL(voice_back_msg()),this,SLOT(voice_back()));
    connect(&udevWg,SIGNAL(udev_back_msg()),this,SLOT(udev_back()));
    connect(&gpioWg,SIGNAL(gpio_back_msg()),this,SLOT(gpio_back()));
    connect(&touchWg,SIGNAL(touch_back_msg()),this,SLOT(touch_back()));
    connect(this,SIGNAL(wifi_status_msg()),&wifiWg,SLOT(wifi_status_show()));
    connect(&wifiWg,SIGNAL(wifi_back_msg()),this,SLOT(wifi_back()));
    connect(&eth0Wg,SIGNAL(ipset_back_msg()),this,SLOT(eth0_back()));
    connect(&allWg,SIGNAL(all_test_back_msg()),this,SLOT(all_back()));
    connect(&bluetoothWg,SIGNAL(bluetooth_back_msg()),this,SLOT(bluetooth_back()));
    connect(&serialWg,SIGNAL(serial_back_msg()),this,SLOT(serial_back()));
    connect(&systemWg,SIGNAL(sys_back_msg()),this,SLOT(sys_back()));
    connect(&systemWg,SIGNAL(main_cn_msg()),this,SLOT(cn_main()));
    connect(&simModuleWg,SIGNAL(sim_back_msg()),this,SLOT(sim_module_back()));

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
    if(screenFlag == 0)
    {
        udevWg.resize(screenWidth,screenHeight);
        udevWg.show();
        udevWg.activateWindow();udevWg.setFocus();
    }
    else                                                        //width < height: Rotate 90 degrees
    {
        if(udevFirstFlag == 0)                                  //the interface is displayed for the first time, the interface need to initialize a rotation of 90
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&udevWg);
            w->setRotation(90);
            udevView = new QGraphicsView(scene);
            udevView->setWindowFlags(Qt::FramelessWindowHint);
            udevView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            udevView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            udevView->resize(screenWidth,screenHeight);
            udevWg.resize(screenHeight,screenWidth);
            udevWg.show();
            udevView->show();
            udevView->activateWindow();udevView->setFocus();
            udevWg.activateWindow();udevWg.setFocus();
            udevFirstFlag++;
        }
        else
        {
            udevWg.show();
            udevView->show();
            udevView->activateWindow();udevView->setFocus();
            udevWg.activateWindow();udevWg.setFocus();
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
    touchWg.resize(screenWidth,screenHeight);
    touchWg.show();
    touchWg.activateWindow();touchWg.setFocus();
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
    languageFlag = !languageFlag;
    ui->retranslateUi(this);
    wifiWg.languageReload();
    eth0Wg.languageReload();
    gpioWg.languageReload();
    voiceWg.languageReload();
    udevWg.languageReload();
    bluetoothWg.languageReload();
    allWg.languageReload();
    serialWg.languageReload();
    simModuleWg.languageReload();
    emit wifi_status_msg();
}

void MainWindow::on_btn_bluetooth_clicked()
{
    this->close();
    if(screenFlag == 0)
    {
        bluetoothWg.resize(screenWidth,screenHeight);
        bluetoothWg.show();
        bluetoothWg.activateWindow();bluetoothWg.setFocus();
    }
    else
    {
        if(bluetoothFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&bluetoothWg);
            w->setRotation(90);
            bluetoothView = new QGraphicsView(scene);
            bluetoothView->setWindowFlags(Qt::FramelessWindowHint);
            bluetoothView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            bluetoothView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            bluetoothView->resize(screenWidth,screenHeight);
            bluetoothWg.resize(screenHeight,screenWidth);
            bluetoothWg.show();
            bluetoothView->show();
            bluetoothView->activateWindow();bluetoothView->setFocus();
            bluetoothWg.activateWindow();bluetoothWg.setFocus();
            bluetoothFirstFlag++;
        }
        else
        {
            bluetoothWg.show();
            bluetoothView->show();
            bluetoothView->activateWindow();bluetoothView->setFocus();
            bluetoothWg.activateWindow();bluetoothWg.setFocus();
        }
    }
}

void MainWindow::on_btn_allTest_clicked()
{
    this->hide();
    if(screenFlag == 0)
    {
        allWg.resize(screenWidth,screenHeight);
        allWg.show();
        allWg.activateWindow();allWg.setFocus();
    }
    else
    {
        if(allTestFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&allWg);
            w->setRotation(90);
            allView = new QGraphicsView(scene);
            allView->setWindowFlags(Qt::FramelessWindowHint);
            allView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            allView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            allView->resize(screenWidth,screenHeight);
            allWg.resize(screenHeight,screenWidth);
            allWg.show();
            allView->show();
            allView->activateWindow();allView->setFocus();
            allWg.activateWindow();allWg.setFocus();
            allTestFirstFlag++;
        }
        else
        {
            allWg.show();
            allView->show();
            allView->activateWindow();allView->setFocus();
            allWg.activateWindow();allWg.setFocus();
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
    if(screenFlag == 0)
    {
        simModuleWg.resize(screenWidth,screenHeight);
        simModuleWg.show();
        simModuleWg.activateWindow();simModuleWg.setFocus();
    }
    else
    {
        if(simFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&simModuleWg);
            w->setRotation(90);

            simModuleView = new QGraphicsView(scene);
            simModuleView->setWindowFlags(Qt::FramelessWindowHint);
            simModuleView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            simModuleView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            simModuleView->resize(screenWidth,screenHeight);
            simModuleWg.resize(screenHeight,screenWidth);
            simModuleWg.show();
            simModuleView->show();
            simModuleView->activateWindow();simModuleView->setFocus();
            simModuleWg.activateWindow();simModuleWg.setFocus();
            simFirstFlag++;
        }
        else
        {
            simModuleWg.show();
            simModuleView->show();
            simModuleView->activateWindow();simModuleView->setFocus();
            simModuleWg.activateWindow();simModuleWg.setFocus();
        }
    }
}

void MainWindow::voice_back()
{
    if(screenFlag == 1)
    {
        voiceView->hide();
    }
    voiceWg.close();
    this->show();
}

void MainWindow::udev_back()
{
    if(screenFlag == 1)
    {
        udevView->hide();
    }
    udevWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::gpio_back()
{
    if(screenFlag == 1)
    {
        gpioView->hide();
    }
    gpioWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::touch_back()
{
    touchWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::wifi_back()
{
    if(screenFlag == 1)
    {
        wifiView->hide();
    }
    wifiWg.close();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::eth0_back()
{
    if(screenFlag == 1)
    {
        eth0View->hide();
    }
    eth0Wg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::all_back()
{
    if(screenFlag == 1)
    {
        allView->hide();
    }
    allWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::bluetooth_back()
{
    if(screenFlag == 1)
    {
        bluetoothView->hide();
    }
    bluetoothWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::serial_back()
{
    if(screenFlag == 1)
    {
        serialView->hide();
    }
    serialWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::sim_module_back()
{
    if(screenFlag == 1)
    {
        simModuleView->hide();
    }
    simModuleWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::sys_back()
{
    if(screenFlag == 1)
    {
        systemView->hide();
    }
    systemWg.close();
    this->show();
    this->activateWindow();this->setFocus();
}

void MainWindow::mainFont()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = screenWidth / realX * 2.54;
    qreal realHeight = screenHeight / realY *2.54;
    QFont font;
    if(screenFlag)
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
    if(screenFlag == 0)
    {
        voiceWg.resize(screenWidth,screenHeight);
        voiceWg.show();
        voiceWg.activateWindow();voiceWg.setFocus();
    }
    else
    {
        if(voiceFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&voiceWg);
            w->setRotation(90);

            voiceView = new QGraphicsView(scene);

            voiceView->setWindowFlags(Qt::FramelessWindowHint);
            voiceView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            voiceView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            voiceView->resize(screenWidth,screenHeight);
            voiceWg.resize(screenHeight,screenWidth);
            voiceWg.show();
            voiceView->show();
            voiceView->activateWindow();voiceView->setFocus();
            voiceWg.activateWindow();voiceWg.setFocus();
            voiceFirstFlag++;
        }
        else
        {
            voiceWg.show();
            voiceView->show();
            voiceView->activateWindow();voiceView->setFocus();
            voiceWg.activateWindow();voiceWg.setFocus();
        }
    }
}

void MainWindow::serialShow()
{
    if(screenFlag == 0)
    {
        serialWg.resize(screenWidth,screenHeight);
        serialWg.show();
        serialWg.activateWindow();serialWg.setFocus();
    }
    else
    {
        if(serialFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&serialWg);
            w->setRotation(90);

            serialView = new QGraphicsView(scene);

            serialView->setWindowFlags(Qt::FramelessWindowHint);
            serialView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            serialView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            serialView->resize(screenWidth,screenHeight);
            serialWg.resize(screenHeight,screenWidth);
            serialWg.show();
            serialView->show();
            serialView->activateWindow();serialView->setFocus();
            serialWg.activateWindow();serialWg.setFocus();
            serialFirstFlag++;
        }
        else
        {
            serialWg.resize(screenHeight,screenWidth);
            serialWg.show();
            serialView->resize(screenWidth,screenHeight);
            serialView->show();
            serialView->activateWindow();serialView->setFocus();
            serialWg.activateWindow();serialWg.setFocus();
        }
    }
}

void MainWindow::ipsetShow()
{
    if(screenFlag == 0)
    {
        eth0Wg.resize(screenWidth,screenHeight);
        eth0Wg.show();
        eth0Wg.activateWindow();eth0Wg.setFocus();
    }
    else
    {
        if(eth0FirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&eth0Wg);
            w->setRotation(90);

            eth0View = new QGraphicsView(scene);

            eth0View->setWindowFlags(Qt::FramelessWindowHint);
            eth0View->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            eth0View->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            eth0View->resize(screenWidth,screenHeight);
            eth0Wg.resize(screenHeight,screenWidth);
            eth0Wg.show();
            eth0View->show();
            eth0View->activateWindow();eth0View->setFocus();
            eth0Wg.activateWindow();eth0Wg.setFocus();
            eth0FirstFlag++;
        }
        else
        {
            eth0Wg.resize(screenHeight,screenWidth);
            eth0Wg.show();
            eth0View->resize(screenWidth,screenHeight);
            eth0View->show();
            eth0View->activateWindow();eth0View->setFocus();
            eth0Wg.activateWindow();eth0Wg.setFocus();
        }
    }
}

void MainWindow::gpioShow()
{
    if(screenFlag == 0)
    {
        gpioWg.resize(screenWidth,screenHeight);
        gpioWg.show();
        gpioWg.activateWindow();gpioWg.setFocus();
    }
    else
    {
        if(gpioFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&gpioWg);
            w->setRotation(90);

            gpioView = new QGraphicsView(scene);

            gpioView->setWindowFlags(Qt::FramelessWindowHint);
            gpioView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            gpioView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            gpioView->resize(screenWidth,screenHeight);
            gpioWg.resize(screenHeight,screenWidth);
            gpioWg.show();
            gpioView->lower();
            gpioView->show();
            gpioView->activateWindow();gpioView->setFocus();
            gpioWg.activateWindow(); gpioWg.setFocus();
            gpioFirstFlag++;
        }
        else
        {
            gpioWg.show();
            gpioView->show();
            gpioView->activateWindow();gpioView->setFocus();
            gpioWg.activateWindow(); gpioWg.setFocus();
        }
    }
}

void MainWindow::systemShow()
{
    if(screenFlag == 0)
    {
        systemWg.resize(screenWidth,screenHeight);
        systemWg.show();
        systemWg.activateWindow();systemWg.setFocus();
    }
    else
    {
        if(sysFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&systemWg);
            w->setRotation(90);

            systemView = new QGraphicsView(scene);
            systemView->setWindowFlags(Qt::FramelessWindowHint);
            systemView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            systemView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            systemView->resize(screenWidth,screenHeight);
            systemWg.resize(screenHeight,screenWidth);
            systemWg.show();
            systemView->show();
            systemView->activateWindow();systemView->setFocus();
            systemWg.activateWindow();systemWg.setFocus();
            sysFirstFlag++;
        }
        else
        {
            systemWg.show();
            systemView->show();
            systemView->activateWindow();systemView->setFocus();
            systemWg.activateWindow();systemWg.setFocus();
        }
    }
}

void MainWindow::wifiShow()
{
    if(screenFlag == 0)
    {
        wifiWg.resize(screenWidth,screenHeight);
        wifiWg.show();
        wifiWg.activateWindow();wifiWg.setFocus();
    }
    else
    {
        if(wifiFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&wifiWg);
            w->setRotation(90);

            wifiView = new QGraphicsView(scene);

            wifiView->setWindowFlags(Qt::FramelessWindowHint);
            wifiView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            wifiView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            wifiView->resize(screenWidth,screenHeight);
            wifiWg.resize(screenHeight,screenWidth);
            wifiWg.show();
            wifiView->show();
            wifiView->activateWindow();wifiView->setFocus();
            wifiWg.activateWindow(); wifiWg.setFocus();
            wifiFirstFlag++;
        }
        else
        {
            wifiWg.show();
            wifiView->show();
            wifiView->activateWindow();wifiView->setFocus();
            wifiWg.activateWindow(); wifiWg.setFocus();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(showFirstFlag == 0)
    {
        wifiShow();wifi_back();
        serialShow();serial_back();
        ipsetShow();eth0_back();
        gpioShow();gpio_back();
        voiceShow();voice_back();
        systemShow();sys_back();
        showFirstFlag++;
    }
    QWidget::closeEvent(event);
}
