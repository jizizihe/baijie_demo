#include "bluetooth.h"
#include "ui_bluetooth.h"
#include <QScreen>
#include <QTimer>
#include <QListWidgetItem>

using namespace std;
static QScreen *screen;
static int screenFlag;
static int screenWidth;
static int screenHeight;
static int openFlag;
static int scanFlag;       // 0:scan 1:stop 2:the first starting up scan
static int timerScanFlag;
static int showFlag;
static int pairingFlag;
static int connectingFlag;
static QString btAddress;
static QString btName;
static QString btConnectName;   //Store the name of the connected Bluetooth
static QString connectFlag;
static QTimer *timerConncet;   //Periodically refresh the connection information
static QTimer *timerScan;      //Periodically refresh the scan list
static QLabel *swicthLabel;
static QHBoxLayout *horLayout;

bluetooth::bluetooth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bluetooth)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    openFlag = 1;
    wifi_bt_interfaceWg = new wifi_bt_interface(this);
    bluetoothFont();
    loadLabel = new QLabel(this);
    pMovie = new QMovie("://button_image/loading.webp");
    loadLabel->setFixedSize(50, 50);
    loadLabel->setScaledContents(true);
    loadLabel->setMovie(pMovie);
    if(screenFlag == 1)
    {
        loadLabel->move(screenHeight/2,screenWidth/2);
    }
    else
    {
        loadLabel->move(screenWidth/2,screenHeight/2 );
    }

    myThread = new QThread(this);
    bluetoothThread = new bluetooth_thread();
    timerConncet = new QTimer(this);
    timerScan = new QTimer(this);

    ui->bluetooth_Switch->setCheckedColor(QColor(100, 225, 100, 150));
    switchSetText();
    connect(ui->bluetooth_Switch,SIGNAL(toggled(bool)),this,SLOT(switch_change_flag(bool)));
    connect(timerScan,SIGNAL(timeout()),this,SLOT(scan_refresh()));
    connect(timerConncet,SIGNAL(timeout()),this,SLOT(connect_refresh()));
    connect(this,SIGNAL(bluetooth_scan_msg()),bluetoothThread,SLOT(bluetooth_scan_thread()));
    connect(this,SIGNAL(bluetooth_pair_msg(QString)),bluetoothThread,SLOT(bluetooth_pair_thread(QString)));
    connect(this,SIGNAL(bluetooth_connect_msg(QString)),bluetoothThread,SLOT(bluetooth_connect_thread(QString)));
    connect(this,SIGNAL(rm_shell_msg()),wifi_bt_interfaceWg,SLOT(rm_bluetooth_shell()));
    connect(bluetoothThread,SIGNAL(send_bluetooth_msg(int, QString)),this,SLOT(recv_msg(int, QString)));

    bluetoothThread->moveToThread(myThread);
    myThread->start();
    loadLabel->show();
    pMovie->start();
    ui->stackedWidget->setCurrentIndex(0);
    ui->BtNameListWidget->setDisabled(true);
    emit bluetooth_scan_msg();                 //startup scan
    ui->btn_btScan->setText("stop");
    scanFlag = 2;
}

bluetooth::~bluetooth()
{
    delete ui;
    delete bluetoothThread;
    delete myThread;
}

