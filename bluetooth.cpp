#include "bluetooth.h"
#include "ui_bluetooth.h"
#include <QScreen>
#include <QTimer>
#include <QListWidgetItem>

static int g_screenWidth;
static int g_screenHeight;
static int g_openFlag;
static int g_scanFlag;       // 0:Scanning         1:Stop        2:The first starting up scan
static int g_timerScanFlag;  // 0:Not Timed automatic scan   1:Timed automatic scan
static int g_showFlag;       // 0:Close bluetooth interface  1:Show bluetooth interface
static int g_pairingFlag;
static int g_connectingFlag;
static QString g_btAddress;
static QString g_btName;
static QString g_btConnectName;   // Store the name of the connected bluetooth
static QString g_connectFlag;
static QTimer *g_timerConncet;    // Periodically refresh the connection information
static QTimer *g_timerScan;       // Periodically refresh the scan list
static QLabel *g_swicthLabel;
static QHBoxLayout *g_horLayout;

bluetooth::bluetooth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bluetooth)
{
    ui->setupUi(this);
    ui->bluetooth_Switch->setCheckedColor(QColor(100, 225, 100, 150));
    ui->stackedWidget->setCurrentIndex(0);
    QScreen *screen = qApp->primaryScreen();
    g_screenWidth = screen->size().width();
    g_screenHeight = screen->size().height();
    g_openFlag = 1;
    g_bluetoothInterface = new bluetooth_interface(this);   
    g_loadLabel = new QLabel(this);
    g_loadMovie = new QMovie("://button_image/loading.webp");
    g_loadLabel->setFixedSize(50, 50);
    g_loadLabel->setScaledContents(true);
    g_loadLabel->setMovie(g_loadMovie);
    g_loadLabel->move(g_screenWidth/2,g_screenHeight/2);
    g_myThread = new QThread(this);
    g_bluetoothThread = new bluetooth_thread();
    g_timerConncet = new QTimer(this);
    g_timerScan = new QTimer(this);

    setSwitchText();setBluetoothFont();
    connect(ui->bluetooth_Switch,SIGNAL(toggled(bool)),this,SLOT(switch_change_flag(bool)));
    connect(g_timerScan,SIGNAL(timeout()),this,SLOT(scan_refresh()));
    connect(g_timerConncet,SIGNAL(timeout()),this,SLOT(connect_info_refresh()));
    connect(this,SIGNAL(bluetooth_scan_msg()),g_bluetoothThread,SLOT(bluetooth_scan_thread()));
    connect(this,SIGNAL(bluetooth_pair_msg(QString)),g_bluetoothThread,SLOT(bluetooth_pair_thread(QString)));
    connect(this,SIGNAL(bluetooth_connect_msg(QString)),g_bluetoothThread,SLOT(bluetooth_connect_thread(QString)));
    connect(this,SIGNAL(rm_shell_msg()),g_bluetoothInterface,SLOT(rm_bluetooth_shell()));
    connect(g_bluetoothThread,SIGNAL(send_bluetooth_msg(int, QString)),this,SLOT(recv_msg(int, QString)));

    g_bluetoothThread->moveToThread(g_myThread);
    g_myThread->start();
    g_loadLabel->show();
    g_loadMovie->start();
    emit bluetooth_scan_msg();                 // Startup scan
    ui->btn_btScan->setText(tr("stop"));
    g_scanFlag = 2;
}

bluetooth::~bluetooth()
{
    delete ui;
    delete g_bluetoothThread;
    delete g_myThread;
}

