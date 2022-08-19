#include "bluetooth.h"
#include "ui_bluetooth.h"
#include <QScreen>

using namespace std;
static QScreen *screen;
static int screen_flag;
static int Width;
static int Height;
static int open_flag;

bluetooth::bluetooth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bluetooth)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    Width = screen->size().width();			//屏幕宽
    Height = screen->size().height();
    if(Width < Height)
    {
        screen_flag = 1;
    }
    open_flag = 1;
    wifi_bt_t = new wifi_bt_interface(this);
    ui->label_2->setText(QString(tr("bluetooth name:")));
    ui->label_2->hide();
    blue_font();

    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();
    //qDebug()<<screen_width<<screen_height;

    LoadLabel = new QLabel(this);
    //LoadLabel->resize(100,100);
    LoadLabel->move(screen_width/2 - 150,screen_height/2 -50);
    pMovie = new QMovie("://button_image/loading.webp");
    LoadLabel->setFixedSize(50, 50);
    LoadLabel->setScaledContents(true);
    LoadLabel->setMovie(pMovie);
    //pMovie->start();

    myThread = new QThread(this);
    BluetoothThread = new bluetooth_thread();
    connect(this,SIGNAL(bluetooth_scan_msg()),BluetoothThread,SLOT(bluetooth_scan_thread()));
    connect(this,SIGNAL(bluetooth_pair_msg(QString)),BluetoothThread,SLOT(bluetooth_pair_thread(QString)));
    connect(this,SIGNAL(bluetooth_connect_msg(QString)),BluetoothThread,SLOT(bluetooth_connect_thread(QString)));

    connect(BluetoothThread,SIGNAL(send_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    BluetoothThread->moveToThread(myThread);
    myThread->start();

    emit bluetooth_scan_msg();
}

bluetooth::~bluetooth()
{
    delete ui;

    delete BluetoothThread;
    delete myThread;

}

void bluetooth::BtnChange_flag(bool btflag)
{
    btflag = open_flag;

    if(btflag == 1) // open
    {
        wifi_bt_t->bluetooth_enable(true);

//        if(ui->BtNameListWidget->count() == 0)
//        {
//            LoadLabel->show();
//            pMovie->start();
//            emit bluetooth_signal(1,0);
//        }

        for(int i = 0; i < ui->BtNameListWidget->count(); i++)
        {
            ui->BtNameListWidget->setItemHidden(ui->BtNameListWidget->item(i), false);
        }
        ui->BTScanBtn->setDisabled(false);
        ui->BTPairBtn->setDisabled(false);
        ui->BTConnectBtn->setDisabled(false);
    }
    else //close
    {
        wifi_bt_t->bluetooth_enable(false);

        for(int i = 0; i < ui->BtNameListWidget->count(); i++)
        {
            ui->BtNameListWidget->setItemHidden(ui->BtNameListWidget->item(i), true);
        }
        pMovie->stop();
        LoadLabel->close();
        ui->BTScanBtn->setDisabled(true);
        ui->BTPairBtn->setDisabled(true);
        ui->BTConnectBtn->setDisabled(true);
    }

}

void bluetooth::recv_msg(int signal_type,QString str)
{
    int flag;
    if(signal_type == Bt_scan_signal)
    {
        //qDebug() << "Line "<< __LINE__<< "recvmsg: "<< str;
        ui->BtNameListWidget->clear();
        flag = open_flag;
        //qDebug() << "line:" << __LINE__ << "flag:" << flag;
        if(flag == 0)
            return ;
        BtScanList.clear();
        BtScanList = str.split("\n");
        BtScanList.removeAll(QString(""));
        for(int i = 0; i < BtScanList.size(); i++)
        {
            QString tmp = BtScanList.at(i);
            tmp = tmp.trimmed().section("\t",1,1);
            ui->BtNameListWidget->addItem(tmp);
        }
        ui->BtNameListWidget->setCurrentRow(0);
        ui->BtNameListWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:40px;}");
        ui->BtNameListWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{height:30px;}");
        ui->BtNameListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else if(signal_type == Bt_pair_signal)
    {
        if(str == "successful")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("pair success!"),
                             0,this);
             mesg.addButton(tr("OK"), QMessageBox::ActionRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             else
             mesg.move(Width/3,Height/3);
             mesg.exec();
        }
        else if(str == "failed")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("pair failed!"),
                             0,this);
             mesg.addButton(tr("OK"), QMessageBox::ActionRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             else
             mesg.move(Width/3,Height/3);
             mesg.exec();
        }
    }
    else if(signal_type == Bt_connect_signal)
    {
        if(str == "successful")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("connect success!"),
                             0,this);
             mesg.addButton(tr("OK"), QMessageBox::ActionRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             else
             mesg.move(Width/3,Height/3);
             mesg.exec();
        }
        else if(str == "failed")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("connect failed!"),
                             0,this);
             mesg.addButton(tr("OK"), QMessageBox::ActionRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             else
             mesg.move(Width/3,Height/3);
             mesg.exec();
        }
    }

    ui->BTScanBtn->setEnabled(true);
    ui->BTPairBtn->setEnabled(true);
    ui->BTConnectBtn->setEnabled(true);

    pMovie->stop();
    LoadLabel->close();

    QString strCmd = QString("hciconfig hci0 name |grep Name |awk '{print $2}'");
    QString strResult = wifi_bt_t->executeLinuxCmd(strCmd);
    //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;

    strResult.remove(0,1);
    strResult.remove(strResult.length()-2,2);
    //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;
    QString name = QString(tr("%1")).arg(strResult);
    ui->label_2->show();
    ui->BtDeviceNameLab->setText(name);

}

