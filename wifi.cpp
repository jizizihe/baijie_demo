#include "wifi.h"
#include "ui_wifi.h"
#include <QSpinBox>
#include <QToolBox>
#include <QScreen>
#include <QTimer>
#include <QLine>
#include <QSpacerItem>

static int screenWidth;
static int screenHeight;
static int screenFlag;
static int hotspotConnectFalg;
static int hotspotUpFlag;
static int openFlag;
static QScreen *screen;
static QString hotspotName;
static QString hotspotPasswd;
static QString hotspotWlan;
static QTimer *scanTimer;
static QTimer *statusShowTimer;
static QStringList wifiWidgetShowList;
static QLabel *openLabel;
static QHBoxLayout *horLayout;

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wifi)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);
    wifi_bt_t = new wifi_bt_interface(this);
    WifiConnectDialog = new WifiConDialog(this);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    openFlag = 1;
    wifiFont();
    LoadLabel = new QLabel(this);
    if(screenFlag == 1)
    {
        LoadLabel->move(screenHeight/2,screenWidth/2);
    }
    else
    {
        LoadLabel->move(screenWidth/2,screenHeight/2 );
    }

    pMovie = new QMovie("://button_image/loading.webp");
    LoadLabel->setFixedSize(50, 50);
    LoadLabel->setScaledContents(true);
    LoadLabel->setMovie(pMovie);
    ui->lbl_hotspotStatusValue->setText(tr("not connect"));
    myThread = new QThread(this);
    WifiThread = new wifi_thread();
    scanTimer = new QTimer(this);
    statusShowTimer = new QTimer(this);
    ui->wifi_Switch->setCheckedColor(QColor(100, 225, 100, 120));
    switchSetText();
    connect(ui->wifi_Switch,SIGNAL(toggled(bool)),this,SLOT(switch_change_flag(bool)));
    connect(scanTimer,SIGNAL(timeout()),this,SLOT(wifi_scan_refresh()));
    connect(statusShowTimer,SIGNAL(timeout()),this,SLOT(wifi_status_refresh()));
    connect(this,SIGNAL(ToThread()),WifiThread,SLOT(Thread_Fun()));
    connect(this,SIGNAL(wifi_scan_msg()),WifiThread,SLOT(wifi_scan_thread()));
    connect(this,SIGNAL(wifi_activation_msg(QString)),WifiThread,SLOT(wifi_activation_thread(QString)));
    connect(WifiConnectDialog,SIGNAL(wifidial_close_msg()),this,SLOT(wifidailog_hide()));
    connect(WifiThread,SIGNAL(send_wifi_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    WifiThread->moveToThread(myThread);
    myThread->start();
    ui->stackedWidget_3->setCurrentIndex(0);
    connect(ui->WifiListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ListWidgeItem_clicked()));
    connect(this,SIGNAL(hotspot_build_msg(QString,QString,QString)),WifiThread,SLOT(hotspot_build_thread(QString,QString,QString)));
    connect(WifiThread,SIGNAL(wifi_info_fresh_msg(QString)),this,SLOT(wifi_info_fresh(QString)));
    connect(WifiConnectDialog,SIGNAL(wifi_modify_pass_msg(QString,QString)),WifiThread,SLOT(wifi_modyfy_pass(QString,QString)));
    connect(WifiThread,SIGNAL(wifi_modify_pass_msg(bool)),WifiConnectDialog,SLOT(wifi_modify_pass(bool)));
    connect(WifiConnectDialog,SIGNAL(wifi_connect_dialog_msg(QString,QString)),WifiThread,SLOT(wifi_connect_thread(QString,QString)));
    connect(WifiConnectDialog,SIGNAL(wifi_show_refresh_msg()),this,SLOT(wifi_status_show()));
    connect(WifiThread,SIGNAL(wifi_wait_end_msg()),WifiConnectDialog,SLOT(wifi_wait_end_func()));
    connect(this,SIGNAL(scan_wlan_msg()),WifiThread,SLOT(scan_wlan()));
    connect(WifiThread,SIGNAL(revc_scan_wlan_msg(QString)),this,SLOT(revc_scan_wlan(QString)));
    connect(this,SIGNAL(get_hotspot_sql_msg()),this,SLOT(get_hotspot_sql()));
    emit scan_wlan_msg();
    emit wifi_scan_msg(); ui->stackedWidget->setCurrentIndex(0);
    emit get_hotspot_sql_msg();
}

wifi::~wifi()
{
    delete ui;
    delete wifi_bt_t;
    delete WifiConnectDialog;
    delete WifiThread;
    delete myThread;
    delete openLabel;
    delete horLayout;
}

