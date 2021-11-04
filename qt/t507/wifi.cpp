#include "wifi.h"
#include "ui_wifi.h"

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wifi)
{
    ui->setupUi(this);

    this->resize(1024,600);

    retBt = new QPushButton(this);
    retBt->setFixedSize(100,40);
//    retBt->setText(tr("return"));
    retBt->move(10,10);
    retBt->setIcon(QIcon(":/t507_button_image/return.png"));

    pLabel = new QLabel(this);
    pLabel->setText(tr("Wifi Test"));
    pLabel->resize(200,100);
    pLabel->move(400,20);
    QFont ft;
    ft.setPointSize(18);
    pLabel->setFont(ft);

    WifiConnectBt = new QPushButton(this);
    WifiConnectBt->setText(tr("wifi connect"));
    WifiConnectBt->resize(250,50);
    WifiConnectBt->move(50,150);

    WifiDisconnectBt = new QPushButton(this);
    WifiDisconnectBt->setText(tr("wifi disconnect"));
    WifiDisconnectBt->resize(250,50);
    WifiDisconnectBt->move(50,230);

    HotSpotBt = new QPushButton(this);
    HotSpotBt->setText(tr("hot spot"));
    HotSpotBt->resize(250,50);
    HotSpotBt->move(50,310);

    SignalQualityBt = new QPushButton(this);
    SignalQualityBt->setText(tr("signal quality"));
    SignalQualityBt->resize(250,50);
    SignalQualityBt->move(50,390);

    StatusBt = new QPushButton(this);
    StatusBt->setText(tr("connection status"));
    StatusBt->resize(250,50);
    StatusBt->move(50,470);

    WifiMsgText = new QTextEdit(this);
    WifiMsgText->setReadOnly(true);
    WifiMsgText->resize(650,370);
    WifiMsgText->move(350,150);


    ConnectWin = new WifiConnect;

    ScanTimer = new QTimer(this);
    ScanTimer->start(100);
    //connect(ScanTimer,&QTimer::timeout,ConnectWin,SLOT(wifi_scan());

    connect(this,SIGNAL(scan_signal()),ConnectWin,SLOT(scan_show()));

    //connect(&HotSpotWin_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(retBt,SIGNAL(clicked(bool)),this,SLOT(retBt_clicked()));
    connect(WifiConnectBt,SIGNAL(clicked()),this,SLOT(WifiConnectBt_clicked()));
    connect(WifiDisconnectBt,SIGNAL(clicked()),this,SLOT(WifiDisconnectBt_clicked()));
    connect(HotSpotBt,SIGNAL(clicked()),this,SLOT(HotSpotBt_clicked()));
    connect(SignalQualityBt,SIGNAL(clicked()),this,SLOT(SignalQualityBt_clicked()));
    connect(StatusBt,SIGNAL(clicked()),this,SLOT(StatusBt_clicked()));

    emit scan_signal();
}

wifi::~wifi()
{
    delete ui;
}


void wifi::retBt_clicked()
{
    emit Mysignal();
}


void wifi::WifiConnectBt_clicked()
{
    //ConnectWin = new WifiConnect;
    ConnectWin->setGeometry(0,0,1024,600);
    //this->hide();
    ConnectWin->show();

}

QString wifi_disconnect()
{
    QString strCmd = QString("nmcli device disconnect wlan0");
    qDebug() << "text == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    qDebug() << strResult;

    return strResult;
}

void wifi::WifiDisconnectBt_clicked()
{
    QString strResult = wifi_disconnect();

    this->WifiMsgText->setText(strResult);
}

void wifi::HotSpotBt_clicked()
{
    //this->hide();

    //HotSpotWin_w.show();
    HotSpotSetWin = new HotSpot;
    HotSpotSetWin->setGeometry(0,0,1024,600);
    HotSpotSetWin->show();

}

QString get_wifisignalquality()
{
    /*
    QString strCmd = QString("wpa_cli -i wlan0 status|grep wpa_state |cut -d = -f 2");
    qDebug() << "text == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    qDebug() << strResult;

    if(strResult == QString("COMPLETED\n"))
    {
        this->WifiMsgText->setText("WiFi connected!");
        strCmd = QString("wpa_cli -i wlan0 status | sed -n '1p' | cut -d = -f 2");
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished();
        QString strResult = process.readAllStandardOutput();
        qDebug() << strResult;

        strCmd = QString("wpa_cli -i wlan0 scan_results | grep dc:fe:18:4d:d1:25 | cut -f 3");
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished();
        strResult = process.readAllStandardOutput();
        qDebug() << strResult;

        this->WifiMsgText->append(strResult);

    }
    else    //(strResult == QString::fromLocal8Bit("DISCONNECTED\n"))
    {
        this->WifiMsgText->setText("WiFi disconnected!");
        this->WifiMsgText->append("Please connect!");

    }
*/

    QProcess process;
    QString strCmd = QString("iw dev wlan0 link | grep SSID |awk '{print $2}'");
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();

    strCmd = QString("nmcli device wifi |grep '%1'|awk '{print $7}'|sed -n '1p' ").arg(strResult.remove("\n"));
    qDebug() << "text == " << strCmd;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    strResult = process.readAllStandardOutput();
    qDebug() << strResult;

    return strResult;
}

void wifi::SignalQualityBt_clicked()
{
    QString strResult = get_wifisignalquality();

    this->WifiMsgText->setText("signal");
    this->WifiMsgText->append(strResult);
}

QString get_wifistatus()
{
    //QString strCmd = QString("nmcli device wifi |awk '{print $8}'");//wifi or hotspot
    QString strCmd = QString("iw dev wlan0 link");
    qDebug() << "text == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString statusResult = process.readAllStandardOutput();
    //qDebug() << statusResult;

    strCmd = QString("wpa_cli -i wlan0 status | grep ip_address");
    qDebug() << "text == " << strCmd;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString addressResult = process.readAllStandardOutput();
    //qDebug() << addressResult;

    QString strResult = QString("%1\n%2").arg(statusResult).arg(addressResult);

    return strResult;
}

void wifi::StatusBt_clicked()
{
    QString strResult = get_wifistatus();

    this->WifiMsgText->setText(strResult);
}

void wifi::language_reload()
{
    ui->retranslateUi(this);
//    retBt->setText(tr("return"));
    pLabel->setText(tr("Wifi Test"));
    WifiConnectBt->setText(tr("wifi connect"));
    WifiDisconnectBt->setText(tr("wifi disconnect"));
    HotSpotBt->setText(tr("hot spot"));
    SignalQualityBt->setText(tr("signal quality"));
    StatusBt->setText(tr("connection status"));

}