void bluetooth::on_retBtn_clicked()
{
    emit Mysignal();
}

void bluetooth::on_BTScanBtn_clicked()
{
    if(open_flag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open Bluetooth first!"));
        return;
    }
    ui->BtNameListWidget->clear();

    ui->BTScanBtn->setDisabled(true);
    ui->BTPairBtn->setDisabled(true);
    ui->BTConnectBtn->setDisabled(true);

    LoadLabel->show();
    pMovie->start();

    emit bluetooth_scan_msg();
}

void bluetooth::on_BTPairBtn_clicked()
{
    int count = ui->BtNameListWidget->count();
//    qDebug() << "LINE: "<< __LINE__ << "count:" << count;
    if(count == 0)
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please scan Bluetooth first!"),
                         0,this);
         mesg.addButton(tr("OK"), QMessageBox::ActionRole);
         if(screen_flag == 1)
         mesg.move(Width*2/3,Height/3);
         else
         mesg.move(Width/3,Height/3);
         mesg.exec();
        return ;
    }

    int BtNameIndex = ui->BtNameListWidget->currentRow();
    QString BtAddress = BtScanList.at(BtNameIndex);
    BtAddress = BtAddress.trimmed().section("\t",0,0);
    qDebug() << BtAddress;

    ui->BTScanBtn->setDisabled(true);
    ui->BTScanBtn->setDisabled(true);
    ui->BTScanBtn->setDisabled(true);

    LoadLabel->show();
    pMovie->start();

    emit bluetooth_pair_msg(BtAddress);
}

void bluetooth::on_BTConnectBtn_clicked()
{
    int count = ui->BtNameListWidget->count();
//    qDebug() << "LINE: "<< __LINE__ << "count:" << count;
    if(count == 0)
    {
        QMessageBox::information(this,"information",tr("Please scan Bluetooth first!"));
        return ;
    }

    LoadLabel->show();
    pMovie->start();

    QString strCmd = QString("ps -x|grep pulseaudio | grep -v grep |wc -l");
    qDebug() << "strCmd == " << strCmd;
    QString strResult = wifi_bt_t->executeLinuxCmd(strCmd);
    qDebug() << strResult;

    if(strResult == QString("0\n"))
    {
        strCmd = QString("pulseaudio --start");
        qDebug() << "strCmd == " << strCmd;
        strResult = wifi_bt_t->executeLinuxCmd(strCmd);
        qDebug() << strResult;
    }

    int BtNameIndex = ui->BtNameListWidget->currentRow();
    QString BtAddress = BtScanList.at(BtNameIndex);
    BtAddress = BtAddress.trimmed().section("\t",0,0);
    qDebug() << BtAddress;

    emit bluetooth_connect_msg(BtAddress);

    ui->BTScanBtn->setDisabled(true);
    ui->BTScanBtn->setDisabled(true);
    ui->BTScanBtn->setDisabled(true);
}

void bluetooth::language_reload()
{
    ui->retranslateUi(this);
}

void  bluetooth::blue_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = Width / realX * 2.54;
    qreal realHeight = Height / realY *2.54;
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

    ui->BTConnectBtn->setFont(font);
    ui->BTPairBtn->setFont(font);
    ui->BTScanBtn->setFont(font);
    ui->label_2->setFont(font);
    ui->pushButton->setFont(font);

}

void bluetooth::on_pushButton_clicked()
{
    if(open_flag == 0)
    {
        open_flag = 1;
        ui->pushButton->setText(tr("close"));
    }
   else
   {
        open_flag = 0;
        ui->pushButton->setText(tr("open"));
        ui->BtNameListWidget->clear();
    }
}
