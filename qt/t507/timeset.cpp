#include "timeset.h"
#include "ui_timeset.h"

timeset::timeset(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::timeset)
{
    ui->setupUi(this);

    this->resize(1024,600);

    retBt = new QPushButton(this);
    retBt->setFixedSize(100,40);
//    retBt->setText(tr("return"));
    retBt->setIcon(QIcon(":/t507_button_image/return.png"));
    retBt->move(10,10);

    pLabel = new QLabel(this);
    pLabel->setText(tr("Time Settings"));
    pLabel->resize(200,100);
    pLabel->move(350,20);
    QFont ft;
    ft.setPointSize(18);
    pLabel->setFont(ft);

    SystimeReadLabel = new QLabel(this);
    SystimeReadLabel->setText(tr("Systime"));
    SystimeReadLabel->resize(200,100);
    SystimeReadLabel->move(150,150);

    RTCReadLabel = new QLabel(this);
    RTCReadLabel->setText(tr("RTCtime"));
    RTCReadLabel->resize(200,100);
    RTCReadLabel->move(150,220);

    datetime = new QDateTimeEdit(this);
    datetime->resize(350,50);
    datetime->move(300,350);
    datetime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    datetime->setTime(QTime::currentTime());
    datetime->setDate(QDate::currentDate());

    SystimeSetBt = new QPushButton(tr("SystimeSet"), this);
    SystimeSetBt->resize(200,50);
    SystimeSetBt->move(300,450);

    RTCSetBt = new QPushButton(tr("RTC Write"), this);
    RTCSetBt->resize(200,50);
    RTCSetBt->move(510,450);

    SystimepLabel = new QLabel(this);
//    SystimepLabel->setText(tr("Time Settings"));
    SystimepLabel->resize(400,100);
    SystimepLabel->move(300,150);

    RTCtimepLabel = new QLabel(this);
//    RTCtimepLabel->setText(tr("Time Settings"));
    RTCtimepLabel->resize(500,100);
    RTCtimepLabel->move(300,220);

    QTimer *SysTimer = new QTimer(this);
    connect(SysTimer,SIGNAL(timeout()),this,SLOT(SystimerUpdate()));
    SysTimer->start(1000);
    RTCTimer = new QTimer(this);
    connect(RTCTimer,SIGNAL(timeout()),this,SLOT(RTCtimerUpdate()));
    RTCTimer->start(1000);

    connect(retBt,SIGNAL(clicked(bool)),this,SLOT(retBt_clicked()));
    connect(SystimeSetBt,SIGNAL(clicked()),this,SLOT(SystimeSetBt_clicked()));
    connect(RTCSetBt,SIGNAL(clicked()),this,SLOT(RTCSetBt_clicked()));

}

timeset::~timeset()
{
    delete ui;
}

void timeset::SystimerUpdate(void)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");

    SystimepLabel->setText(str);
}

void timeset::RTCtimerUpdate(void)
{
    RTCTimer->stop();

    QString strCmd = QString("hwclock -w");
    //qDebug() << "text == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    process.start("hwclock -r");
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();
    //qDebug() << "strResult == " << strResult.mid(0,19);
    RTCtimepLabel->setText(strResult.mid(0,19));

    RTCTimer->start(20000);
}


void timeset::retBt_clicked()
{
    emit Mysignal();
}

void timeset::SystimeSet(QString  datetext)
{
    //  QString text = "\"2021-09-23 18:30:50\"";
      QString strCmd = QString("date -s \"%1\"").arg(datetext);
      qDebug() << "text == " << strCmd;
      QProcess process;
      process.start("bash", QStringList() <<"-c" << strCmd);
      process.waitForFinished();

}

void timeset::SystimeSetBt_clicked()
{
      QString  datetext = this->datetime->text();
      qDebug() << "text === " << datetext;

      SystimeSet(datetext);
}

/* ret : 0--success; 1 or other -- failed */
QString timeset::RTCSet(QString  datetext)
{
    //  QString text = "\"2021-09-23 18:30:50\"";
    QString strCmd = QString("hwclock -w");
    qDebug() << "text == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    strCmd = QString("echo $?").arg(datetext);
    qDebug() << "text == " << strCmd;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    //qDebug() << "strResult == " << strResult;

    return strResult;
}

void timeset::RTCSetBt_clicked()
{
    QString  datetext = this->datetime->text();
    qDebug() << "text === " << datetext;

    QString ret = RTCSet(datetext);
    if(ret == "0\n")
    {
        qDebug() << "RTC set ok!";
    }
    else
    {
        qDebug() << "RTC set failed!";
    }
}

void timeset::language_reload()
{
    ui->retranslateUi(this);
//    retBt->setText(tr("return"));
    pLabel->setText(tr("Time Settings"));
    SystimeSetBt->setText(tr("SystimeSet"));
    RTCSetBt->setText(tr("RTC Write"));
}
