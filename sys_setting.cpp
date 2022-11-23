#include "sys_setting.h"
#include "ui_sys_setting.h"

static int g_screenWidth;
static int g_screenHeight;
static QScreen *g_screen;

sys_setting::sys_setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sys_setting)
{
    ui->setupUi(this);

    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();
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
    g_backlightWg.resize(g_screenWidth,g_screenHeight);
    g_backlightWg.show();
}

void sys_setting::on_btn_RTC_clicked()
{
    this->hide();
    g_timesetWg.resize(g_screenWidth,g_screenHeight);
    g_timesetWg.show();
    g_timesetWg.activateWindow();
}

void sys_setting::on_btn_cn_clicked()
{
    emit main_cn_msg();                      // Send signals, translate all the interfaces of the system
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
    g_userWg.resize(g_screenWidth,g_screenHeight);
    g_userWg.show();
}

void sys_setting::on_btn_about_clicked()
{
    this->hide();
    g_boardWg.resize(g_screenWidth,g_screenHeight);
    g_boardWg.show();
}

void sys_setting::backlight_back()
{
    g_backlightWg.hide();
    this->show();
}

void sys_setting::time_back()
{
    g_timesetWg.hide();
    this->show();
    this->activateWindow();
}

void sys_setting::board_back()
{
    g_boardWg.hide();
    this->show();
}

void sys_setting::user_back()
{
    g_userWg.hide();
    this->show();
}

void sys_setting::setSystemFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;
    if(realWidth < 15)
    {
        font.setPointSize(12);
    }
    else if (realWidth < 18)
    {
        font.setPointSize(14);
    }
    else
    {
        font.setPointSize(17);
    }
    ui->lbl_cn->setFont(font);
    ui->lbl_timeSet->setFont(font);
    ui->lbl_backlight->setFont(font);
    ui->lbl_aboutBoard->setFont(font);
    ui->lbl_userManual->setFont(font);
    ui->lbl_system->setFont(font);
    ui->lbl_OGT->setFont(font);
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
                         tr("Set successful!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        ui->lbl_OGT->setText(tr("OTG Host"));
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Set failed!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
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
                         tr("Set successful!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        ui->lbl_OGT->setText(tr("OTG Slave"));
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Set failed!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
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