void wifi::recv_msg(int signalType, QString strResult)
{
    int wifiSignal,savedWifiCount=0,listWidgetScrollvalue;
    /*scanWifiListFirst: wifi devices scanned saveWifiList: wifi devices saved in the database scanWifiListSecond: scanWifiListFirst - saveWifiList*/
    QStringList scanWifiListFirst,saveWifiList,scanWifiListSecond;
    QString ScanResult;
    QString tmp,nameStr,signalStr,status;
    pMovie->stop();
    LoadLabel->close();
    switch (signalType)
    {
    case wifiScanSignal:
        ScanResult = strResult;
        scanWifiListFirst = ScanResult.split("\n");
        scanWifiListFirst.removeAll(QString(""));
        listWidgetScrollvalue = ui->WifiListWidget->verticalScrollBar()->value();
        wifiWidgetShowList.clear();
        if(openFlag == 0)
            return ;
        ui->WifiListWidget->clear();
        saveWifiList = databaseWg.tableShow("wifiPasswd");
        for(int i = 0; i < scanWifiListFirst.size(); i++)
        {
            tmp = scanWifiListFirst.at(i);
            nameStr = tmp.split(":").at(0);
            signalStr = tmp.split(":").at(1);
            if(nameStr.isEmpty())
            {
                continue;
            }
            if(!saveWifiList.isEmpty())
            {
                for(int j=0;j<saveWifiList.size();j++)
                {
                    QString savestr = saveWifiList.at(j);
                    if(nameStr == savestr)                     //Gets the devices that are saved in the database and scanned nearby
                    {
                        wifiWidgetShowList << nameStr;
                        wifiWidgetShowList << signalStr;
                    }
                    j++;
                }
            }
        }
        if(!wifiWidgetShowList.isEmpty())
        {
            QStringList saveRemvconnect;
            QString strCmd = QString("iw dev wlan0 link | grep SSID");
            QString wifiName = wifi_bt_t->executeLinuxCmd(strCmd);
            wifiName.remove("\n");wifiName.remove(0,7);
            status = tr("saved");
            saveRemvconnect = wifiWidgetShowList;
            if(!wifiName.isEmpty())                                // show connected device
            {
                for(int i=0;i<saveRemvconnect.size();i++)
                {
                    signalStr = saveRemvconnect.at(i+1);
                    wifiSignal = signalStr.toInt();
                    if(wifiName == saveRemvconnect.at(i))
                    {
                        status = tr("connected"); wifiWidgetShow(saveRemvconnect.at(i),wifiSignal,status);
                        saveRemvconnect.removeAt(i);saveRemvconnect.removeAt(i);break;
                    }
                    i++;
                }
            }
            for(int i=0;i<saveRemvconnect.size();i++)               //show saved devices
            {
                status = tr("saved");
                signalStr = saveRemvconnect.at(i+1);
                wifiSignal = signalStr.toInt();
                wifiWidgetShow(saveRemvconnect.at(i),wifiSignal,status);
                i++;
            }
            status.clear(); scanWifiListSecond = scanWifiListFirst;int num;
            for(int j=0;j<scanWifiListFirst.size();j++)
            {
                QString scanDevice = scanWifiListFirst.at(j);
                nameStr = scanDevice.split(":").at(0);
                for(int i=0;i<wifiWidgetShowList.size();i++)
                {
                    tmp = wifiWidgetShowList.at(i);
                    if(nameStr == tmp)
                    {
                        if(savedWifiCount > 0)
                        {
                            num = j-savedWifiCount;
                        }
                        else
                        {
                            num = j;
                        }
                        savedWifiCount++;
                        scanWifiListSecond.removeAt(num);break;
                    }
                    i++;
                }
            }
            for(int j=0;j<scanWifiListSecond.size();j++)          //show scanned devices
            {
                tmp = scanWifiListSecond.at(j);
                nameStr = tmp.split(":").at(0);
                signalStr = tmp.split(":").at(1);
                wifiSignal = signalStr.toInt();
                if(nameStr.isEmpty())
                {
                    continue;
                }
                wifiWidgetShow(nameStr,wifiSignal,status);
            }
        }
        else
        {
            status.clear();
            for(int j=0;j<scanWifiListFirst.size();j++)
            {
                tmp = scanWifiListFirst.at(j);
                nameStr = tmp.split(":").at(0);
                signalStr = tmp.split(":").at(1);
                wifiSignal = signalStr.toInt();
                if(nameStr.isEmpty())
                {
                    continue;
                }
                wifiWidgetShow(nameStr,wifiSignal,status);
            }
        }
        ui->WifiListWidget->verticalScrollBar()->setValue(listWidgetScrollvalue);
        ui->WifiListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        break;
    case wifiConnectSignal :
        if(strResult == QString(1))
        {
            databaseWg.insertTableTwo("wifiPasswd",WifiConnectDialog->getWifiNameText(),WifiConnectDialog->getPasswdText());
            wifi_info_fresh(WifiConnectDialog->getWifiNameText());
            ui->WifiListWidget->setEnabled(true);
            WifiConnectDialog->setWifiNameText("");
            WifiConnectDialog->setPasswdText("");
            WifiConnectDialog->close();
            scanTimer->stop();
            wifi_status_show();
        }
        else
        {
            QMessageBox mesg(QMessageBox::Critical,
                             tr("QMessageBox::critical()"),
                             tr("Connect failed!"),
                             0,this);
            mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
            mesg.setFocusPolicy(Qt::NoFocus);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(width()*2/9+(width()*7/18-mesg.width()/2),height()/6+(height()*5/12-mesg.height()/2));
            mesg.exec();
        }
        break;
    case wifiActivationSignal:
        if(strResult != QString(1))
        {
            QMessageBox::critical(this,"information",tr("Connect failed!You can try changing your password."));
            QListWidgetItem *item = ui->WifiListWidget->currentItem();
            QWidget* pwig = ui->WifiListWidget->itemWidget(item);
            QList<QLabel*> labelList = pwig->findChildren<QLabel*>();
            QString wifiName = labelList.at(0)->text();
            WifiConnectDialog->show();
            if(screenFlag == 1)
            {
                WifiConnectDialog->resize(screenHeight*2/3,screenWidth*3/5);
                int moveWidth = screenWidth*5/6-(screenWidth*5/6-WifiConnectDialog->height())/2;
                int moveheight = screenHeight*2/9+(screenHeight*7/9-WifiConnectDialog->width())/2;
                WifiConnectDialog->move(moveWidth,moveheight);
            }
            else
            {
                WifiConnectDialog->resize(width()*3/5,height()*3/5);
                WifiConnectDialog->move(width()*2/9+(width()*7/18-WifiConnectDialog->width()/2),height()/6+(height()*5/12-WifiConnectDialog->height()/2));
            }

            WifiConnectDialog->activateWindow();WifiConnectDialog->setFocus();

            WifiConnectDialog->setWifiNameText(wifiName);
            QString strPasswd = databaseWg.selectTableName("wifiPasswd",wifiName);
            WifiConnectDialog->setPasswdText(strPasswd);
            WifiConnectDialog->setWifiOkBtnText(1);          //set the button text is " change "
        }
        break;
    case hotspotBuildSignal:
        if(strResult == QString(1))
        {
            if(hotspotUpFlag == 0)
            {
                QMessageBox::information(this,"information",tr("Connect succeeded!"));
            }
            else
            {
                QMessageBox::information(this,"information",tr("Change succeeded!"));
            }
            ui->lbl_hotspotNameValue->setText(hotspotName);
            ui->lbl_hotspotPasswdValue->setText(hotspotPasswd);
            ui->lbl_wlanValue->setText(hotspotWlan);
            ui->lbl_hotspotStatusValue->setText(tr("connection"));
            QStringList list = databaseWg.tableShow("hostpot");
            if(!list.isEmpty())
            {
                QString name = list.at(0);
                databaseWg.deleteTableName("hostpot",name);
            }
            databaseWg.insertTableThree("hostpot",hotspotName,hotspotPasswd,hotspotWlan);
            if(hotspotWlan == "wlan0")
            {
                hotspotConnectFalg = 1;
            }
            else
            {
                hotspotConnectFalg = 0;
            }
            ui->stackedWidget->setCurrentIndex(2);
            ui->stackedWidget_3->setCurrentIndex(1);
            if(hotspotUpFlag == 0)
            {
                ui->btn_hotspotBuild->setText(tr("change"));hotspotUpFlag = 1;
            }
        }
        else
        {
            QMessageBox::critical(this,"information",tr("Connect failed!"));
        }
        break;
    default:
        break;
    }
}

