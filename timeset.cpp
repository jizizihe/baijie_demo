#include "timeset.h"
#include "ui_timeset.h"
#include <QScreen>

static int s_width;
static int s_height;
static int screen_flag;

static qreal realX;
static qreal realY;

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
    SysTimer->start(1000);
    RTCTimer = new QTimer(this);
    connect(RTCTimer,SIGNAL(timeout()),this,SLOT(RTCtimerUpdate()));
    RTCTimer->start(1000);

    connect(ui->retBt,SIGNAL(clicked(bool)),this,SLOT(retBt_clicked()));
    connect(ui->SystimeSetBt,SIGNAL(clicked()),this,SLOT(SystimeSetBt_clicked()));
    connect(ui->RTCSetBt,SIGNAL(clicked()),this,SLOT(RTCSetBt_clicked()));

}

timeset::~timeset()
{
    delete ui;
}

void timeset::SystimerUpdate(void)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");

    ui->SystimepLabel->setText(str);
}

void timeset::RTCtimerUpdate(void)
{
    RTCTimer->stop();

    QString strCmd = QString("hwclock -w");
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    process.start("hwclock -r");
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();
    ui->RTCtimepLabel->setText(strResult.mid(0,19));

    RTCTimer->start(10000);
}


void timeset::retBt_clicked()
{
    emit Mysignal();
}

QString timeset::SystimeSet(QString  datetext)
{
    //  QString text = "\"2021-09-23 18:30:50\"";
      QString strCmd = QString("date -s \"%1\"").arg(datetext);
      QProcess process;
      process.start("bash", QStringList() <<"-c" << strCmd);
      process.waitForFinished();

      strCmd = QString("echo $?").arg(datetext);
      //qDebug() << "text == " << strCmd;
      process.start("bash", QStringList() <<"-c" << strCmd);
      process.waitForFinished();

      QString strResult = process.readAllStandardOutput();

      return strResult;
}

void timeset::SystimeSetBt_clicked()
{
      QString  datetext = ui->datetime->text();
     // qDebug() << "text === " << datetext;

      QString ret = SystimeSet(datetext);
      if(ret == "0\n")
      {
     //     qDebug() << "Systime set ok!";
          QMessageBox::information(this,"information",tr("Systime set ok!"));
      }
      else
      {
          //qDebug() << "Systime set failed!";
          QMessageBox::critical(this,"information",tr("Systime set failed!"));
      }
}

/* ret : 0--success; 1 or other -- failed */
QString timeset::RTCSet(QString  datetext)
{
    //  QString text = "\"2021-09-23 18:30:50\"";
    QString strCmd = QString("hwclock -w");
 //   qDebug() << "text == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    strCmd = QString("echo $?").arg(datetext);
  //  qDebug() << "text == " << strCmd;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    //qDebug() << "strResult == " << strResult;

    return strResult;
}

void timeset::showEvent(QShowEvent *event)
{
    ui->datetime->setFocus();
}

void timeset::RTCSetBt_clicked()
{
    QString  datetext = ui->datetime->text();
   // qDebug() << "text === " << datetext;

    QString ret = RTCSet(datetext);
    if(ret == "0\n")
    {
        //qDebug() << "RTC set ok!";
        QMessageBox::information(this,"information",tr("RTC set ok!"));
    }
    else
    {
        qDebug() << "RTC set failed!";
        QMessageBox::critical(this,"information",tr("RTC set failed!"));

    }
}

void timeset::language_reload()
{
    ui->retranslateUi(this);
    //ui->pLabel->setText(tr("Time Settings"));
   // ui->SystimeSetBt->setText(tr("SystimeSet"));
   // ui->RTCSetBt->setText(tr("RTCSet"));
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
}
