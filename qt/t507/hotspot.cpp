#include "hotspot.h"
#include "ui_hotspot.h"

HotSpot::HotSpot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HotSpot)
{
    ui->setupUi(this);

    HotSpotTitleLab = new QLabel( this);
    HotSpotTitleLab->resize(200,100);
    HotSpotTitleLab->move(200,10);
    HotSpotTitleLab->setText("Hot Spot");
    QFont ft;
    ft.setPointSize(14);
    HotSpotTitleLab->setFont(ft);

    HotSpotNameLab = new QLabel( this);
    HotSpotNameLab->resize(100,50);
    HotSpotNameLab->move(100,100);
    HotSpotNameLab->setText("name");

    HotSpotPasswdLab = new QLabel( this);
    HotSpotPasswdLab->resize(100,50);
    HotSpotPasswdLab->move(100,200);
    HotSpotPasswdLab->setText("passwd");

    HotSpotNameLine = new QLineEdit(this);
    HotSpotNameLine->resize(200,50);
    HotSpotNameLine->move(220,100);

    HotSpotPasswdLine = new QLineEdit(this);
    HotSpotPasswdLine->setMaxLength(8);
    HotSpotPasswdLine->setEchoMode(QLineEdit::Password);
    HotSpotPasswdLine->resize(200,50);
    HotSpotPasswdLine->move(220,200);

    HotSpotUpBt = new QPushButton("up",this);
    HotSpotUpBt->resize(100,50);
    HotSpotUpBt->move(100,300);

    HotSpotDownBt = new QPushButton("down",this);
    HotSpotDownBt->resize(100,50);
    HotSpotDownBt->move(225,300);

    HotSpotCloseBt = new QPushButton("close",this);
    HotSpotCloseBt->resize(100,50);
    HotSpotCloseBt->move(350,300);

    connect(HotSpotUpBt,SIGNAL(clicked()),this,SLOT(HotSpotUpBt_clicked()));
    connect(HotSpotDownBt,SIGNAL(clicked()),this,SLOT(HotSpotDownBt_clicked()));
    connect(HotSpotCloseBt,SIGNAL(clicked()),this,SLOT(HotSpotCloseBt_clicked()));

}

HotSpot::~HotSpot()
{
    delete ui;
}


QString hotspot_connect(QString HtName,QString HtPasswd)
{
    QString strCmd = QString("nmcli device wifi hotspot con-name %1 ifname wlan0 ssid \"%2\" password \"%3\" ").arg(HtName).arg(HtName).arg(HtPasswd);
    //QString strCmd = QString("nmcli connection up hotspot1");//activated hotspot
    qDebug() << "strCmd == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    qDebug() << strResult;

    return strResult;
}

void HotSpot::HotSpotUpBt_clicked()
{
    QString HtName = this->HotSpotNameLine->text();
    QString HtPasswd= this->HotSpotPasswdLine->text();;
    qDebug() << HtName;
    qDebug() << HtPasswd;

    QString strResult = hotspot_connect(HtName, HtPasswd);
    qDebug() << strResult;
}

QString hotspot_disconnect()
{
    QString strCmd = QString("nmcli device status | grep wlan0 | awk '{print $3}'");
    qDebug() << "strCmd == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    qDebug() << strResult;

    if(strResult == "connected\n")
    {
        strCmd = QString("nmcli device disconnect wlan0");
        qDebug() << "strCmd == " << strCmd;
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished();

        strResult = process.readAllStandardOutput();
        qDebug() << strResult;
    }
    else
    {
        qDebug() << strResult;
    }

    return strResult;
}

void HotSpot::HotSpotDownBt_clicked()
{
    QString strResult = hotspot_disconnect();
    qDebug() << strResult;
}

void HotSpot::HotSpotCloseBt_clicked()
{
    this->close();
}