void wifi::wifi_info_fresh(QString wifiName)
{
    WifiInfo = wifi_bt_t->getWifiStatus(wifiName);
    ui->lbl_wifiPasswordValue->setText(WifiInfo.passwd);
    ui->lbl_wifiNameValue->setText(WifiInfo.name);
    ui->lbl_wifiConnectionValue->setText(WifiInfo.active);
    ui->lbl_wifiSignalValue->setText(WifiInfo.signal);
    ui->lbl_wifiSecurityValue->setText(WifiInfo.secrity);
    ui->lbl_wifiAaddressValue->setText(WifiInfo.ipAddress);
}

void wifi::ListWidgeItem_clicked()
{
    QListWidgetItem *item = ui->WifiListWidget->currentItem();
    QWidget* pwig = ui->WifiListWidget->itemWidget(item);
    QList<QLabel*> labelList = pwig->findChildren<QLabel*>();     //Get the label inside WifiListWidget
    QString wifiName = labelList.at(0)->text();                   //Get the name of the wifi that clicks on WifiListWidget
    int flag = wifi_bt_t->wifiConnectExistFlag(wifiName);
    QStringList list = databaseWg.tableShow("wifiPasswd");
    if((flag == 1)&&(!list.isEmpty()))
    {
        for(int i=0;i<list.size();i++)
        {
            QString name = list.at(i);
            if(!QString::compare(name,wifiName,Qt::CaseSensitive))
            {
                flag = 0;break;
            }
            i++;
        }
        if(flag == 0)
        {
            wifi_info_fresh(wifiName);
            if(QString("no") == WifiInfo.active)
            {
                emit wifi_activation_msg(wifiName);
                LoadLabel->show();
                pMovie->start();
            }
            ui->btn_wifiChangePasswd->show();
            ui->btn_wifiRemove->show();
            ui->stackedWidget->setCurrentIndex(3);
            ui->stackedWidget_2->setCurrentIndex(0);
            ui->WifiListWidget->setEnabled(true);
            statusShowTimer->start(5000);
            scanTimer->stop();
        }
        else
        {
            ui->WifiListWidget->setEnabled(false);
            ui->WifiListWidget->setSelectionMode(QAbstractItemView::NoSelection);
            ui->WifiListWidget->setFocusPolicy(Qt::NoFocus);
            if(screenFlag == 1)
            {
                WifiConnectDialog->resize(screenHeight*2/3,screenWidth*3/5);
                int moveWidth = screenWidth*5/6-(screenWidth*5/6-WifiConnectDialog->height())/2;
                int moveHeight = screenHeight*2/9+(screenHeight*7/9-WifiConnectDialog->width())/2;
                WifiConnectDialog->move(moveWidth,moveHeight);
            }
            else
            {
                WifiConnectDialog->resize(width()*3/5,height()*3/5);
                WifiConnectDialog->move(width()*2/9+(width()*7/18-WifiConnectDialog->width()/2),height()/6+(height()*5/12-WifiConnectDialog->height()/2));
            }
            WifiConnectDialog->show();
            this->setAttribute(Qt::WA_ShowWithoutActivating,true);
            this->setFocusPolicy(Qt::NoFocus);
            WifiConnectDialog->activateWindow();WifiConnectDialog->setFocus();
            WifiConnectDialog->setWifiNameText(wifiName);
            WifiConnectDialog->setWifiOkBtnText(0);
        }
    }
    else
    {
        ui->WifiListWidget->setEnabled(false);
        ui->WifiListWidget->setSelectionMode(QAbstractItemView::NoSelection);
        ui->WifiListWidget->setFocusPolicy(Qt::NoFocus);
        if(screenFlag == 1)
        {
            WifiConnectDialog->resize(screenHeight*2/3,screenWidth*3/5);
            int moveWidth = screenWidth*5/6-(screenWidth*5/6-WifiConnectDialog->height())/2;
            int moveHeight = screenHeight*2/9+(screenHeight*7/9-WifiConnectDialog->width())/2;
            WifiConnectDialog->move(moveWidth,moveHeight);
        }
        else
        {
            WifiConnectDialog->resize(width()*3/5,height()*3/5);
            WifiConnectDialog->move(width()*2/9+(width()*7/18-WifiConnectDialog->width()/2),height()/6+(height()*5/12-WifiConnectDialog->height()/2));
        }
        WifiConnectDialog->show();
        this->setAttribute(Qt::WA_ShowWithoutActivating,true);
        this->setFocusPolicy(Qt::NoFocus);
        WifiConnectDialog->activateWindow();WifiConnectDialog->setFocus();
        WifiConnectDialog->setWifiNameText(wifiName);
        WifiConnectDialog->setWifiOkBtnText(0);
    }
}

