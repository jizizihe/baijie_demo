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
    retBt->setText(tr("return"));
    retBt->move(10,10);

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
    WifiMsgText->resize(650,370);
    WifiMsgText->move(350,150);


    //connect(&HotSpotWin_w,SIGNAL(Mysignal()),this,SLOT(show_main()));
    connect(retBt,SIGNAL(clicked(bool)),this,SLOT(retBt_clicked()));
    connect(WifiConnectBt,SIGNAL(clicked()),this,SLOT(WifiConnectBt_clicked()));
    connect(WifiDisconnectBt,SIGNAL(clicked()),this,SLOT(WifiDisconnectBt_clicked()));
    connect(HotSpotBt,SIGNAL(clicked()),this,SLOT(HotSpotBt_clicked()));
    connect(SignalQualityBt,SIGNAL(clicked()),this,SLOT(SignalQualityBt_clicked()));
    connect(StatusBt,SIGNAL(clicked()),this,SLOT(StatusBt_clicked()));

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
    ConnectWin = new WifiConnect;
    ConnectWin->setGeometry(200,150,600,400);
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
    HotSpotSetWin->setGeometry(200,150,600,400);
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
    QString strCmd = QString("iw dev wlan0 link | grep signal");
    qDebug() << "text == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();
    qDebug() << strResult;

    return strResult;
}

void wifi::SignalQualityBt_clicked()
{
    QString strResult = get_wifisignalquality();

    this->WifiMsgText->setText(strResult);
}

QString get_wifistatus()
{
    //QString strCmd = QString("nmcli device status");//wifi or hotspot
    QString strCmd = QString("iw dev wlan0 link");
    qDebug() << "text == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();
    qDebug() << strResult;

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
    retBt->setText(tr("return"));
    pLabel->setText(tr("Wifi Test"));
    WifiConnectBt->setText(tr("wifi connect"));
    WifiDisconnectBt->setText(tr("wifi disconnect"));
    HotSpotBt->setText(tr("hot spot"));
    SignalQualityBt->setText(tr("signal quality"));
    StatusBt->setText(tr("connection status"));

}
