#include "timeset.h"
#include "ui_timeset.h"
#include <QScreen>

static int g_screenWidth;
static int g_screenHeight;
static int g_screenFlag;
static int g_showFirstFlag;
static int g_syncNetworkFlag;      // 0: Async Network   1: Sync Network
static qreal g_realX;
static qreal g_realY;

timeset::timeset(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::timeset)
{
    ui->setupUi(this);
    QScreen *g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();

    if(g_screenWidth < g_screenHeight)
    {
        g_screenFlag = 1;
        ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }

    g_realX = g_screen->physicalDotsPerInchX();
    g_realY = g_screen->physicalDotsPerInchY();
    setTimeFont();
    ui->datetime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->datetime->setTime(QTime::currentTime());
    ui->datetime->setDate(QDate::currentDate());

    g_sysTimer = new QTimer(this);
    connect(g_sysTimer,SIGNAL(timeout()),this,SLOT(sys_timer_update()));
    g_rtcTimer = new QTimer(this);
    connect(g_rtcTimer,SIGNAL(timeout()),this,SLOT(rtc_timer_update()));
    g_proSys.start("bash");
    g_proRTC.start("bash");

    rtc_timer_update();
    sys_timer_update();
    connect(ui->btn_ret,SIGNAL(clicked(bool)),this,SLOT(btn_ret_clicked()));
    connect(ui->btn_sysTimeSet,SIGNAL(clicked()),this,SLOT(btn_sysTimeSet_clicked()));
    connect(ui->btn_RTCSet,SIGNAL(clicked()),this,SLOT(btn_RTCSet_clicked()));
    isSyncNetwork();             //Whether the time is synchronized with the network
}

timeset::~timeset()
{
    delete ui;
    g_proSys.close();
    g_proRTC.close();
}

void timeset::sys_timer_update(void)
{
    g_proSys.write("date +'%Y-%m-%d %H:%M:%S' \n");
    QString strResult = g_proSys.readAllStandardOutput();strResult.remove("\n");
    ui->lbl_systimeValue->setText(strResult);
}

void timeset::rtc_timer_update(void)
{
    g_proRTC.write("hwclock -r \n");
    QString strResult = g_proRTC.readAllStandardOutput();
    ui->lbl_RTCtimeValue->setText(strResult.mid(0,19));
}

void timeset::btn_ret_clicked()
{
    emit time_set_back_msg();
    g_sysTimer->stop();
    g_rtcTimer->stop();
}

QString timeset::setSystime(QString  dateText)
{
    QString strCmd = QString("date -s \"%1\"").arg(dateText);
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    strCmd = QString("echo $?");
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    process.close();
    return strResult;
}

void timeset::btn_sysTimeSet_clicked()
{
    if(g_syncNetworkFlag == 1)
    {
        QMessageBox::information(this,"information",tr("The current time is synchronized with the network time,you can click the 'Async Network' button!"));
        return;
    }
    g_sysTimer->stop();
    QString  datetext = ui->datetime->text();
    QString strResult = setSystime(datetext);
    if(strResult == "0\n")
    {
        QMessageBox::information(this,"information",tr("Systime set ok!"));
    }
    else
    {
        QMessageBox::critical(this,"information",tr("Systime set failed!"));
    }
    g_sysTimer->start(1000);
}

QString timeset::setRTC()
{
    QString strCmd = QString("hwclock -w");
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    strCmd = QString("echo $?");
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();
    process.close();
    return strResult;
}

void timeset::btn_RTCSet_clicked()
{
    g_rtcTimer->stop();
    QString strResult = setRTC();
    if(strResult == "0\n")
    {
        QMessageBox::information(this,"information",tr("RTC set ok!"));
    }
    else
    {
        QMessageBox::critical(this,"information",tr("RTC set failed!"));
    }
    g_rtcTimer->start(1000);
}

void timeset::showEvent(QShowEvent *event)
{
    if(g_showFirstFlag==0)
    {
        g_showFirstFlag++;
    }
    else
    {
        sys_timer_update();
        rtc_timer_update();
        g_sysTimer->start(1000);
        g_rtcTimer->start(1000);
    }
}

void timeset::languageReload()
{
    ui->retranslateUi(this);
}

void timeset::setTimeFont()
{
    qreal realWidth = g_screenWidth / g_realX * 2.54;
    qreal realHeight = g_screenHeight / g_realY *2.54;
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
    ui->datetime->setFont(font);
    ui->lbl_RTCtimeValue->setFont(font);
    ui->btn_RTCSet->setFont(font);
    ui->lbl_RTCtime->setFont(font);
    ui->lbl_systimeValue->setFont(font);
    ui->lbl_systime->setFont(font);
    ui->btn_sysTimeSet->setFont(font);
    ui->lbl_timeSet->setFont(font);
    ui->btn_sync->setFont(font);
    ui->lbl_setTime->setFont(font);
}

void timeset::isSyncNetwork()
{
    QString strCmd = QString("systemctl status systemd-timesyncd.service | grep Active |awk '{print $2}'");
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();
    if(strResult == "inactive\n")
    {
        ui->btn_sync->setText(tr("Sync Network"));
        g_syncNetworkFlag = 0;
    }
    else
    {
        ui->btn_sync->setText(tr("Async Network"));
        g_syncNetworkFlag = 1;
    }
    process.close();
}

void timeset::on_btn_sync_clicked()
{
    if(ui->btn_sync->text() == tr("Sync Network"))
    {
        setSyncNetwork();
    }
    else
    {
        setAsyncNetwork();
    }
}

void timeset::setSyncNetwork()
{
    QString strCmd = QString("systemctl start systemd-timesyncd.service");
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    process.close();
    ui->btn_sync->setText(tr("Async Network"));
    QMessageBox::information(this,"information",tr("The system time is successfully synchronized with the network time!"));
    g_syncNetworkFlag = 1;
}

void timeset::setAsyncNetwork()
{
    QString strCmd = QString("systemctl stop systemd-timesyncd.service");
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    process.close();
    ui->btn_sync->setText(tr("Sync Network"));
    QMessageBox::information(this,"information",tr("System time successfully canceled synchronization of network time!"));
    g_syncNetworkFlag = 0;
}
