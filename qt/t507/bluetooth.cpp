#include "bluetooth.h"
#include "ui_bluetooth.h"

using namespace std;

bluetooth::bluetooth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bluetooth)
{
    ui->setupUi(this);

    wifi_bt_t = new wifi_bt_interface(this);

    ui->BtSwitch->setToggle(true);
    ui->BtSwitch->setCheckedColor(QColor(100, 225, 100));
    connect(ui->BtSwitch,SIGNAL(toggled(bool)),this,SLOT(BtnChange_flag(bool)));


    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();
    //qDebug()<<screen_width<<screen_height;

    LoadLabel = new QLabel(this);
    //LoadLabel->resize(100,100);
    LoadLabel->move(screen_width/2 - 150,screen_height/2 -50);
    pMovie = new QMovie("://t507_button_image/loading.webp");
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
    btflag = ui->BtSwitch->isToggled();

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
        flag = ui->BtSwitch->isToggled();
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
            QMessageBox::information(this,"information",tr("pair success!"));
        }
        else if(str == "failed")
        {
            QMessageBox::critical(this,"information",tr("pair failed!"));
        }
    }
    else if(signal_type == Bt_connect_signal)
    {
        if(str == "successful")
        {
            QMessageBox::information(this,"information",tr("connect success!"));
        }
        else if(str == "failed")
        {
            QMessageBox::critical(this,"information",tr("connect failed!"));
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

    ui->BtDeviceNameLab->setText(strResult);

}

void bluetooth::on_retBtn_clicked()
{
    emit Mysignal();
}

void bluetooth::on_BTScanBtn_clicked()
{
    //qDebug() << "BTScanBtn_clicked";
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
        QMessageBox::information(this,"information",tr("Please scan Bluetooth first!"));
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