void bluetooth::recv_msg(int signalType,QString str)
{
    if(signalType == EnumBtScanSignal)
    {
        if(g_pairingFlag == 1)
        {
            g_timerScanFlag = 0;
            return;
        }
        if(g_connectingFlag == 1)
        {
            g_timerScanFlag = 0;
            return;
        }
        ui->BtNameListWidget->clear();
        if(g_openFlag == 0)
            return ;
        g_btScanList.clear();
        ui->BtNameListWidget->clear();
        g_btPairList = g_database.tableShow("bluetooth");
        QString name,status;
        QStringList pariedList = g_btPairList;
        connect_info_refresh();
        if(!g_btPairList.isEmpty())
        {
            for(int i = 0;i<pariedList.size();i++)
            {
                name = pariedList.at(i);
                if(!QString::compare(g_btConnectName,name,Qt::CaseSensitive))   // Find whether there is a connected device in the paired device list
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
        QStringList scanList = str.split("\n");
        int num=0;
        g_btScanList = scanList;
        g_btScanList.removeAll(QString(""));
        g_btScanList.removeAll(QString("\t"));
        g_btPairList = g_database.tableShow("bluetooth");

        for(int i = 0; i < scanList.size(); i++)
        {
            QString strScanDevice = scanList.at(i);
            QString strScanDeviceAddress = strScanDevice.trimmed().section("\t",0,0);
            QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
            if(g_btPairList.size()<= 0)
            {
                bluetoothListShow(strScanDeviceName,"");
            }
            else
            {
                for(int j = 0; j < g_btPairList.size(); j++)
                {
                    if(strScanDeviceAddress == QString(g_btPairList.at(j+1)))         // Remove paired devices from the scan list
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
                        g_btScanList.removeAt(num1);
                        break;
                    }
                    else
                    {
                        j++;
                        if(j+1 == g_btPairList.size())
                            bluetoothListShow(strScanDeviceName,"");
                    }
                }
            }
        }

        if(g_timerScanFlag == 1)
        {
            if(g_scanFlag == 2)                            // After the startup scan is complete, the first scan after the bluetooth interface is displayed
            {
                ui->BtNameListWidget->setEnabled(true);
                g_loadMovie->stop();
                g_loadLabel->hide();
                ui->btn_btScan->setText(tr("scan"));
                g_scanFlag = 1;
                if(g_showFlag == 1)
                    g_timerScan->start(10000);
            }
            else
            {
                if(g_pairingFlag == 1)                     // After the bluetooth interface is displayed, the system automatically scans every 10 seconds
                {
                    g_timerScanFlag = 0;
                    return;
                }
                if(g_showFlag == 1)
                    g_timerScan->start(10000);
            }
        }
        else
        {
            if(g_scanFlag == 0)                         // Click the button to scan, and keep scanning until click the stop button
            {
                emit bluetooth_scan_msg();
            }
            else                                       // The bluetooth interface is displayed when the startup scan is not finished
            {
                ui->BtNameListWidget->setEnabled(true);
                g_loadMovie->stop();
                g_loadLabel->hide();
                ui->btn_btScan->setText(tr("scan"));
                if(g_showFlag == 1)
                    g_timerScan->start(10000);
            }
        }
    }
    else if(signalType == EnumBtPairSignal)
    {
        ui->BtNameListWidget->setEnabled(true);
        if(str == "successful")
        {
            g_pairingFlag = 0;
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Pair success!"),
                             0,this);
            mesg.addButton(tr("OK"), QMessageBox::ActionRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
            int BtNameIndex = ui->BtNameListWidget->currentRow();
            BtNameIndex = BtNameIndex-g_btPairList.size()/2;
            g_btScanList.removeAt(BtNameIndex);                          // Remove paired devices from scan list
            g_database.insertTableTwo("bluetooth",g_btName,g_btAddress);   // Paired device insert database
            ui->BtNameListWidget->clear();
            g_btPairList = g_database.tableShow("bluetooth");
            QString name,status;
            if(!g_btPairList.isEmpty())
            {

                for(int i = 0;i<g_btPairList.size();i++)
                {
                    name = g_btPairList.at(i);
                    status = tr("saved");
                    i++;
                    bluetoothListShow(name,status);
                }
            }

            for(int i = 0; i < g_btScanList.size(); i++)
            {
                QString strScanDevice = g_btScanList.at(i);
                QString strScanDeviceAddress = strScanDevice.trimmed().section("\t",0,0);
                QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
                if(g_btPairList.size()<= 0)
                {
                    bluetoothListShow(strScanDeviceName,"");
                }
                else
                {
                    for(int j = 0; j < g_btPairList.size(); j++)
                    {
                        if(strScanDeviceAddress == QString(g_btPairList.at(j+1)))
                        {
                            break;
                        }
                        else
                        {
                            j++;
                            if(j+1 == g_btPairList.size())
                                bluetoothListShow(strScanDeviceName,"");
                        }

                    }
                }
            }
            g_timerScan->start(10000);
        }
        else if(str == "failed")
        {
            g_pairingFlag = 0;
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Pair failed!"),
                             0,this);
            mesg.addButton(tr("OK"), QMessageBox::ActionRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
            g_timerScan->start(10000);
        }
        ui->btn_btScan->setEnabled(true);
        ui->BtNameListWidget->setEnabled(true);

        g_loadMovie->stop();
        g_loadLabel->hide();
    }
    else if(signalType == EnumBtConnectSignal)
    {
        ui->btn_btScan->setEnabled(true);
        ui->BtNameListWidget->setEnabled(true);
        g_timerScan->stop();
        if(str == "successful")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Connect success!"),
                             0,this);
            mesg.addButton(tr("OK"), QMessageBox::ActionRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();

            g_btPairList = g_database.tableShow("bluetooth");     // The paired bluetooth saved to the database
            ui->BtNameListWidget->clear();
            ui->lbl_statusValue->setText(tr("connect"));
            ui->lbl_connectDeviceValue->setText(g_btName);
            g_btConnectName=g_btName;
            ui->stackedWidget->setCurrentIndex(1);
            ui->stackedWidget_2->setCurrentIndex(1);
            g_timerConncet->start(3000);
            bluetoothConnectStatus();
            QString name,status;
            if(!g_btPairList.isEmpty())
            {
                QStringList pairList = g_btPairList;
                for(int i = 0;i<pairList.size();i++)
                {
                    name = pairList.at(i);
                    if(!QString::compare(g_btConnectName,name,Qt::CaseSensitive))
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
            for(int i = 0; i < g_btScanList.size(); i++)
            {
                QString strScanDevice = g_btScanList.at(i);
                QString strScanDeviceAddress = strScanDevice.trimmed().section("\t",0,0);
                QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
                if(g_btPairList.size()<= 0)
                {
                    bluetoothListShow(strScanDeviceName,"");
                }
                else
                {
                    for(int j = 0; j < g_btPairList.size(); j++)
                    {
                        if(strScanDeviceAddress == QString(g_btPairList.at(j+1)))
                        {
                            break;
                        }
                        else
                        {
                            j++;
                            if(j+1 == g_btPairList.size())
                                bluetoothListShow(strScanDeviceName,"");
                        }

                    }
                }
            }
            g_connectingFlag = 0;
        }
        else if(str == "failed")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Connect failed!"),
                             0,this);
            mesg.addButton(tr("OK"), QMessageBox::ActionRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();

            ui->BtNameListWidget->clear();
            g_btPairList = g_database.tableShow("bluetooth");
            QString name,status;
            if(!g_btPairList.isEmpty())
            {
                for(int i = 0;i<g_btPairList.size();i++)
                {
                    name = g_btPairList.at(i);
                    status = tr("saved");
                    i++;
                    bluetoothListShow(name,status);
                }
            }
            for(int i = 0; i < g_btScanList.size(); i++)
            {
                QString strScanDevice = g_btScanList.at(i);
                QString strScanDeviceAdrss = strScanDevice.trimmed().section("\t",0,0);
                QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
                if(g_btPairList.size()<= 0)
                {
                    bluetoothListShow(strScanDeviceName,"");
                }
                else
                {
                    for(int j = 0; j < g_btPairList.size(); j++)
                    {
                        if(strScanDeviceAdrss == QString(g_btPairList.at(j+1)))
                        {
                            break;
                        }
                        else
                        {
                            j++;
                            if(j+1 == g_btPairList.size())
                                bluetoothListShow(strScanDeviceName,"");
                        }
                    }
                }
            }
            g_connectingFlag = 0;
            ui->lbl_statusValue->setText(tr("no connect"));
            ui->lbl_addressValue->setText(tr("no"));
            ui->lbl_typeValue->setText(tr("no"));
            ui->lbl_connectDeviceValue->setText(g_btName);
            ui->stackedWidget->setCurrentIndex(1);
            ui->stackedWidget_2->setCurrentIndex(1);
            g_btConnectName = "";
        }
        else if(str == "connected")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Please disconnect the \"%1\" device first!").arg(g_btConnectName),
                             0,this);
            mesg.addButton(tr("OK"), QMessageBox::ActionRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
        }
        ui->btn_btScan->setEnabled(true);
        ui->BtNameListWidget->setEnabled(true);

        g_loadMovie->stop();
        g_loadLabel->hide();
    }
}

