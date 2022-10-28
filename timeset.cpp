#include "timeset.h"
#include "ui_timeset.h"
#include <QScreen>

static int screenWidth;
static int screenHeight;
static int screenFlag;
static int showFirstFlag;
static int syncNetworkFlag;      // 0: Async Network   1: Sync Network
static qreal realX;
static qreal realY;

timeset::timeset(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::timeset)
{
    ui->setupUi(this);
    QScreen *screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }

    realX = screen->physicalDotsPerInchX();
    realY = screen->physicalDotsPerInchY();
    timesetFont();
    ui->datetime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->datetime->setTime(QTime::currentTime());
    ui->datetime->setDate(QDate::currentDate());

    SysTimer = new QTimer(this);
    connect(SysTimer,SIGNAL(timeout()),this,SLOT(sys_timer_update()));
    RTCTimer = new QTimer(this);
    connect(RTCTimer,SIGNAL(timeout()),this,SLOT(RTC_timer_update()));
    proSys.start("bash");
    proRTC.start("bash");

    RTC_timer_update();
    sys_timer_update();
    connect(ui->btn_ret,SIGNAL(clicked(bool)),this,SLOT(btn_ret_clicked()));
    connect(ui->btn_sysTimeSet,SIGNAL(clicked()),this,SLOT(btn_sysTimeSet_clicked()));
    connect(ui->btn_RTCSet,SIGNAL(clicked()),this,SLOT(btn_RTCSet_clicked()));
    isSyncNetwork();             //Whether the time is synchronized with the network
}

timeset::~timeset()
{
    delete ui;
    proSys.close();
    proRTC.close();
}

void timeset::sys_timer_update(void)
{
    proSys.write("date +'%Y-%m-%d %H:%M:%S' \n");
    QString strResult = proSys.readAllStandardOutput();strResult.remove("\n");
    ui->lbl_systimeValue->setText(strResult);
}

void timeset::RTC_timer_update(void)
{
    proRTC.write("hwclock -r \n");
    QString strResult = proRTC.readAllStandardOutput();
    ui->lbl_RTCtimeValue->setText(strResult.mid(0,19));
}

void timeset::btn_ret_clicked()
{
    emit time_set_back_msg();
    SysTimer->stop();
    RTCTimer->stop();
}

QString timeset::SystimeSet(QString  dateText)
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
    if(syncNetworkFlag == 1)
    {
        QMessageBox::information(this,"information",tr("The current time is synchronized with the network time,you can click the 'Async Network' button!"));
        return;
    }
    SysTimer->stop();
    QString  datetext = ui->datetime->text();
    QString strResult = SystimeSet(datetext);
    if(strResult == "0\n")
    {
        QMessageBox::information(this,"information",tr("Systime set ok!"));
    }
    else
    {
        QMessageBox::critical(this,"information",tr("Systime set failed!"));
    }
    SysTimer->start(1000);
}

QString timeset::RTCSet()
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
    RTCTimer->stop();
    QString strResult = RTCSet();
    if(strResult == "0\n")
    {
        QMessageBox::information(this,"information",tr("RTC set ok!"));
    }
    else
    {
        QMessageBox::critical(this,"information",tr("RTC set failed!"));
    }
    RTCTimer->start(1000);
}

void timeset::showEvent(QShowEvent *event)
{
    if(showFirstFlag==0)
    {
        showFirstFlag++;
    }
    else
    {
        sys_timer_update();
        RTC_timer_update();
        SysTimer->start(1000);
        RTCTimer->start(1000);
    }
}

void timeset::languageReload()
{
    ui->retranslateUi(this);
}

void timeset::timesetFont()
{
    qreal realWidth = screenWidth / realX * 2.54;
    qreal realHeight = screenHeight / realY *2.54;
    QFont font;
    if(screenFlag)
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
        syncNetworkFlag = 0;
    }
    else
    {
        ui->btn_sync->setText(tr("Async Network"));
        syncNetworkFlag = 1;
    }
    process.close();
}

void timeset::on_btn_sync_clicked()
{
    if(ui->btn_sync->text() == tr("Sync Network"))
    {
        QString strCmd = QString("systemctl start systemd-timesyncd.service");
        QProcess process;
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished();
        process.close();
        ui->btn_sync->setText(tr("Async Network"));
        QMessageBox::information(this,"information",tr("The system time is successfully synchronized with the network time!"));
        syncNetworkFlag = 1;
    }
    else
    {
        QString strCmd = QString("systemctl stop systemd-timesyncd.service");
        QProcess process;
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished();
        process.close();
        ui->btn_sync->setText(tr("Sync Network"));
        QMessageBox::information(this,"information",tr("System time successfully canceled synchronization of network time!"));
        syncNetworkFlag = 0;
    }
}
