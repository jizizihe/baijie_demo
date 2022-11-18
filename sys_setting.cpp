#include "sys_setting.h"
#include "ui_sys_setting.h"
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

static int g_screenFlag = 0;
static int g_screenWidth;
static int g_screenHeight;
static int g_sbacklightFirstFlag = 0;
static int g_timeSetFirstFlag = 0;
static int g_userFirstFlag = 0;
static int g_boardFirstFlag = 0;
static int g_showFirstFlag;
static QGraphicsView *g_backlightView;
static QGraphicsView *g_timeView;
static QGraphicsView *g_userView;
static QGraphicsView *g_boardView;
static QScreen *g_screen;

sys_setting::sys_setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sys_setting)
{
    ui->setupUi(this);

    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();

    if(g_screenWidth < g_screenHeight)
    {
        g_screenFlag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    setSystemFont();
    setBtnOtgValue();
    this->setAttribute(Qt::WA_StyledBackground,true);
    connect(&g_backlightWg,SIGNAL(backlight_back_msg()),this,SLOT(backlight_back()));
    connect(&g_timesetWg,SIGNAL(time_set_back_msg()),this,SLOT(time_back()));
    connect(&g_userWg,SIGNAL(user_manual_back_msg()),this,SLOT(user_back()));
    connect(&g_boardWg,SIGNAL(about_board_back_msg()),this,SLOT(board_back()));
}

sys_setting::~sys_setting()
{
    delete ui;
}

void sys_setting::on_btn_ret_clicked()
{
    emit sys_back_msg();
}

void sys_setting::on_btn_backlight_clicked()
{
    this->hide();
    if(g_screenFlag == 0)
    {
        g_backlightWg.resize(g_screenWidth,g_screenHeight);
        g_backlightWg.show();
    }
    else
    {
        if(g_sbacklightFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_backlightWg);
            w->setRotation(90);

            g_backlightView = new QGraphicsView(scene);
            g_backlightView->setWindowFlags(Qt::FramelessWindowHint);
            g_backlightView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_backlightView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_backlightView->resize(g_screenWidth,g_screenHeight);
            g_backlightWg.resize(g_screenHeight,g_screenWidth);
            g_backlightWg.show();
            g_backlightView->show();
            g_sbacklightFirstFlag++;
        }
        else
        {
            g_backlightWg.resize(g_screenHeight,g_screenWidth);
            g_backlightWg.show();
            g_backlightView->resize(g_screenWidth,g_screenHeight);
            g_backlightView->show();
        }
    }
}

void sys_setting::on_btn_RTC_clicked()
{
    this->hide();
    if(g_screenFlag == 0)
    {
        g_timesetWg.resize(g_screenWidth,g_screenHeight);
        g_timesetWg.show();
        g_timesetWg.activateWindow();g_timesetWg.setFocus();
    }
    else
    {
        if(g_timeSetFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_timesetWg);
            w->setRotation(90);

            g_timeView = new QGraphicsView(scene);
            g_timeView->setWindowFlags(Qt::FramelessWindowHint);
            g_timeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_timeView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_timeView->resize(g_screenWidth,g_screenHeight);
            g_timesetWg.resize(g_screenHeight,g_screenWidth);
            g_timesetWg.show();
            g_timeView->show();
            g_timeView->activateWindow();g_timeView->setFocus();
            g_timesetWg.activateWindow();g_timesetWg.setFocus();
            g_timeSetFirstFlag++;
        }
        else
        {
            g_timesetWg.resize(g_screenHeight,g_screenWidth);
            g_timesetWg.show();
            g_timeView->resize(g_screenWidth,g_screenHeight);
            g_timeView->show();
            g_timeView->activateWindow();g_timeView->setFocus();
            g_timesetWg.activateWindow();g_timesetWg.setFocus();
        }
    }
}

void sys_setting::on_btn_cn_clicked()
{
    emit main_cn_msg();                      //Send signals, translate all the interfaces of the system
}

void sys_setting::languageReload()
{
    ui->retranslateUi(this);
    g_timesetWg.languageReload();
    g_backlightWg.languageReload();
    g_boardWg.languageReload();
    g_userWg.languageReload();
}

void sys_setting::on_btn_user_clicked()
{
    this->hide();
    if(g_screenFlag == 0)
    {
        g_userWg.resize(g_screenWidth,g_screenHeight);
        g_userWg.show();
    }
    else
    {
        if(g_userFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_userWg);
            w->setRotation(90);

            g_userView = new QGraphicsView(scene);
            g_userView->setWindowFlags(Qt::FramelessWindowHint);
            g_userView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_userView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_userView->resize(g_screenWidth,g_screenHeight);
            g_userWg.resize(g_screenHeight,g_screenWidth);
            g_userWg.show();
            g_userView->show();
            g_userFirstFlag++;
        }
        else
        {
            g_userWg.resize(g_screenHeight,g_screenWidth);
            g_userWg.show();
            g_userView->resize(g_screenWidth,g_screenHeight);
            g_userView->show();
        }
    }
}