void wifi::on_btn_ret_clicked()
{
    WifiConnectDialog->close();
    scanTimer->stop();statusShowTimer->stop();
    emit wifi_back_msg();
}

void wifi::wifi_status_show()
{
    QString strCmd = QString("iw dev wlan0 link | grep SSID");
    QString wifiName = wifi_bt_t->executeLinuxCmd(strCmd);
    QStringList list = databaseWg.tableShow("wifiPasswd");
    wifiName.remove("\n");wifiName.remove(0,7);
    if(wifiName.isEmpty() || (hotspotConnectFalg == 1) || (openFlag == 0)||(list.isEmpty()))
    {
        ui->lbl_wifiNameValue->clear();
        ui->lbl_wifiPasswordValue->clear();
        ui->lbl_wifiConnectionValue->setText(tr("not connect"));
        ui->lbl_wifiSignalValue->clear();
        ui->lbl_wifiSecurityValue->clear();
        ui->lbl_wifiAaddressValue->clear();
    }
    else
    {
        wifi_info_fresh(wifiName.remove(" \n"));
    }
    ui->stackedWidget->setCurrentIndex(3);
    ui->stackedWidget_2->setCurrentIndex(0);
}

void wifi::on_btn_scan_clicked()
{
    if(hotspotConnectFalg == 1)
    {
        QMessageBox::information(this,"information",tr("Please turn off the hotspot first!"));
        return;
    }
    if(openFlag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open the wifi!"));
        return;
    }
    LoadLabel->show();
    pMovie->start();
    scanTimer->start(10000);statusShowTimer->stop();
    ui->stackedWidget->setCurrentIndex(0);
    if(openFlag == 1)
    {
        emit wifi_scan_msg();
    }
}