void bluetooth::recv_msg(int signalType,QString str)
{
    if(signalType == btScanSignal)
    {
        if(pairingFlag == 1)
        {
            timerScanFlag = 0;
            return;
        }
        if(connectingFlag == 1)
        {
            timerScanFlag = 0;
            return;
        }
        ui->BtNameListWidget->clear();
        if(openFlag == 0)
            return ;
        btScanList.clear();
        ui->BtNameListWidget->clear();
        btPairList = databaseWg.tableShow("bluetooth");
        QString name,status;
        QStringList pariedList = btPairList;
        connect_refresh();
        if(!btPairList.isEmpty())
        {
            for(int i = 0;i<pariedList.size();i++)
            {
                name = pariedList.at(i);
                if(!QString::compare(btConnectName,name,Qt::CaseSensitive))   //Find whether there is a connected device in the paired device list
                {
                    status = tr("connected");
                    bluetoothListShow(name,status);
                    pariedList.removeAt(i);
                    pariedList.removeAt(i);
                    break;
                }
                i++;
            };
            for(int i = 0;i<pariedList.size();i++)
            {
                name = pariedList.at(i);
                status = tr("saved");
                i++;
                bluetoothListShow(name,status);
            }
        }
        QStringList scanList = str.split("\n"); int num=0;
        btScanList = scanList;
        btScanList.removeAll(QString(""));btScanList.removeAll(QString("\t"));
        btPairList = databaseWg.tableShow("bluetooth");

        for(int i = 0; i < scanList.size(); i++)
        {
            QString strScanDevice = scanList.at(i);
            QString strScanDeviceAddress = strScanDevice.trimmed().section("\t",0,0);
            QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
            if(btPairList.size()<= 0)
            {
                bluetoothListShow(strScanDeviceName,"");
            }
            else
            {
                for(int j = 0; j < btPairList.size(); j++)
                {
                    if(strScanDeviceAddress == QString(btPairList.at(j+1)))         //Remove paired devices from the scan list
                    {
                        int num1 = 0;
                        if(num > 0)
                        {
                            num1 = i-num;
                        }
                        else
                        {
                            num1 = i;
                        }
                        num++;
                        btScanList.removeAt(num1);
                        break;
                    }
                    else
                    {
                        j++;
                        if(j+1 == btPairList.size())
                            bluetoothListShow(strScanDeviceName,"");
                    }
                }
            }
        }

        if(timerScanFlag == 1)
        {
            if(scanFlag == 2)                          // After the startup scan is complete, the first scan after The Bluetooth interface is displayed
            {
                ui->BtNameListWidget->setEnabled(true);
                pMovie->stop();
                loadLabel->hide();
                ui->btn_btScan->setText("scan");
                scanFlag = 1;
                if(showFlag == 1)
                    timerScan->start(10000);
            }
            else
            {
                if(pairingFlag == 1)                      //After The Bluetooth interface is displayed, the system automatically scans every 10 seconds
                {
                    timerScanFlag = 0;
                    return;
                }
                if(showFlag == 1)
                    timerScan->start(10000);
            }
        }
        else
        {
            if(scanFlag == 0)                         //Click the button to scan, and keep scanning until click the stop button
            {
                emit bluetooth_scan_msg();
            }
            else                                      //The Bluetooth interface is displayed when the startup scan is not finished
            {
                ui->BtNameListWidget->setEnabled(true);
                pMovie->stop();
                loadLabel->hide();
                ui->btn_btScan->setText("scan");
                if(showFlag == 1)
                    timerScan->start(10000);
            }
        }
    }
    else if(signalType == btPairSignal)
    {
        ui->BtNameListWidget->setEnabled(true);
        if(str == "successful")
        {
            pairingFlag = 0;
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("pair success!"),
                             0,this);
            mesg.addButton(tr("OK"), QMessageBox::ActionRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
            int BtNameIndex = ui->BtNameListWidget->currentRow();
            BtNameIndex = BtNameIndex-btPairList.size()/2;
            btScanList.removeAt(BtNameIndex);                          //Remove paired devices from scan list
            databaseWg.insertTableTwo("bluetooth",btName,btAddress);   //Paired device insert database
            ui->BtNameListWidget->clear();
            btPairList = databaseWg.tableShow("bluetooth");
            QString name,status;
            if(!btPairList.isEmpty())
            {

                for(int i = 0;i<btPairList.size();i++)
                {
                    name = btPairList.at(i);
                    status = tr("saved");
                    i++;
                    bluetoothListShow(name,status);
                }
            }

            for(int i = 0; i < btScanList.size(); i++)
            {
                QString strScanDevice = btScanList.at(i);
                QString strScanDeviceAddress = strScanDevice.trimmed().section("\t",0,0);
                QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
                if(btPairList.size()<= 0)
                {
                    bluetoothListShow(strScanDeviceName,"");
                }
                else
                {
                    for(int j = 0; j < btPairList.size(); j++)
                    {
                        if(strScanDeviceAddress == QString(btPairList.at(j+1)))
                        {
                            break;
                        }
                        else
                        {
                            j++;
                            if(j+1 == btPairList.size())
                                bluetoothListShow(strScanDeviceName,"");
                        }

                    }
                }
            }
            timerScan->start(10000);
        }
        else if(str == "failed")
        {
            pairingFlag = 0;
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("pair failed!"),
                             0,this);
            mesg.addButton(tr("OK"), QMessageBox::ActionRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
            timerScan->start(10000);
        }
        ui->btn_btScan->setEnabled(true);
        ui->BtNameListWidget->setEnabled(true);

        pMovie->stop();
        loadLabel->hide();
    }
    else if(signalType == btConnectSignal)
    {
        ui->btn_btScan->setEnabled(true);
        ui->BtNameListWidget->setEnabled(true);
        timerScan->stop();
        if(str == "successful")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("connect success!"),
                             0,this);
            mesg.addButton(tr("OK"), QMessageBox::ActionRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();

            btPairList = databaseWg.tableShow("bluetooth");     //The paired bluetooth saved to the database
            ui->BtNameListWidget->clear();
            ui->lbl_statusValue->setText(tr("connect"));
            ui->lbl_connectDeviceValue->setText(btName);
            btConnectName=btName;
            ui->stackedWidget->setCurrentIndex(1);
            ui->stackedWidget_2->setCurrentIndex(1);
            timerConncet->start(3000);
            bluetoothConnectStatus();
            QString name,status;
            if(!btPairList.isEmpty())
            {
                QStringList pairList = btPairList;
                for(int i = 0;i<pairList.size();i++)
                {
                    name = pairList.at(i);
                    if(!QString::compare(btConnectName,name,Qt::CaseSensitive))
                    {
                        status = "connected";
                        bluetoothListShow(name,status);
                        pairList.removeAt(i);
                        pairList.removeAt(i);
                        break;
                    }
                    i++;
                }
                for(int i = 0;i<pairList.size();i++)
                {
                    name = pairList.at(i);
                    status = tr("saved");
                    i++;
                    bluetoothListShow(name,status);
                }
            }
            for(int i = 0; i < btScanList.size(); i++)
            {
                QString strScanDevice = btScanList.at(i);
                QString strScanDeviceAddress = strScanDevice.trimmed().section("\t",0,0);
                QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
                if(btPairList.size()<= 0)
                {
                    bluetoothListShow(strScanDeviceName,"");
                }
                else
                {
                    for(int j = 0; j < btPairList.size(); j++)
                    {
                        if(strScanDeviceAddress == QString(btPairList.at(j+1)))
                        {
                            break;
                        }
                        else
                        {
                            j++;
                            if(j+1 == btPairList.size())
                                bluetoothListShow(strScanDeviceName,"");
                        }

                    }
                }
            }
            connectingFlag = 0;
        }
        else if(str == "failed")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("connect failed!"),
                             0,this);
            mesg.addButton(tr("OK"), QMessageBox::ActionRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();

            ui->BtNameListWidget->clear();
            btPairList = databaseWg.tableShow("bluetooth");
            QString name,status;
            if(!btPairList.isEmpty())
            {
                for(int i = 0;i<btPairList.size();i++)
                {
                    name = btPairList.at(i);
                    status = tr("saved");
                    i++;
                    bluetoothListShow(name,status);
                }
            }
            for(int i = 0; i < btScanList.size(); i++)
            {
                QString strScanDevice = btScanList.at(i);
                QString strScanDeviceAdrss = strScanDevice.trimmed().section("\t",0,0);
                QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
                if(btPairList.size()<= 0)
                {
                    bluetoothListShow(strScanDeviceName,"");
                }
                else
                {
                    for(int j = 0; j < btPairList.size(); j++)
                    {
                        if(strScanDeviceAdrss == QString(btPairList.at(j+1)))
                        {
                            break;
                        }
                        else
                        {
                            j++;
                            if(j+1 == btPairList.size())
                                bluetoothListShow(strScanDeviceName,"");
                        }

                    }
                }
            }
            connectingFlag = 0;
            ui->lbl_statusValue->setText(tr("no connect"));
            ui->lbl_addressValue->setText(tr("no"));
            ui->lbl_typeValue->setText(tr("no"));
            ui->lbl_connectDeviceValue->setText(btName);
            ui->stackedWidget->setCurrentIndex(1);
            ui->stackedWidget_2->setCurrentIndex(1);btConnectName = "";
        }
        else if(str == "connected")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("please disconnect the \"%1\" device first!").arg(btConnectName),
                             0,this);
            mesg.addButton(tr("OK"), QMessageBox::ActionRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
        }
        ui->btn_btScan->setEnabled(true);
        ui->BtNameListWidget->setEnabled(true);

        pMovie->stop();
        loadLabel->hide();
    }
}

