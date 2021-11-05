#include "wificonnect.h"
#include "ui_wificonnect.h"

WifiConnect::WifiConnect(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WifiConnect)
{
    ui->setupUi(this);

    WifiSsidLab = new QLabel(this);
    WifiSsidLab->resize(100,50);
    WifiSsidLab->move(500,50);
    WifiSsidLab->setText(tr("ssid"));

    WifiSsidLine = new QLineEdit(this);
    WifiSsidLine->resize(200,50);
    WifiSsidLine->move(600,50);

    WifiSsidLab = new QLabel(this);
    WifiSsidLab->resize(100,50);
    WifiSsidLab->move(500,150);
    WifiSsidLab->setText(tr("passwd"));

    WifiPasswdLine = new QLineEdit(this);
    WifiPasswdLine->setMaxLength(8);
    WifiPasswdLine->setEchoMode(QLineEdit::Password);
    WifiPasswdLine->resize(200,50);
    WifiPasswdLine->move(600,150);

    WifiNameBox = new QComboBox(this);
    WifiNameBox->resize(200,50);
    WifiNameBox->move(600,50);
    WifiNameBox->hide();

//    qDebug() << "Now there are " << WifiNameBox->count() << "Items";
//    qDebug() << "The current item is" << WifiNameBox->currentText();

    WifiScanBt = new QPushButton(tr("scan"),this);
    WifiScanBt->resize(100,50);
    WifiScanBt->move(830,50);

    WifiCleanBt = new QPushButton(tr("clean"),this);
    WifiCleanBt->resize(100,50);
    WifiCleanBt->move(830,150);

    WifiConnectBt = new QPushButton(tr("connect"),this);
    WifiConnectBt->resize(150,50);
    WifiConnectBt->move(560,250);

    WifiCloseBt = new QPushButton(tr("close"),this);
    WifiCloseBt->resize(150,50);
    WifiCloseBt->move(730,250);

    WifiScanListWid = new QListWidget(this);
    WifiScanListWid->resize(400,500);
    WifiScanListWid->move(20,100);

    connect(WifiScanBt,SIGNAL(clicked()),this,SLOT(WifiScanBt_clicked()));
    connect(WifiCleanBt,SIGNAL(clicked()),this,SLOT(WifiCleanBt_clicked()));
    connect(WifiConnectBt,SIGNAL(clicked()),this,SLOT(WifiConnectBt_clicked()));
    connect(WifiCloseBt,SIGNAL(clicked()),this,SLOT(WifiCloseBt_clicked()));
    connect(WifiScanListWid, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ListWidgeItem_clicked()));
}

WifiConnect::~WifiConnect()
{
    delete ui;
}

void WifiConnect::scan_show()
{
    wifi_connect_show();

    //QThread::sleep(5);
}

//void *WifiConnect::scan_thread(void *)
//{
//    //WifiConnect::WifiScanBt_clicked();
//    //emit scan_signal();
//    //wifi_connect_show();
//    QThread::sleep(5);
//    return NULL;
//}

//void WifiConnect::WifiScanThread()
//{
//    //ScanTimer->stop();

//    pthread_t scan_thread_id = 0;

//    if (pthread_create(&scan_thread_id, NULL, scan_thread,NULL))
//    {
//        printf("ERROR: can't create read_thread thread!\n");
//    }
//    else
//    {
//        pthread_detach(scan_thread_id);
//    }

//    //ScanTimer->start(5000);
//}

void WifiConnect::ListWidgeItem_clicked()
{
    qDebug() << "istWidgeItem_clicked()";
    QString str = WifiScanListWid->currentItem()->text();
    qDebug() << "WifiScanListWid->currentItem()->text = " << str;
    WifiSsidLine->setText(str);
}

QString wifi_scan()
{
    QString strCmd = QString("iw dev wlan0 scan|grep SSID|awk '{for(i=2;i<=NF;i++){printf \"%s \", $i}; printf \"\\n\"}' ");
    //qDebug() << "strCmd == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();

    QString strResult = process.readAllStandardOutput();
    //qDebug() << strResult;

    return strResult;
}