void wifi::on_btn_hotspot_clicked()
{
    statusShowTimer->stop();
    if(openFlag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open the wifi!"));
        return;
    }
    if((ui->stackedWidget_3->currentIndex() == 0)&&(hotspotUpFlag == 1))
    {
        ui->stackedWidget_3->setCurrentIndex(1);
        return;
    }
    ui->btn_hotspotBuild->setFocus();

    if(!hotspotName.isEmpty())
    {
        ui->HtName->setText(hotspotName);
        ui->HtPasswd->setText(hotspotPasswd);
        ui->comboBox->setCurrentText(hotspotWlan);
    }
    scanTimer->stop();
    ui->stackedWidget->setCurrentIndex(2);
}

void wifi::on_btn_wifiChangePasswd_clicked()
{
    QString wifiName = ui->lbl_wifiNameValue->text();
    if(wifiName.isEmpty())
    {
        return;
    }
    if(screenFlag == 1)
    {
        WifiConnectDialog->resize(screenHeight*2/3,screenWidth*3/5);
        WifiConnectDialog->resize(screenHeight*2/3,screenWidth*3/5);
        int moveWidth = screenWidth*5/6-(screenWidth*5/6-WifiConnectDialog->height())/2;
        int moveHeight = screenHeight*2/9+(screenHeight*7/9-WifiConnectDialog->width())/2;
        WifiConnectDialog->move(moveWidth,moveHeight);
    }
    else
    {
        WifiConnectDialog->resize(width()*3/5,height()*3/5);
        WifiConnectDialog->move(width()*2/9+(width()*7/18-WifiConnectDialog->width()/2),height()/6+(height()*5/12-WifiConnectDialog->height()/2));
    }
    WifiConnectDialog->show();
    WifiConnectDialog->activateWindow();WifiConnectDialog->setFocus();

    WifiConnectDialog->setWifiNameText(wifiName);
    QString strPasswd = databaseWg.selectTableName("wifiPasswd",wifiName);
    WifiConnectDialog->setPasswdText(strPasswd);
    WifiConnectDialog->setWifiOkBtnText(1);
}

void wifi::on_btn_wifiRemove_clicked()
{
    QString wifiName;
    QString strResult;
    wifiName = ui->lbl_wifiNameValue->text();
    if(wifiName.isEmpty())
    {
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,tr("QMessageBox::question()"),tr("Are you sure you want to delete it?"),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(reply == QMessageBox::No)
    {
        return;
    }
    else if(reply == QMessageBox::Yes)
    {
        strResult = wifi_bt_t->wifiConnectionRemove(wifiName);

        bool downResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);
        statusShowTimer->stop();
        if(downResult == 1)
        {
            ui->stackedWidget->setCurrentIndex(0); emit wifi_scan_msg();scanTimer->start(10000);
            databaseWg.deleteTableName(QString("wifiPasswd"),wifiName);
        }
        else
        {
            QMessageBox::critical(this,"information",tr("remove failed!"));
        }
    }
}

