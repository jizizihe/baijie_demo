#include "wificonnect.h"
#include "ui_wificonnect.h"

WifiConnect::WifiConnect(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WifiConnect)
{
    ui->setupUi(this);

    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();
    //qDebug()<<screen_width<<screen_height;

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

//    WifiScanBt->setIcon(QIcon(":/t507_button_image/bluetooth/scan.png"));
    WifiScanBt = new QPushButton(tr("Refresh"),this);
    WifiScanBt->resize(120,50);
    WifiScanBt->move(300,50);

//    WifiCleanBt->setIcon(QIcon(":/t507_button_image/serial/clean.png"));
    WifiCleanBt = new QPushButton(tr("clean"),this);
    WifiCleanBt->resize(100,50);
    WifiCleanBt->move(830,150);

//    WifiConnectBt->setIcon(QIcon(":/t507_button_image/wifi/connect.png"));
    WifiConnectBt = new QPushButton(tr("connect"),this);
    WifiConnectBt->resize(150,50);
    WifiConnectBt->move(560,250);

//    WifiCloseBt->setIcon(QIcon(":/t507_button_image/wifi/close.png"));
    WifiCloseBt = new QPushButton(tr("close"),this);
    WifiCloseBt->resize(150,50);
    WifiCloseBt->move(730,250);

    WifiScanListWid = new QListWidget(this);
    WifiScanListWid->resize(400,500);
    WifiScanListWid->move(20,100);

    LoadLabel = new QLabel(this);
    //LoadLabel->resize(100,100);
    LoadLabel->move(screen_width/2 ,screen_height/2);
    pMovie = new QMovie("://t507_button_image/loading.webp");
    LoadLabel->setFixedSize(50, 50);
    LoadLabel->setScaledContents(true);
    LoadLabel->setMovie(pMovie);
    //pMovie->start();

    connect(WifiScanBt,SIGNAL(clicked()),this,SLOT(WifiScanBt_clicked()));
    connect(WifiCleanBt,SIGNAL(clicked()),this,SLOT(WifiCleanBt_clicked()));
    connect(WifiConnectBt,SIGNAL(clicked()),this,SLOT(WifiConnectBt_clicked()));
    connect(WifiCloseBt,SIGNAL(clicked()),this,SLOT(WifiCloseBt_clicked()));
    connect(WifiScanListWid, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ListWidgeItem_clicked()));

    QThread * myThread = new QThread(this);
    wifi_thread * WifiThread = new wifi_thread();
    connect(this,SIGNAL(ToThread()),WifiThread,SLOT(Thread_Fun()));
    connect(this,SIGNAL(wifi_scan_msg()),WifiThread,SLOT(wifi_scan_thread()));
    connect(WifiThread,SIGNAL(send_msg(QString)),this,SLOT(recv_msg(QString)));
    WifiThread->moveToThread(myThread);
    myThread->start();

    emit ToThread();
    emit wifi_scan_msg();

    SwitchBtn = new slideButton(this);
    SwitchBtn->move(220,55);
    //qDebug() << "LINe:" << __LINE__<<"switchflag " << switchflag;
    connect(SwitchBtn,SIGNAL(buttonChange(int )),this,SLOT(BtnChange_flag(int)));

}

WifiConnect::~WifiConnect()
{
    delete ui;
}