void sys_setting::on_btn_about_clicked()
{
    this->hide();
    if(g_screenFlag == 0)
    {
        g_boardWg.resize(g_screenWidth,g_screenHeight);
        g_boardWg.show();
    }
    else
    {
        if(g_boardFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_boardWg);
            w->setRotation(90);

            g_boardView = new QGraphicsView(scene);
            g_boardView->setWindowFlags(Qt::FramelessWindowHint);
            g_boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_boardView->resize(g_screenWidth,g_screenHeight);
            g_boardWg.resize(g_screenHeight,g_screenWidth);
            g_boardWg.show();
            g_boardView->show();
            g_boardFirstFlag++;
        }
        else
        {
            g_boardWg.resize(g_screenHeight,g_screenWidth);
            g_boardWg.show();
            g_boardView->resize(g_screenWidth,g_screenHeight);
            g_boardView->show();
        }
    }
}

void sys_setting::backlight_back()
{
    if(g_screenFlag == 1)
    {
        g_backlightView->hide();
    }
    g_backlightWg.hide();
    this->show();
}

void sys_setting::time_back()
{
    if(g_screenFlag == 1)
    {
        g_timeView->hide();
    }
    g_timesetWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void sys_setting::board_back()
{
    if(g_screenFlag == 1)
    {
        g_boardView->hide();
    }
    g_boardWg.hide();
    this->show();
}

void sys_setting::user_back()
{
    if(g_screenFlag == 1)
    {
        g_userView->hide();
    }
    g_userWg.hide();
    this->show();
}

void sys_setting::setSystemFont()
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
            font.setPointSize(12);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(14);
        }
        else
        {
            font.setPointSize(17);
        }
    }
    else
    {
        if(realWidth < 15)
        {
            font.setPointSize(12);
        }
        else if (realWidth < 17)
        {
            font.setPointSize(14);
        }
        else
        {
            font.setPointSize(17);
        }
    }
    ui->lbl_cn->setFont(font);
    ui->lbl_timeSet->setFont(font);
    ui->lbl_backlight->setFont(font);
    ui->lbl_aboutBoard->setFont(font);
    ui->lbl_userManual->setFont(font);
    ui->lbl_system->setFont(font);
    ui->lbl_OGT->setFont(font);
}

void sys_setting::closeEvent(QCloseEvent *event)
{
    if(g_showFirstFlag == 0)
    {
        on_btn_RTC_clicked();
        time_back();
        g_showFirstFlag++;
    }
    QWidget::closeEvent(event);
}

void sys_setting::on_btn_OTGHost_clicked()
{
    QString strOTGStatus = ui->lbl_OGT->text();
    if(strOTGStatus == tr("OTG Slave"))
    {
        setOtgHost();
    }
    else
    {
        setOtgSlave();
    }
}

void sys_setting::setOtgHost()
{
    QString strCmd = "cat /sys/bus/platform/drivers/otg\" \"manager/usbc0/usb_host";
    QProcess pro;
    pro.start("bash", QStringList() <<"-c" << strCmd);
    pro.waitForFinished(-1);
    QString strResult = pro.readAllStandardOutput();
    pro.close();
    if(!strResult.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("set successful!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(g_screenFlag == 1)
            mesg.move(g_screenWidth*2/3,g_screenHeight/3);
        else
            mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        ui->lbl_OGT->setText(tr("OTG Host"));
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("set failed!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(g_screenFlag == 1)
            mesg.move(g_screenWidth*2/3,g_screenHeight/3);
        else
            mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
    }
}

void sys_setting::setOtgSlave()
{
    QString strCmd = "cat /sys/bus/platform/drivers/otg\" \"manager/usbc0/usb_device";
    QProcess pro;
    pro.start("bash", QStringList() <<"-c" << strCmd);
    pro.waitForFinished(-1);
    QString strResult = pro.readAllStandardOutput();
    pro.close();
    if(!strResult.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("set successful!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(g_screenFlag == 1)
            mesg.move(g_screenWidth*2/3,g_screenHeight/3);
        else
            mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        ui->lbl_OGT->setText(tr("OTG Slave"));
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("set failed!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(g_screenFlag == 1)
            mesg.move(g_screenWidth*2/3,g_screenHeight/3);
        else
            mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
    }
}

void sys_setting::setBtnOtgValue()
{
    QString strCmd = "cat /sys/bus/platform/drivers/otg\" \"manager/usbc0/otg_role";
    QProcess pro;
    pro.start("bash", QStringList() <<"-c" << strCmd);
    pro.waitForFinished(-1);
    QString strResult = pro.readAllStandardOutput();
    pro.close();
    if(strResult.contains("device",Qt::CaseInsensitive))
    {
        ui->lbl_OGT->setText(tr("OTG Slave"));
    }
    else
    {
        ui->lbl_OGT->setText(tr("OTG Host"));
    }
}