void bluetooth::on_btn_ret_clicked()
{
    g_timerScanFlag = 0;
    g_showFlag = 0;
    emit bluetooth_back_msg();
    g_timerConncet->stop();
    emit rm_shell_msg();
}

void bluetooth::on_btn_btScan_clicked()
{
    g_timerConncet->stop();
    if(g_openFlag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open bluetooth first!"));
        return;
    }
    int index = ui->stackedWidget->currentIndex();
    if(index == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
        g_timerScan->start(10000);
        return;
    }

    if(!g_loadLabel->isHidden())
    {
        g_loadLabel->hide();
        g_loadMovie->stop();
        ui->btn_btScan->setText(tr("scan"));
        g_scanFlag = 1;
        return;
    }

    g_loadLabel->show();
    g_loadMovie->start();
    emit bluetooth_scan_msg();
    ui->btn_btScan->setText(tr("stop"));
    g_scanFlag = 0;
    g_timerScanFlag = 0;
}

void bluetooth::languageReload()
{
    ui->retranslateUi(this);
    int flag = ui->bluetooth_Switch->isToggled();
    if(flag == 1)
    ui->bluetooth_Switch->setToggle(true);
    else
    ui->bluetooth_Switch->setToggle(false);
}

void  bluetooth::setBluetoothFont()
{
    QScreen *screen = qApp->primaryScreen();
    qreal realX = screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;
    if(realWidth < 15)
    {
        font.setPointSize(12);
    }
    else if (realWidth < 18)
    {
        font.setPointSize(14);
    }
    else
    {
        font.setPointSize(17);
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
        g_bluetoothInterface->bluetoothEnableFlag(true);
        g_openFlag = 1;
        if(g_showFlag == 1)
        {
            g_loadLabel->show();
            g_loadMovie->start();
            g_timerScan->start(10000);
            emit bluetooth_scan_msg();
            ui->btn_btScan->setText(tr("stop"));
        }
        g_swicthLabel->setText(tr("  off"));
        g_swicthLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        g_swicthLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    }
    else
    {
        g_bluetoothInterface->bluetoothEnableFlag(false);
        g_timerScanFlag = 0;
        g_openFlag = 0;
        g_loadLabel->hide();
        g_loadMovie->stop();
        ui->BtNameListWidget->clear();
        ui->stackedWidget->setCurrentIndex(0);
        ui->BtNameListWidget->setEnabled(true);
        g_timerConncet->stop();
        g_timerScan->stop();
        if(g_connectFlag != "1")
        {
            g_bluetoothInterface->bluetoothDisconnect(g_btAddress);
            ui->lbl_connectDeviceValue->setText(tr("no"));
            ui->stackedWidget_2->setCurrentIndex(0);
            g_btConnectName = "";
            ui->lbl_statusValue->setText(tr("not connect"));
        }
        ui->btn_btScan->setText(tr("scan"));
        g_swicthLabel->setText(tr("on   "));
        g_swicthLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        g_swicthLabel->setStyleSheet("color: rgba(255, 255, 255,200);");
    }
}