void wifi::on_btn_hotspotBuild_clicked()
{
    QString strCmd = QString("nmcli con show --active");
    QString strResult = wifi_bt_t->executeLinuxCmd(strCmd);strResult.remove("");
    QStringList activeNetList = strResult.split("\n");int num = activeNetList.size();

    if(num == 1)                                          //No network connection is currently available
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Connect to hotspot failed!No network is currently available,please connect to the network first"),
                         0,this);
        mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
        mesg.setFocusPolicy(Qt::NoFocus);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        wifi_bt_t->hotspotDisconnect();
        hotspotConnectFalg = 0;
        hotspotName.clear();
        ui->btn_hotspotBuild->setText(tr("up"));hotspotUpFlag = 0;
        ui->stackedWidget_3->setCurrentIndex(0);
        return;
    }
    else
    {
        activeNetList.removeAt(0);
        QString device; QStringList activeNetDeviceList,activeNetTypeList;
        int hasNet=0;                                                    //Determine whether there is a network flag bit
        for(int i=0;i<activeNetList.size();i++)
        {
            device = activeNetList.at(i);
            activeNetDeviceList = device.split(" ");
            activeNetDeviceList.removeAll("");
            if(!activeNetDeviceList.isEmpty())
            {
                device = activeNetDeviceList.at(activeNetDeviceList.size()-1);
                activeNetTypeList << device;
            }
        }
        for(int i=0;i<activeNetTypeList.size();i++)
        {
            if(activeNetTypeList.at(i) == "wlan0")
            {
                continue;
            }
            else
            {
                hasNet = 1;break;
            }
        }

        if(hasNet == 0)
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Connect to hotspot failed!No network is currently available,please connect to the network first"),
                             0,this);
            mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
            mesg.setFocusPolicy(Qt::NoFocus);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
            wifi_bt_t->hotspotDisconnect();
            hotspotConnectFalg = 0;
            hotspotName.clear();
            ui->btn_hotspotBuild->setText(tr("up"));hotspotUpFlag = 0;
            ui->stackedWidget_3->setCurrentIndex(0);
            return;
        }
    }

    int index = ui->stackedWidget_3->currentIndex();
    if(index == 1)
    {
        ui->stackedWidget_3->setCurrentIndex(0);
        ui->HtName->setText(hotspotName);
        ui->HtPasswd->setText(hotspotPasswd);
        ui->comboBox->setCurrentText(hotspotWlan);
        return;
    }
    hotspotName = ui->HtName->text();
    hotspotPasswd = ui->HtPasswd->text();
    hotspotWlan = ui->comboBox->currentText();

    if(hotspotName.isEmpty() || hotspotPasswd.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Input cannot be empty!"),
                         0,this);
        mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
        mesg.setFocusPolicy(Qt::NoFocus);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        return;
    }

    if(hotspotPasswd.length() < 8)
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("The number of password digits must be greater than 8"),
                         0,this);
        mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
        mesg.setFocusPolicy(Qt::NoFocus);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        return ;
    }

    emit hotspot_build_msg(hotspotWlan,hotspotName,hotspotPasswd);
    LoadLabel->show();
    pMovie->start();
}

void wifi::on_btn_hotspotDown_clicked()
{
    ui->lbl_hotspotNameValue->clear();
    ui->lbl_hotspotPasswdValue->clear();
    ui->lbl_hotspotStatusValue->setText(tr("not connect"));
    ui->lbl_wlanValue->clear();
    QString strResult;
    QString strCmd = QString("nmcli con show --active|grep hotspot | awk '{print $1}'");
    strResult = wifi_bt_t->executeLinuxCmd(strCmd);

    if(strResult.isEmpty())
    {
        QMessageBox::information(this,"information",tr("Hotspot not connected!"));
        return ;
    }

    bool retFlag = wifi_bt_t->hotspotDisconnect();
    if(retFlag == true)
    {
        QMessageBox::information(this,"information",tr("successfully deactivated!"));
        hotspotConnectFalg = 0;
        hotspotName.clear();
        ui->btn_hotspotBuild->setText(tr("up"));hotspotUpFlag = 0;
        ui->stackedWidget_3->setCurrentIndex(0);
    }
    else if(retFlag == false)
    {
        QMessageBox::critical(this,"information",tr("deactivated failed!"));
    }
}

void wifi::wifi_scan_refresh()
{
    if(openFlag == 1)
    {
        emit wifi_scan_msg();
    }
}

void wifi::languageReload()
{
    ui->retranslateUi(this);
    emit get_hotspot_sql_msg();
}

