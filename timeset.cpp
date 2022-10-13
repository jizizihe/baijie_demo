#include "timeset.h"
#include "ui_timeset.h"
#include <QScreen>

static int s_width;
static int s_height;
static int screen_flag;
static int show_first;
static qreal realX;
static qreal realY;
static int sync_flag;

timeset::timeset(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::timeset)
{
    ui->setupUi(this);
    QScreen *screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();

    if(s_width < s_height)
    {
        screen_flag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }

    realX = screen->physicalDotsPerInchX();
    realY = screen->physicalDotsPerInchY();
    timeset_font();
    ui->datetime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->datetime->setTime(QTime::currentTime());
    ui->datetime->setDate(QDate::currentDate());

    SysTimer = new QTimer(this);
    connect(SysTimer,SIGNAL(timeout()),this,SLOT(SystimerUpdate()));
    RTCTimer = new QTimer(this);
    connect(RTCTimer,SIGNAL(timeout()),this,SLOT(RTCtimerUpdate()));
    pro_sys.start("bash");
    pro_rtc.start("bash");

    RTCtimerUpdate();
    SystimerUpdate();
    connect(ui->retBt,SIGNAL(clicked(bool)),this,SLOT(retBt_clicked()));
    connect(ui->SystimeSetBt,SIGNAL(clicked()),this,SLOT(SystimeSetBt_clicked()));
    connect(ui->RTCSetBt,SIGNAL(clicked()),this,SLOT(RTCSetBt_clicked()));
    sync_net();
}

timeset::~timeset()
{
    delete ui;
    pro_sys.close();
    pro_rtc.close();
}

void timeset::timerUpdate()
{
    RTCtimerUpdate();
    SystimerUpdate();
}

void timeset::SystimerUpdate(void)
{
    pro_sys.write("date +'%Y-%m-%d %H:%M:%S' \n");
    QString strResult = pro_sys.readAllStandardOutput();strResult.remove("\n");
    ui->SystimepLabel->setText(strResult);
}

void timeset::RTCtimerUpdate(void)
{
    pro_rtc.write("hwclock -r \n");
    QString strResult = pro_rtc.readAllStandardOutput();
    ui->RTCtimepLabel->setText(strResult.mid(0,19));
}

void timeset::retBt_clicked()
{
    emit Mysignal();
    SysTimer->stop();
    RTCTimer->stop();
}

QString timeset::SystimeSet(QString  datetext)
{
    QString strCmd = QString("date -s \"%1\"").arg(datetext);
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

void timeset::SystimeSetBt_clicked()
{
    if(sync_flag == 1)
    {
        QMessageBox::information(this,"information",tr("The current time is synchronized with the network time,you can click the 'async network' button!"));
        return;
    }
    SysTimer->stop();
    QString  datetext = ui->datetime->text();
    QString ret = SystimeSet(datetext);
    if(ret == "0\n")
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
    QString strCmd = QString("hwclock -w  ");
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    process.write("hwclock -w \n");
    process.waitForFinished();
    strCmd = QString("echo $?");
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();
    process.close();
    return strResult;
}

void timeset::RTCSetBt_clicked()
{
    if(sync_flag == 1)
    {
        QMessageBox::information(this,"information",tr("The current time is synchronized with the network time,you can click the 'async network' button!"));
        return;
    }
    RTCTimer->stop();
    QString ret = RTCSet();
    if(ret == "0\n")
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
    ui->datetime->setFocus();
    if(show_first==0)
    {
        show_first++;
    }
    else
    {
        RTCtimerUpdate();
        SystimerUpdate();
        RTCTimer->start(1000);
        SysTimer->start(1000);
    }
}

void timeset::language_reload()
{
    ui->retranslateUi(this);
}

void timeset::timeset_font()
{
    qreal realWidth = s_width / realX * 2.54;
    qreal realHeight = s_height / realY *2.54;
    QFont font;
    if(screen_flag)
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
    ui->RtcReadLabel->setFont(font);
    ui->RTCSetBt->setFont(font);
    ui->RTCtimepLabel->setFont(font);
    ui->SystimepLabel->setFont(font);
    ui->SystimeReadLabel->setFont(font);
    ui->SystimeSetBt->setFont(font);
    ui->label->setFont(font);
    ui->sync_btn->setFont(font);
}

void timeset::sync_net()
{
    QString strCmd = QString("systemctl status systemd-timesyncd.service | grep Active |awk '{print $2}'");
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();
    if(strResult == "inactive\n")
    {
        ui->sync_btn->setText(tr("sync network"));
        sync_flag = 0;
    }
    else
    {
        ui->sync_btn->setText(tr("async network"));
        sync_flag = 1;
    }
    process.close();
}

void timeset::on_sync_btn_clicked()
{
    if(ui->sync_btn->text() == "sync network")
    {
        QString strCmd = QString("systemctl start systemd-timesyncd.service");
        QProcess process;
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished();
        process.close();
        ui->sync_btn->setText("async network");
        sync_flag = 1;
        RTCSet();RTCSet();
    }
    else
    {
        QString strCmd = QString("systemctl stop systemd-timesyncd.service");
        QProcess process;
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished();
        process.close();
        ui->sync_btn->setText("sync network");
        sync_flag = 0;
    }
}
