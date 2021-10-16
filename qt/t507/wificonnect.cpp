#include "wificonnect.h"
#include "ui_wificonnect.h"

WifiConnect::WifiConnect(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WifiConnect)
{
    ui->setupUi(this);

    WifiSsidLab = new QLabel(this);
    WifiSsidLab->resize(60,50);
    WifiSsidLab->move(20,50);
    WifiSsidLab->setText("ssid");

    WifiSsidLab = new QLabel(this);
    WifiSsidLab->resize(100,50);
    WifiSsidLab->move(20,150);
    WifiSsidLab->setText("passwd");

    WifiPasswdLine = new QLineEdit(this);
    WifiPasswdLine->setMaxLength(8);
    WifiPasswdLine->setEchoMode(QLineEdit::Password);
    WifiPasswdLine->resize(200,50);
    WifiPasswdLine->move(120,150);

    WifiNameBox = new QComboBox(this);
    WifiNameBox->resize(200,50);
    WifiNameBox->move(120,50);

    qDebug() << "Now there are " << WifiNameBox->count() << "Items";
    qDebug() << "The current item is" << WifiNameBox->currentText();

    WifiScanBt = new QPushButton("scan",this);
    WifiScanBt->resize(100,50);
    WifiScanBt->move(350,50);

    WifiCleanBt = new QPushButton("clean",this);
    WifiCleanBt->resize(100,50);
    WifiCleanBt->move(350,150);

    WifiConnectBt = new QPushButton("connect",this);
    WifiConnectBt->resize(150,50);
    WifiConnectBt->move(80,250);

    WifiCloseBt = new QPushButton("close",this);
    WifiCloseBt->resize(150,50);
    WifiCloseBt->move(250,250);

    connect(WifiScanBt,SIGNAL(clicked()),this,SLOT(WifiScanBt_clicked()));
    connect(WifiCleanBt,SIGNAL(clicked()),this,SLOT(WifiCleanBt_clicked()));
    connect(WifiConnectBt,SIGNAL(clicked()),this,SLOT(WifiConnectBt_clicked()));
    connect(WifiCloseBt,SIGNAL(clicked()),this,SLOT(WifiCloseBt_clicked()));

}

WifiConnect::~WifiConnect()
{
    delete ui;
}

void wifi_scan()
{
    QString strCmd = QString("iw dev wlan0 scan | grep SSID|awk '{print $2}' > /wifi_scan");
    qDebug() << "strCmd == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    qDebug() << strResult;

}

void WifiConnect::WifiScanBt_clicked()
{
    int i,WifiNum = 0;
    QProcess process;

    wifi_scan();

    QString strCmd = QString("wc -l /wifi_scan | awk '{print $1}' ");
    qDebug() << "strCmd == " << strCmd;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    qDebug() << strResult;

    WifiNum = strResult.toInt();
    qDebug() << WifiNum;

    for(i = 0; i < WifiNum; i++)
    {
        strCmd = QString("cat /wifi_scan | sed -n '%1p' ").arg(i+1);
        //qDebug() << "strCmd == " << strCmd;
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished();

        strResult = process.readAllStandardOutput();
        //qDebug() << strResult.simplified();

        WifiNameBox->addItem(strResult.simplified());
    }

}

void WifiConnect::WifiCleanBt_clicked()
{

    WifiPasswdLine->setText("");
}

void wifi_connect(QString WifiSsid,QString PassWd)
{
    QString strCmd = QString("ifconfig wlan0 up");
    qDebug() << "strCmd == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();
    qDebug() << strResult;

    strCmd = QString("nmcli device wifi connect \"%1\" password \"%2\" name wifi").arg(WifiSsid).arg(PassWd);
    qDebug() << "strCmd == " << strCmd;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    strResult = process.readAllStandardOutput();
    qDebug() << strResult;

}

void WifiConnect::WifiConnectBt_clicked()
{
    QString WifiSsid = this->WifiNameBox->currentText();
    qDebug() << "WifiSsid = " << WifiSsid;
    QString PassWd = this->WifiPasswdLine->text();
    qDebug() << "PassWd = " << PassWd;

    wifi_connect(WifiSsid, PassWd);
}

void WifiConnect::WifiCloseBt_clicked()
{
    this->close();
}