void bluetooth::bluetoothListShow(QString name,QString status)      // BtNameListWidget dispaly the device name and device status
{
    QHBoxLayout *g_horLayout = new QHBoxLayout();
    g_horLayout->setContentsMargins(5,0,0,0);
    QWidget *widget=new QWidget(this);
    QLabel *nameLabel = new QLabel(widget);
    QLabel *statusLabel = new QLabel(widget);
    QListWidgetItem *item = new QListWidgetItem();
    nameLabel->setText(name);
    statusLabel->setText(tr(status.toUtf8()));
    g_horLayout->addWidget(nameLabel);
    g_horLayout->addWidget(statusLabel);
    widget->setLayout(g_horLayout);
    g_horLayout->setStretchFactor(nameLabel,4);
    g_horLayout->setStretchFactor(statusLabel,1);
    g_horLayout->setSpacing(0);
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
    QString name = g_bluetoothInterface->getBluetoothName();
    ui->lbl_deviceNameValue->setText(name);
    g_showFlag = 1;
    g_timerScan->start(10000);
    QWidget::showEvent(event);
}

void bluetooth::on_BtNameListWidget_itemClicked(QListWidgetItem *item)
{
    if(!g_loadLabel->isHidden())
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
    QString name = labelList.at(0)->text();
    g_btName = name;
    if(status == tr("connected"))
    {
        bluetoothConnectStatus();
        ui->lbl_statusValue->setText(tr("connect"));
        ui->lbl_connectDeviceValue->setText(g_btName);
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(1);
        g_timerConncet->start(3000);
        return;
    }

    g_btPairList = g_database.tableShow("bluetooth");
    int btNameIndex = ui->BtNameListWidget->currentRow();
    if(!g_btPairList.isEmpty())
    {
        if(g_btPairList.size()/2 > btNameIndex)    // Determine whether clicked on to saved devices
        {
            ui->BtNameListWidget->setEnabled(false);
            ui->btn_btScan->setDisabled(true);
            g_loadLabel->show();
            g_loadMovie->start();

            if(g_connectFlag != "1")
            {
                QString address = g_database.selectTableName("bluetooth",g_connectFlag);
                g_bluetoothInterface->bluetoothDisconnect(address);
                ui->lbl_statusValue->setText(tr("not connect"));
                ui->lbl_connectDeviceValue->setText(tr("no"));
                ui->stackedWidget->setCurrentIndex(0);
                g_btConnectName = "";
            }
            g_myThread->terminate();                        // Pausing the scan process,stop the scan
            delete g_bluetoothThread;
            g_connectingFlag = 1;
            g_btName = name;
            g_btAddress = g_database.selectTableName("bluetooth",name);

            QString strCmd = QString("ps -x|grep pulseaudio | grep -v grep |wc -l");
            QString strResult = g_bluetoothInterface->executeLinuxCmd(strCmd);

            if(strResult == QString("0\n"))
            {
                strCmd = QString("pulseaudio --start");
                strResult = g_bluetoothInterface->executeLinuxCmd(strCmd);
            }

            g_bluetoothThread = new bluetooth_thread();       // Create a new process and create a connection
            connect(this,SIGNAL(bluetooth_pair_msg(QString)),g_bluetoothThread,SLOT(bluetooth_pair_thread(QString)));
            connect(this,SIGNAL(bluetooth_connect_msg(QString)),g_bluetoothThread,SLOT(bluetooth_connect_thread(QString)));
            connect(this,SIGNAL(bluetooth_scan_msg()),g_bluetoothThread,SLOT(bluetooth_scan_thread()));
            connect(g_bluetoothThread,SIGNAL(send_bluetooth_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
            g_bluetoothThread->moveToThread(g_myThread);
            g_myThread->start();
            emit bluetooth_connect_msg(g_btAddress);

            ui->btn_btScan->setDisabled(true);
            return;
        }
    }
    g_pairingFlag = 1;
    ui->BtNameListWidget->setEnabled(false);
    ui->btn_btScan->setDisabled(true);
    btNameIndex = btNameIndex-g_btPairList.size()/2;
    g_btAddress = g_btScanList.at(btNameIndex);
    g_btName = g_btAddress.trimmed().section("\t",1,1);       // Click on the bluetooth name and address
    g_btAddress = g_btAddress.trimmed().section("\t",0,0);
    emit bluetooth_pair_msg(g_btAddress);
    g_loadLabel->show();
    g_loadMovie->start();
}

void bluetooth::connect_info_refresh()
{
    g_connectFlag = g_bluetoothInterface->bluetoothConnectFlag();
    if(g_connectFlag == "1")
    {
        g_btConnectName = "";
        ui->lbl_connectDeviceValue->setText(tr("no"));
        ui->lbl_statusValue->setText(tr("not connect"));
    }
    else
    {
        g_btConnectName = g_connectFlag;
        ui->lbl_connectDeviceValue->setText(g_btConnectName);
        ui->lbl_statusValue->setText(tr("connect"));
    }
}

void bluetooth::on_btn_disconnect_clicked()
{
    if(g_btConnectName.isEmpty())
    {
        QMessageBox::information(this,"information",tr("No device is connected!"));
    }
    else
    {
        g_bluetoothInterface->bluetoothDisconnect(g_btAddress);
        QMessageBox::information(this,"information",tr("Disconnect successful!"));
        ui->lbl_statusValue->setText(tr("not connect"));
        ui->lbl_connectDeviceValue->setText(tr("no"));
        g_timerConncet->stop();
        g_timerScan->start(10000);
        ui->stackedWidget->setCurrentIndex(0);
        g_btConnectName = "";
        QString name,status;
        ui->BtNameListWidget->clear();
        g_btPairList = g_database.tableShow("bluetooth");
        if(!g_btPairList.isEmpty())                          // Refreshing the list status
        {
            for(int i = 0;i<g_btPairList.size();i++)
            {
                name = g_btPairList.at(i);
                status = tr("saved");
                i++;
                bluetoothListShow(name,status);
            }
        }
        for(int i = 0; i < g_btScanList.size(); i++)
        {
            QString strScanDevice = g_btScanList.at(i);
            QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
            bluetoothListShow(strScanDeviceName,"");
        }
    }
}

void bluetooth::on_btn_remove_clicked()
{
    QString address = g_database.selectTableName("bluetooth",g_btName);
    g_bluetoothInterface->bluetoothRemove(address);
    QMessageBox::information(this,"information",tr("Remove successful!"));
    g_database.deleteTableName("bluetooth",g_btName);
    ui->lbl_statusValue->setText(tr("not connect"));
    ui->lbl_connectDeviceValue->setText(tr("no"));
    ui->BtNameListWidget->clear();
    g_btPairList = g_database.tableShow("bluetooth");
    QString name,status;
    if(!g_btPairList.isEmpty())
    {
        for(int i = 0;i<g_btPairList.size();i++)
        {
            name = g_btPairList.at(i);
            status = tr("saved");
            i++;
            bluetoothListShow(name,status);
        }
    }
    for(int i = 0; i < g_btScanList.size(); i++)
    {
        QString strScanDevice = g_btScanList.at(i);
        QString strScanDeviceName = strScanDevice.trimmed().section("\t",1,1);
        bluetoothListShow(strScanDeviceName,"");
    }

    g_timerConncet->stop();
    g_timerScan->start(10);
    ui->stackedWidget->setCurrentIndex(0);
}

void bluetooth::scan_refresh()
{
    if(g_pairingFlag == 1)
    {
        g_timerScanFlag = 0;
        g_timerScan->stop();
        return;
    }
    if(g_connectingFlag == 1)
    {
        g_timerScanFlag = 0;
        g_timerScan->stop();
        return;
    }
    if(ui->stackedWidget->currentIndex() == 0)
    {
        g_timerScanFlag = 1;
        emit bluetooth_scan_msg();
        g_timerScan->stop();
    }
}

void bluetooth::bluetoothConnectStatus()
{
    QString tmp;
    QString strResult = g_bluetoothInterface->executeLinuxCmd("hciconfig");
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

void bluetooth::setSwitchText()
{
    g_swicthLabel = new QLabel(ui->bluetooth_Switch);
    g_horLayout = new QHBoxLayout();
    g_swicthLabel->setText(tr("  off"));
    g_horLayout->addWidget(g_swicthLabel);
    g_swicthLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    g_swicthLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    ui->bluetooth_Switch->setLayout(g_horLayout);
}
