#include "bluetooth.h"
#include "ui_bluetooth.h"

using namespace std;

bluetooth::bluetooth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bluetooth)
{
    ui->setupUi(this);

    retBt = new QPushButton(this);
    retBt->setFixedSize(100,40);
    retBt->setText(tr("return"));
    retBt->move(10,10);

    BTTitleLab = new QLabel( this);
    BTTitleLab->resize(200,100);
    BTTitleLab->move(400,20);
    BTTitleLab->setText("Blue Tooth");
    QFont ft;
    ft.setPointSize(18);
    BTTitleLab->setFont(ft);

    BTScanBt = new QPushButton("BT scan",this);
    BTScanBt->resize(250,50);
    BTScanBt->move(100,200);

    BTPairBt = new QPushButton("BT pair",this);
    BTPairBt->resize(250,50);
    BTPairBt->move(100,300);

    BTConnectBt = new QPushButton("BT connect",this);
    BTConnectBt->resize(250,50);
    BTConnectBt->move(100,400);

    BTNameBox = new QComboBox(this);
    BTNameBox->resize(500,80);
    BTNameBox->move(400,150);
//    BTNameBox->addItem("baijiekeji");

    BTText = new QTextEdit(this);
    BTText->resize(500,300);
    BTText->move(400,250);

    connect(retBt,SIGNAL(clicked(bool)),this,SLOT(retBt_clicked()));
    connect(BTScanBt,SIGNAL(clicked()),this,SLOT(BTScanBt_clicked()));
    connect(BTConnectBt,SIGNAL(clicked()),this,SLOT(BTConnectBt_clicked()));
    connect(BTPairBt,SIGNAL(clicked()),this,SLOT(BTPairBt_clicked()));

    connect(this,SIGNAL(bluetooth_signal(int,QString)),BtThread,SLOT(flag_set(int,QString)));
    connect(BtThread,SIGNAL(message(QString)),this,SLOT(recvmsg(QString)));
    connect(BtThread,SIGNAL(setText(bool)),this,SLOT(setText_slot(bool)));
}

bluetooth::~bluetooth()
{
    delete ui;

    BtThread->terminate();
    BtThread->wait();
}

void bluetooth::retBt_clicked()
{
    emit Mysignal();
}

void bluetooth::recvmsg(QString str)
{
    qDebug() << str;
    this->BTText->setText(str);
}

void bluetooth::setText_slot(bool flag)
{
    int BtCount,i;

    if(flag == true)
    {
        QString strCmd = QString("wc -l /bluetooth_scan | awk '{print $1}'");
        qDebug() << "strCmd == " << strCmd;
        QString strResult = BtThread->executeLinuxCmd(strCmd);
        qDebug() << strResult;

        BtCount = strResult.toInt();
        qDebug() << BtCount;

        if(BtCount < 2)
        {
            BTText->setText("Scan failed!");
        }

        for(i = 1; i < BtCount; i++)
        {
            strCmd = QString("cat /bluetooth_scan | sed -n '%1p' |awk '{$1=\"\"; print $0}'").arg(i+1);
            //qDebug() << "strCmd == " << strCmd;
            strResult = BtThread->executeLinuxCmd(strCmd);
            qDebug() << strResult;
            BTNameBox->addItem(strResult.simplified());
        }
    }
}

void bluetooth::BTScanBt_clicked()
{
    qDebug() << "BTScanBt_clicked";

    BTText->setText("Start scanning! \nPlease wait a moment...");

    emit bluetooth_signal(1,0);
    BtThread->start();
}

void bluetooth::BTPairBt_clicked()
{
    BTText->setText("Start Pairing! \nPlease wait a moment...");

    int BtNameIndex = BTNameBox->currentIndex();
    QString strCmd = QString("cat /bluetooth_scan | sed -n '%1p' | awk '{print $1}'").arg(BtNameIndex+2);
    //qDebug() << "strCmd == " << strCmd;
    QString BtAddress = BtThread->executeLinuxCmd(strCmd);
    qDebug() << BtAddress;

    emit bluetooth_signal(2,BtAddress);
    BtThread->start();

}

void bluetooth::BTConnectBt_clicked()
{
    BTText->setText("Start Connection! \nPlease wait a moment...");

    QString strCmd = QString("ps -x|grep pulseaudio | grep -v grep |wc -l");
    qDebug() << "strCmd == " << strCmd;
    QString strResult = BtThread->executeLinuxCmd(strCmd);
    qDebug() << strResult;

    if(strResult == QString("0\n"))
    {
        strCmd = QString("pulseaudio --start");
        qDebug() << "strCmd == " << strCmd;
        strResult = BtThread->executeLinuxCmd(strCmd);
        qDebug() << strResult;
    }

    int BtNameIndex = BTNameBox->currentIndex();
    strCmd = QString("cat /bluetooth_scan | sed -n '%1p' | awk '{print $1}'").arg(BtNameIndex+2);
    //qDebug() << "strCmd == " << strCmd;
    QString BtAddress = BtThread->executeLinuxCmd(strCmd);
    qDebug() << BtAddress;

    emit bluetooth_signal(3,BtAddress);
    BtThread->start();

}