void wifi::wifiFont()
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
            font.setPointSize(18);
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
            font.setPointSize(18);
        }
    }
    ui->btn_scan->setFont(font);
    ui->btn_hotspot->setFont(font);
    ui->btn_wifiChangePasswd->setFont(font);
    ui->btn_wifiRemove->setFont(font);
    ui->lbl_wifiName->setFont(font);
    ui->lbl_wifiAaddress->setFont(font);
    ui->lbl_wifiPassword->setFont(font);
    ui->lbl_wifiSignal->setFont(font);
    ui->lbl_wifiSecurity->setFont(font);
    ui->lbl_wifiConnection->setFont(font);
    ui->lbl_hotPasswd->setFont(font);
    ui->btn_hotspotBuild->setFont(font);
    ui->btn_hotspotDown->setFont(font);
    ui->lbl_hotSSID->setFont(font);
    ui->lbl_wifiNameValue->setFont(font);
    ui->lbl_wifiAaddressValue->setFont(font);
    ui->lbl_wifiPasswordValue->setFont(font);
    ui->lbl_wifiSignalValue->setFont(font);
    ui->lbl_wifiSecurityValue->setFont(font);
    ui->lbl_wifiConnectionValue->setFont(font);
    ui->lbl_hotspotPasswd->setFont(font);
    ui->lbl_hotspotName->setFont(font);
    ui->lbl_hotspotStatus->setFont(font);
    ui->lbl_hotspotNameValue->setFont(font);
    ui->lbl_hotspotStatusValue->setFont(font);
    ui->lbl_hotspotPasswdValue->setFont(font);
    ui->lbl_hotwlan->setFont(font);
    ui->comboBox->setFont(font);
    ui->lbl_wlanValue->setFont(font);
    ui->groupBox_2->setFont(font);
    ui->HtName->setFont(font);
    ui->HtPasswd->setFont(font);
    ui->groupBox->setFont(font);
    ui->lbl_wifi->setFont(font);
}

void wifi::closeEvent(QCloseEvent *event)
{
    WifiConnectDialog->show();
    WifiConnectDialog->hide();
    QWidget::closeEvent(event);
}

void wifi::showEvent(QShowEvent *event)
{
    scanTimer->start(10000);
    QWidget::showEvent(event);
}

void wifi::wifidailog_hide()
{
    ui->WifiListWidget->setEnabled(true);
}

void wifi::revc_scan_wlan(QString wlan)
{
    ui->comboBox->clear();
    QStringList list = wlan.split("\n");
    list.removeAll("");
    for(int i = 0;i<list.size();i++)
    {
        QString str = list[i];
        str = str.remove(5,str.length());
        ui->comboBox->addItem(str);
    }
}

void wifi::get_hotspot_sql()
{
    QString strCmd = QString("nmcli connection show --active | grep eth0 |wc -l");
    QString strResult = wifi_bt_t->executeLinuxCmd(strCmd);
    if(strResult == "0\n")
    {
        QStringList list = databaseWg.tableShow("hostpot");
        if(!list.isEmpty())
        {
            hotspotName = list.at(0);
            hotspotPasswd = list.at(1);
            ui->HtName->setText(hotspotName);
            ui->HtPasswd->setText(hotspotPasswd);
        }
        wifi_bt_t->hotspotDisconnect();
        hotspotConnectFalg = 0;
        hotspotName.clear();
        ui->btn_hotspotBuild->setText(tr("up"));hotspotUpFlag = 0;
        return;
    }
    if(wifi_bt_t->hotspotConnectFlag())
    {
        QStringList list = databaseWg.tableShow("hostpot");
        if(!list.isEmpty())
        {
            ui->lbl_hotspotNameValue->setText(list.at(0));
            ui->lbl_hotspotPasswdValue->setText(list.at(1));
            ui->lbl_wlanValue->setText(list.at(2));
            ui->lbl_hotspotStatusValue->setText("connection");
            ui->comboBox->setCurrentText(list.at(2));
            hotspotName = list.at(0);
            hotspotPasswd = list.at(1);
            ui->HtName->setText(hotspotName);
            ui->HtPasswd->setText(hotspotPasswd);

            if( ui->lbl_wlanValue->text() == "wlan0")
            {
                hotspotConnectFalg = 1;
            }
            else
            {
                hotspotConnectFalg = 0;
            }
            ui->stackedWidget->setCurrentIndex(2);
            ui->stackedWidget_3->setCurrentIndex(1);
            ui->btn_hotspotBuild->setText(tr("change"));hotspotUpFlag = 1;
        }
    }
    else
    {
        QStringList list = databaseWg.tableShow("hostpot");
        if(!list.isEmpty())
        {
            hotspotName = list.at(0);
            hotspotPasswd = list.at(1);
            ui->HtName->setText(hotspotName);
            ui->HtPasswd->setText(hotspotPasswd);
        }
    }
}

