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
    retBt->setText(tr("return"));
    retBt->move(10,10);

    pLabel = new QLabel(this);
    pLabel->setText(tr("Time Settings"));
    pLabel->resize(200,100);
    pLabel->move(350,20);

    datetime = new QDateTimeEdit(this);
    datetime->resize(350,50);
    datetime->move(300,150);
    datetime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    datetime->setTime(QTime::currentTime());
    datetime->setDate(QDate::currentDate());

    SystimeSetBt = new QPushButton(tr("SystimeSetBt"), this);
    SystimeSetBt->resize(200,50);
    SystimeSetBt->move(300,300);

    RTCSetBt = new QPushButton(tr("RTCSetBt"), this);
    RTCSetBt->resize(200,50);
    RTCSetBt->move(510,300);

    connect(retBt,SIGNAL(clicked(bool)),this,SLOT(retBt_clicked()));
    connect(SystimeSetBt,SIGNAL(clicked()),this,SLOT(SystimeSetBt_clicked()));
    connect(RTCSetBt,SIGNAL(clicked()),this,SLOT(RTCSetBt_clicked()));

}

timeset::~timeset()
{
    delete ui;
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

void timeset::RTCSet(QString  datetext)
{
    //  QString text = "\"2021-09-23 18:30:50\"";
    QString strCmd = QString("hwclock --set --date \"%1\"").arg(datetext);
    qDebug() << "text == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

}

void timeset::RTCSetBt_clicked()
{
    QString  datetext = this->datetime->text();
    qDebug() << "text === " << datetext;

    RTCSet(datetext);
}

void timeset::language_reload()
{
    ui->retranslateUi(this);
    retBt->setText(tr("return"));
    pLabel->setText(tr("Time Settings"));
    SystimeSetBt->setText(tr("SystimeSetBt"));
    RTCSetBt->setText(tr("RTCSetBt"));
}