void bluetooth::on_btn_ret_clicked()
{
    timerScanFlag = 0;
    showFlag = 0;
    emit bluetooth_back_msg();
    timerConncet->stop();
    emit rm_shell_msg();
}

void bluetooth::on_btn_btScan_clicked()
{
    timerConncet->stop();
    if(openFlag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open Bluetooth first!"));
        return;
    }
    int ind = ui->stackedWidget->currentIndex();
    if(ind == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
        timerScan->start(10000);
        return;
    }

    if(!loadLabel->isHidden())
    {
        loadLabel->hide();
        pMovie->stop();
        ui->btn_btScan->setText(tr("scan"));
        scanFlag = 1;
        return;
    }

    loadLabel->show();
    pMovie->start();
    emit bluetooth_scan_msg();
    ui->btn_btScan->setText(tr("stop"));
    scanFlag = 0;
    timerScanFlag = 0;
}

void bluetooth::languageReload()
{
    ui->retranslateUi(this);
}

void  bluetooth::bluetoothFont()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = screenWidth / realX * 2.54;
    qreal realHeight = screenHeight / realY *2.54;
    QFont font;
    if(screenFlag)
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

    ui->btn_btScan->setFont(font);
    ui->lbl_deviceName->setFont(font);
    ui->lbl_deviceNameValue->setFont(font);
    ui->btn_disconnect->setFont(font);
    ui->btn_remove->setFont(font);
    ui->lbl_address->setFont(font);
    ui->lbl_type->setFont(font);
    ui->lbl_status->setFont(font);
    ui->lbl_connectDevice->setFont(font);
    ui->lbl_addressValue->setFont(font);
    ui->lbl_connectDeviceValue->setFont(font);
    ui->lbl_typeValue->setFont(font);
    ui->lbl_statusValue->setFont(font);
    ui->lbl_bluetooth->setFont(font);
}