void WifiConnect::wifi_connect_show()
{
    int i,WifiNum = 0;
    QProcess process;

    WifiScanListWid->clear();
    WifiSsidLine->clear();
    QString ScanResult = wifi_scan();

    QStringList scanlist;

    scanlist = ScanResult.split("\n");
    scanlist.removeAll(QString(""));
    for(int i = 0; i < scanlist.size(); i++)
    {
        QString tmp = scanlist.at(i);
        qDebug() << tmp;
        WifiScanListWid->addItem(tmp.left(tmp.size() - 1));
    }
    WifiScanListWid->setCurrentRow(0);
    WifiScanListWid->setStyleSheet(R"(
        QListWidget { outline: none; border:1px solid gray; color: black; }
        QListWidget::Item { width: 400px; height: 50px; }
        QListWidget::Item:hover { background: #4CAF50; color: white; }
        QListWidget::item:selected { background: #e7e7e7; color: #f44336; }
        QListWidget::item:selected:!active { background: lightgreen; }
       )");
    WifiScanListWid->verticalScrollBar()->setStyleSheet("QScrollBar{width:40px;}");
    WifiScanListWid->horizontalScrollBar()->setStyleSheet("QScrollBar{height:30px;}");
    WifiScanListWid->setSelectionMode(QAbstractItemView::SingleSelection);

}

void WifiConnect::WifiScanBt_clicked()
{
    wifi_connect_show();
}

void WifiConnect::WifiCleanBt_clicked()
{

    WifiPasswdLine->setText("");
}

void wifi_connect(QString WifiSsid,QString PassWd)
{
    QString strCmd = QString("ifconfig wlan0 up");
    qDebug() << "--line--: " << __LINE__<<"strCmd == " << strCmd;
    QProcess process;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    QString strResult = process.readAllStandardOutput();
    qDebug() << strResult;

    strCmd = QString("nmcli connection show | grep %1 | awk '{print $1}'").arg(WifiSsid);
    qDebug() << "--line--: " << __LINE__<<"strcmd = " << strCmd;
    process.start("bash", QStringList() <<"-c" << strCmd);
    process.waitForFinished();
    strResult = process.readAllStandardOutput();
    qDebug() << "--line--: " << __LINE__<< strResult;

    QString find_str = QString("%1\n").arg(WifiSsid);
    qDebug() << "--line--: " << __LINE__<<"find_str = " << find_str;
    bool checktResult=strResult.contains(find_str,Qt::CaseInsensitive);
    qDebug() << "--line--: " << __LINE__<< checktResult;

    if(checktResult == 1)
    {
        strResult = "it had connected!";
        qDebug() << "--line--: " << __LINE__<< strResult;

        strCmd = QString("nmcli connection up %1").arg(WifiSsid);
        qDebug() << "--line--: " << __LINE__<<"strcmd = " << strCmd;
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished();
        qDebug() << "--line--: " << __LINE__<< process.readAllStandardOutput();
    }
    else
    {
        strResult = "it does not connected!!";
        qDebug() << "--line--: " << __LINE__<< strResult;

        strCmd = QString("nmcli device wifi connect \"%1\" password \"%2\" name %3").arg(WifiSsid).arg(PassWd).arg(WifiSsid);
        qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished();
        strResult = process.readAllStandardOutput();
        qDebug() << strResult;


        bool ConnectResult=strResult.contains("successfully activated",Qt::CaseInsensitive);
        qDebug() << ConnectResult;

        if(ConnectResult == 1)
        {
            strResult = "Connection successful!";
            qDebug() << "--line--: " << __LINE__<< strResult;
        }
        else
        {
            strResult = "Connection failed!";
            qDebug() << "--line--: " << __LINE__<< strResult;

            strCmd = QString("nmcli connection delete %1").arg(WifiSsid);
            qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
            process.start("bash", QStringList() <<"-c" << strCmd);
            process.waitForFinished();
            strResult = process.readAllStandardOutput();
            qDebug() << strResult;

            bool ConnectResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);
            qDebug() << ConnectResult;
            if(ConnectResult == 1)
            {
                strResult = "delete successful!";
                qDebug() << "--line--: " << __LINE__<< strResult;
            }
            else
            {
                strResult = "delete failed!";
                qDebug() << "--line--: " << __LINE__<< strResult;
            }

        }

    }

}

void WifiConnect::WifiConnectBt_clicked()
{
    QString WifiSsid = this->WifiSsidLine->text();
    qDebug() << "WifiSsid = " << WifiSsid;
    QString PassWd = this->WifiPasswdLine->text();
    qDebug() << "PassWd = " << PassWd;

    wifi_connect(WifiSsid, PassWd);
}

void WifiConnect::WifiCloseBt_clicked()
{
    this->close();
}

void WifiConnect::language_reload()
{
    ui->retranslateUi(this);
    WifiSsidLab->setText(tr("ssid"));
    WifiSsidLab->setText(tr("passwd"));
    WifiScanBt->setText(tr("scan"));
    WifiCleanBt->setText(tr("clean"));
    WifiConnectBt->setText(tr("connect"));
    WifiCloseBt->setText(tr("close"));
}