void WifiConnect::recv_msg(QString ScanResult)
{
    //qDebug() << "LINE:" << __LINE__ << "ScanResult:" << ScanResult;

    WifiScanListWid->clear();
    WifiSsidLine->clear();

    scanlist = ScanResult.split("\n");
    scanlist.removeAll(QString(""));
    for(int i = 0; i < scanlist.size(); i++)
    {
        QString tmp = scanlist.at(i);
        //qDebug() << tmp;
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
    WifiScanListWid->verticalScrollBar()->setStyleSheet("QScrollBar{width:30px;}");
    WifiScanListWid->horizontalScrollBar()->setStyleSheet("QScrollBar{height:30px;}");
    WifiScanListWid->setSelectionMode(QAbstractItemView::SingleSelection);

    pMovie->stop();
    LoadLabel->close();

}

void WifiConnect::ListWidgeItem_clicked()
{
    qDebug() << "istWidgeItem_clicked()";
    QString str = WifiScanListWid->currentItem()->text();
    qDebug() << "WifiScanListWid->currentItem()->text = " << str;
    WifiSsidLine->setText(str);
}

void WifiConnect::WifiScanBt_clicked()
{
    int flag;

    flag = SwitchBtn->get_switchflag();
    if(flag == 0) // open
    {
        LoadLabel->show();
        pMovie->start();

        emit wifi_scan_msg();
    }
    else
    {
        return;
    }
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
        //strResult = "it does not connected!!";
        //qDebug() << "--line--: " << __LINE__<< strResult;

        strCmd = QString("nmcli device wifi connect \"%1\" password \"%2\" name %3").arg(WifiSsid).arg(PassWd).arg(WifiSsid);
        qDebug() << "--line--: " << __LINE__<< "strCmd == " << strCmd;
        process.start("bash", QStringList() <<"-c" << strCmd);
        process.waitForFinished(-1);
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
    WifiSsidLine->clear();
    WifiPasswdLine->clear();
    this->close();
}

void WifiConnect::BtnChange_flag(int switchflag)
{
    //qDebug() << "LINe:" << __LINE__<<"switchflag " << switchflag;

    if(switchflag == 0) // open
    {
        if(WifiScanListWid->count() == 0)
        {
            wifi_scan_msg();
        }

        for(int i = 0; i < WifiScanListWid->count(); i++)
        {
            WifiScanListWid->setItemHidden(WifiScanListWid->item(i), false);
        }

    }
    else //close
    {
        for(int i = 0; i < WifiScanListWid->count(); i++)
        {
            WifiScanListWid->setItemHidden(WifiScanListWid->item(i), true);
        }
    }
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


slideButton::slideButton(QWidget *parent) :
    QWidget(parent)
{
    resize(80,40);
    switchflag=0; // default close
    initflag=0; //initflag = 1 init
    //update();
}

void slideButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if(initflag==0)
    { // init
        //qDebug() << "LINe:" << __LINE__<<"switchflag " << switchflag;

        painter.setPen(Qt::NoPen);
        if(switchflag==0){
            painter.setBrush(Qt::gray);
        }
        else if(switchflag==1){
            painter.setBrush(Qt::blue);
        }
        QRectF re(1,2,40,40);
        int startAngle = 90*16;
        int endAngel = 180*16;
        painter.drawPie(re,startAngle,endAngel);
        painter.drawRect(20,2,40,40);
        QRectF re2(40,2,40,40);
        int startAngle2 = -90*16;
        int endAngel2 = 180*16;
        painter.drawPie(re2,startAngle2,endAngel2);

        if(switchflag==0){ // close status
            painter.setBrush(Qt::white);
            painter.drawEllipse(2,3,38,38);
        }
        else if(switchflag==1){ // open status
            painter.setBrush(Qt::white);
            painter.drawEllipse(41,3,38,38);
        }
        initflag=1;
    }
    else
    {
        //qDebug() << "LINe:" << __LINE__<<"switchflag " << switchflag;

        painter.setPen(Qt::NoPen);
        if(switchflag==0){  //前一个状态为open
            painter.setBrush(Qt::gray);
        }
        else if(switchflag==1){//前一个状态为close
            painter.setBrush(Qt::blue);
        }
        QRectF re(1,2,40,40);
        int startAngle = 90*16;
        int endAngel = 180*16;
        painter.drawPie(re,startAngle,endAngel);
        painter.drawRect(20,2,40,40);
        QRectF re2(40,2,40,40);
        int startAngle2 = -90*16;
        int endAngel2 = 180*16;
        painter.drawPie(re2,startAngle2,endAngel2);

        if(switchflag==0){ // open
            painter.setBrush(Qt::white);
            painter.drawEllipse(2,3,38,38);

            switchflag=1;
        }
        else if(switchflag==1){  //close
            painter.setBrush(Qt::white);
            painter.drawEllipse(41,3,38,38);
            switchflag=0;
        }
        emit buttonChange(switchflag);
    }
}

void slideButton::mouseReleaseEvent(QMouseEvent *)
{
    update();
}