void bluetooth::switch_change_flag(bool flag)
{
    flag = ui->bluetooth_Switch->isToggled();
    if(flag == 1)
    {
        openFlag = 1;
        ui->BtNameListWidget->setDisabled(true);
        loadLabel->show();
        pMovie->start();
        emit bluetooth_scan_msg();
        ui->btn_btScan->setText(tr("stop"));
        timerScan->start(10000);
        swicthLabel->setText(tr("  off"));
        swicthLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        swicthLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    }
    else
    {
        timerScanFlag = 0;
        openFlag = 0;
        loadLabel->hide();
        pMovie->stop();
        ui->BtNameListWidget->clear();
        ui->stackedWidget->setCurrentIndex(0);
        ui->BtNameListWidget->setEnabled(true);
        timerConncet->stop();timerScan->stop();
        if(connectFlag != "1")
        {
            wifi_bt_interfaceWg->bluetoothDisconnect(btAddress);
            ui->lbl_connectDeviceValue->setText(tr("no"));
            ui->stackedWidget_2->setCurrentIndex(0);
            btConnectName = "";
            ui->lbl_statusValue->setText(tr("not connect"));
        }
        ui->btn_btScan->setText(tr("scan"));
        swicthLabel->setText(tr("on   "));
        swicthLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        swicthLabel->setStyleSheet("color: rgba(255, 255, 255,200);");
    }
}

void bluetooth::bluetoothListShow(QString name,QString status)      //BtNameListWidget Stores the device name and device status
{
    QHBoxLayout *horLayout = new QHBoxLayout();
    horLayout->setContentsMargins(5,0,0,0);
    QWidget *widget=new QWidget(this);
    QLabel *nameLabel = new QLabel(widget);
    QLabel *statusLabel = new QLabel(widget);
    QListWidgetItem *item = new QListWidgetItem();
    nameLabel->setText(name);
    statusLabel->setText(tr(status.toUtf8()));
    horLayout->addWidget(nameLabel);
    horLayout->addWidget(statusLabel);
    widget->setLayout(horLayout);
    horLayout->setStretchFactor(nameLabel,4);
    horLayout->setStretchFactor(statusLabel,1);
    horLayout->setSpacing(0);
    ui->BtNameListWidget->addItem(item);
    ui->BtNameListWidget->setItemWidget(item, widget);
    ui->BtNameListWidget->setCurrentRow(0);
    ui->BtNameListWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:40px;}");
    ui->BtNameListWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{height:30px;}");
    ui->BtNameListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void bluetooth::showEvent(QShowEvent *event)
{
    ui->stackedWidget->setCurrentIndex(0);
    QString name = wifi_bt_interfaceWg->getBluetoothName();
    ui->lbl_deviceNameValue->setText(name);
    showFlag = 1;timerScan->start(10000);
    QWidget::showEvent(event);
}

