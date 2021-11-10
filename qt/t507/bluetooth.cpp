#include "bluetooth.h"
#include "ui_bluetooth.h"

using namespace std;

bluetooth::bluetooth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bluetooth)
{
    ui->setupUi(this);

    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();
    //qDebug()<<screen_width<<screen_height;

    retBt = new QPushButton(this);
    retBt->setFixedSize(100,40);
//    retBt->setText(tr("return"));
    retBt->setIcon(QIcon(":/t507_button_image/return.png"));
    retBt->move(10,10);

    BTTitleLab = new QLabel( this);
    BTTitleLab->resize(300,100);
    BTTitleLab->move(this->width()/3,20);
    BTTitleLab->setText(tr("Blue Tooth"));
    QFont ft;
    ft.setPointSize(18);
    BTTitleLab->setFont(ft);

    BTScanBt = new QPushButton(tr("BT scan"),this);
    BTScanBt->resize(250,50);
    BTScanBt->move(this->width()/10,this->height()/3);

    BTPairBt = new QPushButton(tr("BT pair"),this);
    BTPairBt->resize(250,50);
    BTPairBt->move(this->width()/10,this->height()/2);

    BTConnectBt = new QPushButton(tr("BT connect"),this);
    BTConnectBt->resize(250,50);
    BTConnectBt->move(this->width()/10,this->height()/3*2);

    BtNameWidget = new QListWidget(this);
    BtNameWidget->resize(this->width()/2,450);
    BtNameWidget->move(this->width()/2-100,this->height()/6);

    LoadLabel = new QLabel(this);
    //LoadLabel->resize(100,100);
    LoadLabel->move(screen_width/2 - 150,screen_height/2 -50);
    pMovie = new QMovie("://t507_button_image/loading.webp");
    LoadLabel->setFixedSize(50, 50);
    LoadLabel->setScaledContents(true);
    LoadLabel->setMovie(pMovie);
    //pMovie->start();

    connect(retBt,SIGNAL(clicked(bool)),this,SLOT(retBt_clicked()));
    connect(BTScanBt,SIGNAL(clicked()),this,SLOT(BTScanBt_clicked()));
    connect(BTConnectBt,SIGNAL(clicked()),this,SLOT(BTConnectBt_clicked()));
    connect(BTPairBt,SIGNAL(clicked()),this,SLOT(BTPairBt_clicked()));

    connect(this,SIGNAL(bluetooth_signal(int,QString)),BtThread,SLOT(flag_set(int,QString)));
    connect(BtThread,SIGNAL(message(int, QString)),this,SLOT(recvmsg(int, QString)));
    connect(BtThread,SIGNAL(setText(bool)),this,SLOT(setText_slot(bool)));
    emit bluetooth_signal(1,0);
    BtThread->start();

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

void bluetooth::recvmsg(int signal_type,QString str)
{
    if(signal_type == 1)
    {
        //qDebug() << "Line "<< __LINE__<< "recvmsg: "<< str;

        BtScanList.clear();
        BtScanList = str.split("\n");
        BtScanList.removeAll(QString(""));
        for(int i = 0; i < BtScanList.size(); i++)
        {
            QString tmp = BtScanList.at(i);
            tmp = tmp.trimmed().section("\t",1,1);
            BtNameWidget->addItem(tmp);
        }
        BtNameWidget->setCurrentRow(0);
        BtNameWidget->setStyleSheet(R"(
            QListWidget { outline: none; border:1px solid gray; color: black; }
            QListWidget::Item { width: 800px; height: 50px; }
            QListWidget::Item:hover { background: #4CAF50; color: white; }
            QListWidget::item:selected { background: #e7e7e7; color: #f44336; }
            QListWidget::item:selected:!active { background: lightgreen; }
           )");
        BtNameWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:40px;}");
        BtNameWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{height:30px;}");
        BtNameWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else if(signal_type == 2)
    {
        if(str == "successful")
        {
            QMessageBox::information(this,"information","pair success!");
        }
        else if(str == "failed")
        {
            QMessageBox::critical(this,"information","pair failed!");
        }
    }
    else if(signal_type == 3)
    {
        if(str == "successful")
        {
            QMessageBox::information(this,"information","connect success!");
        }
        else if(str == "failed")
        {
            QMessageBox::critical(this,"information","connect failed!");
        }
    }

    BTScanBt->setEnabled(true);
    BTPairBt->setEnabled(true);
    BTConnectBt->setEnabled(true);

    pMovie->stop();
    LoadLabel->close();

}

void bluetooth::setText_slot(bool flag)
{
    /*
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
            BTText->setText(tr("Scan failed!"));
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
    */
}

void bluetooth::BTScanBt_clicked()
{
    qDebug() << "BTScanBt_clicked";
    BtNameWidget->clear();
    //BTText->setText(tr("Start scanning! \nPlease wait a moment..."));

    BTPairBt->setDisabled(true);
    BTConnectBt->setDisabled(true);
    LoadLabel->show();
    pMovie->start();

    emit bluetooth_signal(1,0);
    BtThread->start();
}

void bluetooth::BTPairBt_clicked()
{
    int BtNameIndex = BtNameWidget->currentRow();
    QString BtAddress = BtScanList.at(BtNameIndex);
    BtAddress = BtAddress.trimmed().section("\t",0,0);
    qDebug() << BtAddress;

    LoadLabel->show();
    pMovie->start();
    emit bluetooth_signal(2,BtAddress);
    BtThread->start();
}

void bluetooth::BTConnectBt_clicked()
{
    LoadLabel->show();
    pMovie->start();

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

    int BtNameIndex = BtNameWidget->currentRow();
    QString BtAddress = BtScanList.at(BtNameIndex);
    BtAddress = BtAddress.trimmed().section("\t",0,0);
    qDebug() << BtAddress;

    emit bluetooth_signal(3,BtAddress);
    BtThread->start();
}

void bluetooth::language_reload()
{
    ui->retranslateUi(this);
//    retBt->setText(tr("return"));
    BTTitleLab->setText(tr("Blue Tooth"));
    BTScanBt->setText(tr("BT scan"));
    BTPairBt->setText(tr("BT pair"));
    BTConnectBt->setText(tr("BT connect"));
}