void wifi::wifiWidgetShow(QString nameStr,int signal,QString status_str)
{
    QWidget *widget=new QWidget(this);
    QHBoxLayout *horLayout = new QHBoxLayout();
    QVBoxLayout *v1 = new QVBoxLayout();
    QVBoxLayout *v2 = new QVBoxLayout();
    QVBoxLayout *v3 = new QVBoxLayout();
    QVBoxLayout *v4 = new QVBoxLayout();
    QLabel *nameLabel = new QLabel(widget);
    QLabel *line1 = new QLabel(widget);                   //line serves as the icon of the wifi signal bar
    QLabel *line2 = new QLabel(widget);
    QLabel *line3 = new QLabel(widget);
    QLabel *line4 = new QLabel(widget);
    QLabel *status = new QLabel(widget);
    QListWidgetItem *item = new QListWidgetItem();
    QSpacerItem *sparcer1 = new QSpacerItem(0,2,QSizePolicy::Fixed,QSizePolicy::Expanding);
    QSpacerItem *sparcer2 = new QSpacerItem(0,2,QSizePolicy::Fixed,QSizePolicy::Expanding);
    QSpacerItem *sparcer3 = new QSpacerItem(0,2,QSizePolicy::Fixed,QSizePolicy::Expanding);
    QSpacerItem *sparcer4 = new QSpacerItem(0,2,QSizePolicy::Fixed,QSizePolicy::Expanding);
    nameLabel->setText(nameStr);
    status->setText(status_str);
    status->setAlignment(Qt::AlignCenter);
    horLayout->setContentsMargins(30,0,0,0);
    line1->setStyleSheet("background-color: rgb(15, 14, 14);");
    v1->addItem(sparcer1);
    v1->addWidget(line1);

    if(signal < 40)
    {
        line2->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    else
    {
        line2->setStyleSheet("background-color: rgb(15, 14, 14);");
    }
    v2->addItem(sparcer2);
    v2->addWidget(line2);

    if(signal < 60)
    {
        line3->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    else
    {
        line3->setStyleSheet("background-color: rgb(15, 14, 14);");
    }
    v3->addItem(sparcer3);
    v3->addWidget(line3);

    if(signal < 80)
    {
        line4->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    else
    {
        line4->setStyleSheet("background-color: rgb(15, 14, 14);");
    }
    v4->addItem(sparcer4);
    v4->addWidget(line4);

    horLayout->addWidget(nameLabel);
    horLayout->addItem(v1);
    horLayout->addItem(v2);
    horLayout->addItem(v3);
    horLayout->addItem(v4);
    horLayout->addWidget(status);
    horLayout->setStretchFactor(nameLabel,2);
    horLayout->setStretchFactor(status,1);
    widget->setLayout(horLayout);
    horLayout->setSpacing(2);
    line4->setFixedHeight(widget->height()+8);
    line3->setFixedHeight(widget->height()+1);
    line2->setFixedHeight(widget->height()*8/10);
    line1->setFixedHeight(widget->height()*5/10);
    line1->setFixedWidth(widget->width()/6);
    line2->setFixedWidth(widget->width()/6);
    line3->setFixedWidth(widget->width()/6);
    line4->setFixedWidth(widget->width()/6);
    ui->WifiListWidget->addItem(item);
    ui->WifiListWidget->setItemWidget(item, widget);
}

void wifi::wifi_status_refresh()
{
    if((ui->stackedWidget->currentIndex() == 3)&&(ui->stackedWidget_2->currentIndex() == 0))
    {
        wifi_status_show();
    }
}

void wifi::switch_change_flag(bool flag)
{
    flag = ui->wifi_Switch->isToggled();
    if(flag == 1)
    {
        openFlag = 1;
        scanTimer->start(10000);
        for(int i = 0; i < ui->WifiListWidget->count(); i++)
        {
            ui->WifiListWidget->setItemHidden(ui->WifiListWidget->item(i), false);
        }
        wifi_bt_t->wifiEnableFlag(true);
        wifi_status_show();
        ui->stackedWidget->setCurrentIndex(0);
        ui->btn_wifiRemove->setEnabled(true);
        ui->btn_wifiChangePasswd->setEnabled(true);
        openLabel->setText(tr("  off"));
        openLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        openLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    }
    else
    {
        openFlag = 0;
        for(int i = 0; i < ui->WifiListWidget->count(); i++)
        {
            ui->WifiListWidget->setItemHidden(ui->WifiListWidget->item(i), true);
        }
        wifi_bt_t->wifiEnableFlag(false);
        wifi_bt_t->wifiDisconnect();
        pMovie->stop();
        scanTimer->stop();statusShowTimer->stop();
        LoadLabel->close();
        wifi_status_show();
        ui->btn_wifiRemove->setEnabled(false);
        ui->btn_wifiChangePasswd->setEnabled(false);
        openLabel->setText(tr("on   "));
        openLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        openLabel->setStyleSheet("color: rgba(255, 255, 255,200);");
    }
}

void wifi::switchSetText()
{
    openLabel = new QLabel(ui->wifi_Switch);
    horLayout = new QHBoxLayout();
    openLabel->setText(tr("  off"));
    horLayout->addWidget(openLabel);
    openLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    openLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    ui->wifi_Switch->setLayout(horLayout);
}