void bluetooth::on_BtNameListWidget_itemClicked(QListWidgetItem *item)
{
    if(!loadLabel->isHidden())
    {
        if(ui->BtNameListWidget->count()==0)
        {
            return;
        }
        else
        {
            on_btn_btScan_clicked();
        }
    }
    QListWidgetItem *item1 = ui->BtNameListWidget->currentItem();
    QWidget* pwig = ui->BtNameListWidget->itemWidget(item1);
    QList<QLabel*> labelList = pwig->findChildren<QLabel*>();
    QString status = labelList.at(1)->text();
    QString name = labelList.at(0)->text();btName = name;
    if(status == tr("connected"))
    {
        bluetoothConnectStatus();
        ui->lbl_statusValue->setText(tr("connect"));
        ui->lbl_connectDeviceValue->setText(btName);
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(1);timerConncet->start(3000);
        return;
    }

    btPairList = databaseWg.tableShow("bluetooth");
    int BtNameIndex = ui->BtNameListWidget->currentRow();
    if(!btPairList.isEmpty())
    {
        if(btPairList.size()/2>BtNameIndex)    //Determine whether clicked on to saved devices
        {
            ui->BtNameListWidget->setEnabled(false);
            ui->btn_btScan->setDisabled(true);
            loadLabel->show();
            pMovie->start();
            //connect_refresh();
            if(connectFlag != "1")
            {
                QString address = databaseWg.selectTableName("bluetooth",connectFlag);
                wifi_bt_interfaceWg->bluetoothDisconnect(address);
                ui->lbl_statusValue->setText(tr("not connect"));
                ui->lbl_connectDeviceValue->setText(tr("no"));
                ui->stackedWidget->setCurrentIndex(0);
                btConnectName = "";
            }
            myThread->terminate();                        //Pausing the scan process,stop the scan
            delete bluetoothThread;
            connectingFlag = 1;btName = name;
            btAddress = databaseWg.selectTableName("bluetooth",name);

            QString strCmd = QString("ps -x|grep pulseaudio | grep -v grep |wc -l");
            QString strResult = wifi_bt_interfaceWg->executeLinuxCmd(strCmd);

            if(strResult == QString("0\n"))
            {
                strCmd = QString("pulseaudio --start");
                strResult = wifi_bt_interfaceWg->executeLinuxCmd(strCmd);
            }

            // myThread = new QThread(this);
            bluetoothThread = new bluetooth_thread();       //Create a new process and create a connection
            connect(this,SIGNAL(bluetooth_pair_msg(QString)),bluetoothThread,SLOT(bluetooth_pair_thread(QString)));
            connect(this,SIGNAL(bluetooth_connect_msg(QString)),bluetoothThread,SLOT(bluetooth_connect_thread(QString)));
            connect(this,SIGNAL(bluetooth_scan_msg()),bluetoothThread,SLOT(bluetooth_scan_thread()));
            connect(bluetoothThread,SIGNAL(send_bluetooth_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
            bluetoothThread->moveToThread(myThread);
            myThread->start();
            emit bluetooth_connect_msg(btAddress);

            ui->btn_btScan->setDisabled(true);
            return;
        }
    }
    pairingFlag = 1;
    ui->BtNameListWidget->setEnabled(false);
    ui->btn_btScan->setDisabled(true);
    BtNameIndex = BtNameIndex-btPairList.size()/2;
    btAddress = btScanList.at(BtNameIndex);
    btName = btAddress.trimmed().section("\t",1,1);       //Click on the Bluetooth name and address
    btAddress = btAddress.trimmed().section("\t",0,0);
    emit bluetooth_pair_msg(btAddress);
    loadLabel->show();
    pMovie->start();
}

void bluetooth::connect_refresh()
{
    connectFlag = wifi_bt_interfaceWg->bluetoothConnectFlag();
    if(connectFlag == "1")
    {
        btConnectName = "";
        ui->lbl_connectDeviceValue->setText(tr("no"));
        ui->lbl_statusValue->setText(tr("not connect"));
    }
    else
    {
        btConnectName = connectFlag;
        ui->lbl_connectDeviceValue->setText(btConnectName);
        ui->lbl_statusValue->setText(tr("connect"));
    }
}

void bluetooth::on_btn_disconnect_clicked()
{
    if(btConnectName.isEmpty())
    {
        QMessageBox::information(this,"information",tr("No device is connected!"));
    }
    else
    {
        wifi_bt_interfaceWg->bluetoothDisconnect(btAddress);
        QMessageBox::information(this,"information",tr("Disconnect successful!"));
        ui->lbl_statusValue->setText(tr("not connect"));
        ui->lbl_connectDeviceValue->setText(tr("no"));
        timerConncet->stop();timerScan->start(10000);
        ui->stackedWidget->setCurrentIndex(0);
        btConnectName = "";
        QString name,status;
        ui->BtNameListWidget->clear();
        btPairList = databaseWg.tableShow("bluetooth");
        if(!btPairList.isEmpty())                          //Refreshing the list status
        {
            for(int i = 0;i<btPairList.size();i++)
            {
                name = btPairList.at(i);
                status = tr("saved");
                i++;
                bluetoothListShow(name,status);
            }
        }
        for(int i = 0; i < btScanList.size(); i++)
        {
            QString strScanDevice = btScanList.at(i);
            QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
            bluetoothListShow(strScanDeviceName,"");
        }
    }
}

void bluetooth::on_btn_remove_clicked()
{
    QString address = databaseWg.selectTableName("bluetooth",btName);
    wifi_bt_interfaceWg->bluetoothRemove(address);
    QMessageBox::information(this,"information",tr("Remove successful!"));
    databaseWg.deleteTableName("bluetooth",btName);
    ui->lbl_statusValue->setText(tr("not connect"));
    ui->lbl_connectDeviceValue->setText(tr("no"));
    ui->BtNameListWidget->clear();
    btPairList = databaseWg.tableShow("bluetooth");
    QString name,status;
    if(!btPairList.isEmpty())
    {
        for(int i = 0;i<btPairList.size();i++)
        {
            name = btPairList.at(i);
            status = tr("saved");
            i++;
            bluetoothListShow(name,status);
        }
    }
    for(int i = 0; i < btScanList.size(); i++)
    {
        QString strScanDevice = btScanList.at(i);
        QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
        bluetoothListShow(strScanDeviceName,"");
    }

    timerConncet->stop();timerScan->start(10);
    ui->stackedWidget->setCurrentIndex(0);
}

void bluetooth::scan_refresh()
{
    if(pairingFlag == 1)
    {
        timerScanFlag = 0;
        timerScan->stop();
        return;
    }
    if(connectingFlag == 1)
    {
        timerScanFlag = 0;
        timerScan->stop();
        return;
    }
    if(ui->stackedWidget->currentIndex() == 0)
    {
        timerScanFlag = 1;
        emit bluetooth_scan_msg();
        timerScan->stop();
    }
}

void bluetooth::bluetoothConnectStatus()
{
    QString tmp;
    QString strResult = wifi_bt_interfaceWg->executeLinuxCmdBluetoothConnect("hciconfig");
    QStringList list = strResult.split("\n");
    list.removeAll("");
    for(int i=0; i<list.size();i++)
    {
        QString str = list.at(i);
        if(str.contains("Type",Qt::CaseInsensitive))
        {
            tmp = str.section(' ',1,1);
            ui->lbl_typeValue->setText(tmp);
        }
        else if(str.contains("Address",Qt::CaseInsensitive))
        {
            tmp = str.section(' ',2,2);
            ui->lbl_addressValue->setText(tmp);
        }
    }
    if(!ui->lbl_statusValue->text().contains(tr("connect"),Qt::CaseInsensitive))
    {
        ui->lbl_statusValue->setText(tr("not connect"));
    }
}

void bluetooth::switchSetText()
{
    swicthLabel = new QLabel(ui->bluetooth_Switch);
    horLayout = new QHBoxLayout();
    swicthLabel->setText(tr("  off"));
    horLayout->addWidget(swicthLabel);
    swicthLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    swicthLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    ui->bluetooth_Switch->setLayout(horLayout);
}
