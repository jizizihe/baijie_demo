#include "wifi.h"
#include "ui_wifi.h"
#include <QSpinBox>
#include <QToolBox>
#include <QScreen>
#include <QTimer>
#include <QLine>
#include <QSpacerItem>

static int g_screenWidth;
static int g_screenHeight;
static int g_screenFlag;
static int g_hotspotConnectFalg;
static int g_hotspotUpFlag;
static int g_openFlag;
static int g_showFlag;
static QScreen *g_screen;
static QString g_hotspotName;
static QString g_hotspotPasswd;
static QString g_hotspotWlan;
static QTimer *g_scanTimer;
static QTimer *g_statusShowTimer;
static QStringList g_wifiWidgetShowList;
static QLabel *g_openLabel;
static QHBoxLayout *g_horLayout;

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wifi)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    ui->lbl_hotspotStatusValue->setText(tr("not connect"));
    ui->stackedWidget_3->setCurrentIndex(0);
    ui->wifi_Switch->setCheckedColor(QColor(100, 225, 100, 120));
    ui->stackedWidget->setCurrentIndex(0);

    g_scanTimer = new QTimer(this);
    g_statusShowTimer = new QTimer(this);
    g_wifiInterface = new wifi_interface(this);
    g_wifiConnectDialogWg = new WifiConDialog(this);
    g_myThread = new QThread(this);
    g_wifiThread = new wifi_thread();
    g_loadLabel = new QLabel(this);
    g_pMovie = new QMovie("://button_image/loading.webp");

    g_openFlag = 1;
    g_loadLabel->setFixedSize(50, 50);
    g_loadLabel->setScaledContents(true);
    g_loadLabel->setMovie(g_pMovie);
    g_wifiThread->moveToThread(g_myThread);
    g_myThread->start();
    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();

    if(g_screenWidth < g_screenHeight)
    {
        g_screenFlag = 1;
        ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
        g_loadLabel->move(g_screenHeight/2,g_screenWidth/2);
    }
    else
    {
        g_loadLabel->move(g_screenWidth/2,g_screenHeight/2 );
    }

    setWifiFont();
    setSwitchText();

    connect(ui->wifi_Switch,SIGNAL(toggled(bool)),this,SLOT(switch_change_flag(bool)));
    connect(ui->WifiListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ListWidgeItem_clicked()));
    connect(g_scanTimer,SIGNAL(timeout()),this,SLOT(wifi_scan_refresh()));
    connect(g_statusShowTimer,SIGNAL(timeout()),this,SLOT(wifi_status_refresh()));
    connect(this,SIGNAL(wifi_scan_msg()),g_wifiThread,SLOT(wifi_scan_thread()));
    connect(this,SIGNAL(set_wifi_enble(QString)),g_wifiThread,SLOT(wifi_activation_thread(QString)));
    connect(this,SIGNAL(hotspot_build_msg(QString,QString,QString)),g_wifiThread,SLOT(hotspot_build_thread(QString,QString,QString)));
    connect(this,SIGNAL(scan_wlan_msg()),g_wifiThread,SLOT(scan_wlan()));
    connect(this,SIGNAL(get_hotspot_sql_msg()),this,SLOT(get_hotspot_sql()));
    connect(g_wifiConnectDialogWg,SIGNAL(wifi_modify_pass_msg(QString,QString)),g_wifiThread,SLOT(wifi_modyfy_pass(QString,QString)));
    connect(g_wifiConnectDialogWg,SIGNAL(wifi_connect_dialog_msg(QString,QString)),g_wifiThread,SLOT(wifi_connect_thread(QString,QString)));
    connect(g_wifiConnectDialogWg,SIGNAL(wifi_status_show_msg()),this,SLOT(wifi_status_show()));
    connect(g_wifiConnectDialogWg,SIGNAL(wifi_dialog_close_msg()),this,SLOT(wifidailog_hide()));
    connect(g_wifiThread,SIGNAL(wifi_info_refresh_msg(QString)),this,SLOT(wifi_info_refresh(QString)));
    connect(g_wifiThread,SIGNAL(send_wifi_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    connect(g_wifiThread,SIGNAL(wifi_wait_end_msg()),g_wifiConnectDialogWg,SLOT(wifi_wait_end_func()));
    connect(g_wifiThread,SIGNAL(wifi_modify_pass_msg(bool)),g_wifiConnectDialogWg,SLOT(wifi_modify_pass(bool)));
    connect(g_wifiThread,SIGNAL(revc_scan_wlan_msg(QString)),this,SLOT(revc_scan_wlan(QString)));
    emit scan_wlan_msg();
    emit get_hotspot_sql_msg();
}

wifi::~wifi()
{
    delete ui;
    delete g_wifiInterface;
    delete g_wifiConnectDialogWg;
    delete g_wifiThread;
    delete g_myThread;
    delete g_openLabel;
    delete g_horLayout;
}

void wifi::recv_msg(int signalType, QString strResult)
{
    int wifiSignal,savedWifiCount=0,listWidgetScrollvalue;
    /*scanWifiListFirst: wifi devices scanned saveWifiList: wifi devices saved in the database scanWifiListSecond: scanWifiListFirst - saveWifiList*/
    QStringList scanWifiListFirst,saveWifiList,scanWifiListSecond;
    QString ScanResult;
    QString tmp,nameStr,signalStr,status;
    g_pMovie->stop();
    g_loadLabel->close();
    switch (signalType)
    {
    case EnumWifiScanSignal:
        ScanResult = strResult;
        scanWifiListFirst = ScanResult.split("\n");
        scanWifiListFirst.removeAll(QString(""));
        listWidgetScrollvalue = ui->WifiListWidget->verticalScrollBar()->value();
        g_wifiWidgetShowList.clear();
        if(g_openFlag == 0)
            return ;
        ui->WifiListWidget->clear();
        saveWifiList = g_database.tableShow("wifiPasswd");
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
                        g_wifiWidgetShowList << nameStr;
                        g_wifiWidgetShowList << signalStr;
                    }
                    j++;
                }
            }
        }
        if(!g_wifiWidgetShowList.isEmpty())
        {
            QStringList saveRemvconnect;
            QString strCmd = QString("iw dev wlan0 link | grep SSID");
            QString wifiName = g_wifiInterface->executeLinuxCmd(strCmd);
            wifiName.remove("\n");wifiName.remove(0,7);
            status = tr("saved");
            saveRemvconnect = g_wifiWidgetShowList;
            if(!wifiName.isEmpty())                                // show connected device
            {
                for(int i=0;i<saveRemvconnect.size();i++)
                {
                    signalStr = saveRemvconnect.at(i+1);
                    wifiSignal = signalStr.toInt();
                    if(wifiName == saveRemvconnect.at(i))
                    {
                        status = tr("connected");
                        wifiListShow(saveRemvconnect.at(i),wifiSignal,status);
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
                wifiListShow(saveRemvconnect.at(i),wifiSignal,status);
                i++;
            }
            status.clear(); scanWifiListSecond = scanWifiListFirst;int num;
            for(int j=0;j<scanWifiListFirst.size();j++)
            {
                QString scanDevice = scanWifiListFirst.at(j);
                nameStr = scanDevice.split(":").at(0);
                for(int i=0;i<g_wifiWidgetShowList.size();i++)
                {
                    tmp = g_wifiWidgetShowList.at(i);
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
                wifiListShow(nameStr,wifiSignal,status);
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
                wifiListShow(nameStr,wifiSignal,status);
            }
        }
        ui->WifiListWidget->verticalScrollBar()->setValue(listWidgetScrollvalue);
        ui->WifiListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        break;
    case EnumWifiConnectSignal :
        if(strResult == QString(1))
        {
            g_database.insertTableTwo("wifiPasswd",g_wifiConnectDialogWg->getWifiNameText(),g_wifiConnectDialogWg->getPasswdText());
            wifi_info_refresh(g_wifiConnectDialogWg->getWifiNameText());
            ui->WifiListWidget->setEnabled(true);
            g_wifiConnectDialogWg->setWifiNameText("");
            g_wifiConnectDialogWg->setPasswdText("");
            g_wifiConnectDialogWg->close();
            g_scanTimer->stop();
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
            if(g_screenFlag == 1)
                mesg.move(g_screenWidth*2/3,g_screenHeight/3);
            else
                mesg.move(width()*2/9+(width()*7/18-mesg.width()/2),height()/6+(height()*5/12-mesg.height()/2));
            mesg.exec();
        }
        break;
    case EnumWifiActivationSignal:
        if(strResult != QString(1))
        {
            QMessageBox::critical(this,"information",tr("Connect failed!You can try changing your password."));
            QListWidgetItem *item = ui->WifiListWidget->currentItem();
            QWidget* pwig = ui->WifiListWidget->itemWidget(item);
            QList<QLabel*> labelList = pwig->findChildren<QLabel*>();
            QString wifiName = labelList.at(0)->text();
            g_wifiConnectDialogWg->show();
            if(g_screenFlag == 1)
            {
                g_wifiConnectDialogWg->resize(g_screenHeight*2/3,g_screenWidth*3/5);
                int moveWidth = g_screenWidth*5/6-(g_screenWidth*5/6-g_wifiConnectDialogWg->height())/2;
                int moveheight = g_screenHeight*2/9+(g_screenHeight*7/9-g_wifiConnectDialogWg->width())/2;
                g_wifiConnectDialogWg->move(moveWidth,moveheight);
            }
            else
            {
                g_wifiConnectDialogWg->resize(width()*3/5,height()*3/5);
                g_wifiConnectDialogWg->move(width()*2/9+(width()*7/18-g_wifiConnectDialogWg->width()/2),height()/6+(height()*5/12-g_wifiConnectDialogWg->height()/2));
            }

            g_wifiConnectDialogWg->activateWindow();g_wifiConnectDialogWg->setFocus();

            g_wifiConnectDialogWg->setWifiNameText(wifiName);
            QString strPasswd = g_database.selectTableName("wifiPasswd",wifiName);
            g_wifiConnectDialogWg->setPasswdText(strPasswd);
            g_wifiConnectDialogWg->setWifiOkBtnText(1);          //set the button text is " change "
        }
        break;
    case EnumHotspotBuildSignal:
        if(strResult == QString(1))
        {
            if(g_hotspotUpFlag == 0)
            {
                QMessageBox::information(this,"information",tr("Connect succeeded!"));
            }
            else
            {
                QMessageBox::information(this,"information",tr("Change succeeded!"));
            }
            ui->lbl_hotspotNameValue->setText(g_hotspotName);
            ui->lbl_hotspotPasswdValue->setText(g_hotspotPasswd);
            ui->lbl_wlanValue->setText(g_hotspotWlan);
            ui->lbl_hotspotStatusValue->setText(tr("connection"));
            QStringList list = g_database.tableShow("hostpot");
            if(!list.isEmpty())
            {
                QString name = list.at(0);
                g_database.deleteTableName("hostpot",name);
            }
            g_database.insertTableThree("hostpot",g_hotspotName,g_hotspotPasswd,g_hotspotWlan);
            if(g_hotspotWlan == "wlan0")
            {
                g_hotspotConnectFalg = 1;
            }
            else
            {
                g_hotspotConnectFalg = 0;
            }
            ui->stackedWidget->setCurrentIndex(2);
            ui->stackedWidget_3->setCurrentIndex(1);
            if(g_hotspotUpFlag == 0)
            {
                ui->btn_hotspotBuild->setText(tr("change"));g_hotspotUpFlag = 1;
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

void wifi::wifi_info_refresh(QString wifiName)
{
    g_wifiInfo = g_wifiInterface->getWifiStatus(wifiName);
    ui->lbl_wifiPasswordValue->setText(g_wifiInfo.passwd);
    ui->lbl_wifiNameValue->setText(g_wifiInfo.name);
    ui->lbl_wifiConnectionValue->setText(g_wifiInfo.active);
    ui->lbl_wifiSignalValue->setText(g_wifiInfo.signal);
    ui->lbl_wifiSecurityValue->setText(g_wifiInfo.secrity);
    ui->lbl_wifiAaddressValue->setText(g_wifiInfo.ipAddress);
}

void wifi::ListWidgeItem_clicked()
{
    QListWidgetItem *item = ui->WifiListWidget->currentItem();
    QWidget* pwig = ui->WifiListWidget->itemWidget(item);
    QList<QLabel*> labelList = pwig->findChildren<QLabel*>();     //Get the label inside WifiListWidget
    QString wifiName = labelList.at(0)->text();                   //Get the name of the wifi that clicks on WifiListWidget
    int flag = g_wifiInterface->wifiConnectExistFlag(wifiName);
    QStringList list = g_database.tableShow("wifiPasswd");
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
            wifi_info_refresh(wifiName);
            if(QString("no") == g_wifiInfo.active)
            {
                emit set_wifi_enble(wifiName);
                g_loadLabel->show();
                g_pMovie->start();
            }
            ui->btn_wifiChangePasswd->show();
            ui->btn_wifiRemove->show();
            ui->stackedWidget->setCurrentIndex(3);
            ui->stackedWidget_2->setCurrentIndex(0);
            ui->WifiListWidget->setEnabled(true);
            g_statusShowTimer->start(5000);
            g_scanTimer->stop();
        }
        else
        {
            ui->WifiListWidget->setEnabled(false);
            ui->WifiListWidget->setSelectionMode(QAbstractItemView::NoSelection);
            ui->WifiListWidget->setFocusPolicy(Qt::NoFocus);
            if(g_screenFlag == 1)
            {
                g_wifiConnectDialogWg->resize(g_screenHeight*2/3,g_screenWidth*3/5);
                int moveWidth = g_screenWidth*5/6-(g_screenWidth*5/6-g_wifiConnectDialogWg->height())/2;
                int moveHeight = g_screenHeight*2/9+(g_screenHeight*7/9-g_wifiConnectDialogWg->width())/2;
                g_wifiConnectDialogWg->move(moveWidth,moveHeight);
            }
            else
            {
                g_wifiConnectDialogWg->resize(width()*3/5,height()*3/5);
                g_wifiConnectDialogWg->move(width()*2/9+(width()*7/18-g_wifiConnectDialogWg->width()/2),height()/6+(height()*5/12-g_wifiConnectDialogWg->height()/2));
            }
            g_wifiConnectDialogWg->show();
            this->setAttribute(Qt::WA_ShowWithoutActivating,true);
            this->setFocusPolicy(Qt::NoFocus);
            g_wifiConnectDialogWg->activateWindow();g_wifiConnectDialogWg->setFocus();
            g_wifiConnectDialogWg->setWifiNameText(wifiName);
            g_wifiConnectDialogWg->setWifiOkBtnText(0);
        }
    }
    else
    {
        ui->WifiListWidget->setEnabled(false);
        ui->WifiListWidget->setSelectionMode(QAbstractItemView::NoSelection);
        ui->WifiListWidget->setFocusPolicy(Qt::NoFocus);
        if(g_screenFlag == 1)
        {
            g_wifiConnectDialogWg->resize(g_screenHeight*2/3,g_screenWidth*3/5);
            int moveWidth = g_screenWidth*5/6-(g_screenWidth*5/6-g_wifiConnectDialogWg->height())/2;
            int moveHeight = g_screenHeight*2/9+(g_screenHeight*7/9-g_wifiConnectDialogWg->width())/2;
            g_wifiConnectDialogWg->move(moveWidth,moveHeight);
        }
        else
        {
            g_wifiConnectDialogWg->resize(width()*3/5,height()*3/5);
            g_wifiConnectDialogWg->move(width()*2/9+(width()*7/18-g_wifiConnectDialogWg->width()/2),height()/6+(height()*5/12-g_wifiConnectDialogWg->height()/2));
        }
        g_wifiConnectDialogWg->show();
        this->setAttribute(Qt::WA_ShowWithoutActivating,true);
        this->setFocusPolicy(Qt::NoFocus);
        g_wifiConnectDialogWg->activateWindow();g_wifiConnectDialogWg->setFocus();
        g_wifiConnectDialogWg->setWifiNameText(wifiName);
        g_wifiConnectDialogWg->setWifiOkBtnText(0);
    }
}

void wifi::on_btn_ret_clicked()
{
    g_wifiConnectDialogWg->close();
    g_scanTimer->stop();g_statusShowTimer->stop();
    emit wifi_back_msg();
}

void wifi::wifi_status_show()
{
    QString strCmd = QString("iw dev wlan0 link | grep SSID");
    QString wifiName = g_wifiInterface->executeLinuxCmd(strCmd);
    QStringList list = g_database.tableShow("wifiPasswd");
    wifiName.remove("\n");wifiName.remove(0,7);
    if(wifiName.isEmpty() || (g_hotspotConnectFalg == 1) || (g_openFlag == 0)||(list.isEmpty()))
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
        wifi_info_refresh(wifiName.remove(" \n"));
    }
    ui->stackedWidget->setCurrentIndex(3);
    ui->stackedWidget_2->setCurrentIndex(0);
}

void wifi::on_btn_scan_clicked()
{
    if(g_hotspotConnectFalg == 1)
    {
        QMessageBox::information(this,"information",tr("Please turn off the hotspot first!"));
        return;
    }
    if(g_openFlag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open the wifi!"));
        return;
    }
    g_loadLabel->show();
    g_pMovie->start();
    g_scanTimer->start(10000);g_statusShowTimer->stop();
    ui->stackedWidget->setCurrentIndex(0);
    if(g_openFlag == 1)
    {
        emit wifi_scan_msg();
    }
}

void wifi::on_btn_hotspot_clicked()
{
    g_statusShowTimer->stop();
    if(g_openFlag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open the wifi!"));
        return;
    }
    if((ui->stackedWidget_3->currentIndex() == 0)&&(g_hotspotUpFlag == 1))
    {
        ui->stackedWidget_3->setCurrentIndex(1);
        return;
    }
    ui->btn_hotspotBuild->setFocus();

    if(!g_hotspotName.isEmpty())
    {
        ui->HtName->setText(g_hotspotName);
        ui->HtPasswd->setText(g_hotspotPasswd);
        ui->comboBox->setCurrentText(g_hotspotWlan);
    }
    g_scanTimer->stop();
    ui->stackedWidget->setCurrentIndex(2);
}

void wifi::on_btn_wifiChangePasswd_clicked()
{
    QString wifiName = ui->lbl_wifiNameValue->text();
    if(wifiName.isEmpty())
    {
        return;
    }
    if(g_screenFlag == 1)
    {
        g_wifiConnectDialogWg->resize(g_screenHeight*2/3,g_screenWidth*3/5);
        g_wifiConnectDialogWg->resize(g_screenHeight*2/3,g_screenWidth*3/5);
        int moveWidth = g_screenWidth*5/6-(g_screenWidth*5/6-g_wifiConnectDialogWg->height())/2;
        int moveHeight = g_screenHeight*2/9+(g_screenHeight*7/9-g_wifiConnectDialogWg->width())/2;
        g_wifiConnectDialogWg->move(moveWidth,moveHeight);
    }
    else
    {
        g_wifiConnectDialogWg->resize(width()*3/5,height()*3/5);
        g_wifiConnectDialogWg->move(width()*2/9+(width()*7/18-g_wifiConnectDialogWg->width()/2),height()/6+(height()*5/12-g_wifiConnectDialogWg->height()/2));
    }
    g_wifiConnectDialogWg->show();
    g_wifiConnectDialogWg->activateWindow();g_wifiConnectDialogWg->setFocus();

    g_wifiConnectDialogWg->setWifiNameText(wifiName);
    QString strPasswd = g_database.selectTableName("wifiPasswd",wifiName);
    g_wifiConnectDialogWg->setPasswdText(strPasswd);
    g_wifiConnectDialogWg->setWifiOkBtnText(1);
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
        strResult = g_wifiInterface->wifiRemove(wifiName);

        bool downResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);
        g_statusShowTimer->stop();
        if(downResult == 1)
        {
            ui->stackedWidget->setCurrentIndex(0); emit wifi_scan_msg();g_scanTimer->start(10000);
            g_database.deleteTableName(QString("wifiPasswd"),wifiName);
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
    QString strResult = g_wifiInterface->executeLinuxCmd(strCmd);strResult.remove("");
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
        if(g_screenFlag == 1)
            mesg.move(g_screenWidth*2/3,g_screenHeight/3);
        else
            mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        g_wifiInterface->hotspotDisconnect();
        g_hotspotConnectFalg = 0;
        g_hotspotName.clear();
        ui->btn_hotspotBuild->setText(tr("up"));g_hotspotUpFlag = 0;
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
            if(g_screenFlag == 1)
                mesg.move(g_screenWidth*2/3,g_screenHeight/3);
            else
                mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
            g_wifiInterface->hotspotDisconnect();
            g_hotspotConnectFalg = 0;
            g_hotspotName.clear();
            ui->btn_hotspotBuild->setText(tr("up"));g_hotspotUpFlag = 0;
            ui->stackedWidget_3->setCurrentIndex(0);
            return;
        }
    }

    int index = ui->stackedWidget_3->currentIndex();
    if(index == 1)
    {
        ui->stackedWidget_3->setCurrentIndex(0);
        ui->HtName->setText(g_hotspotName);
        ui->HtPasswd->setText(g_hotspotPasswd);
        ui->comboBox->setCurrentText(g_hotspotWlan);
        return;
    }
    g_hotspotName = ui->HtName->text();
    g_hotspotPasswd = ui->HtPasswd->text();
    g_hotspotWlan = ui->comboBox->currentText();

    if(g_hotspotName.isEmpty() || g_hotspotPasswd.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Input cannot be empty!"),
                         0,this);
        mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
        mesg.setFocusPolicy(Qt::NoFocus);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(g_screenFlag == 1)
            mesg.move(g_screenWidth*2/3,g_screenHeight/3);
        else
            mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        return;
    }

    if(g_hotspotPasswd.length() < 8)
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("The number of password digits must be greater than 8"),
                         0,this);
        mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
        mesg.setFocusPolicy(Qt::NoFocus);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(g_screenFlag == 1)
            mesg.move(g_screenWidth*2/3,g_screenHeight/3);
        else
            mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        return ;
    }

    emit hotspot_build_msg(g_hotspotWlan,g_hotspotName,g_hotspotPasswd);
    g_loadLabel->show();
    g_pMovie->start();
}

void wifi::on_btn_hotspotDown_clicked()
{
    ui->lbl_hotspotNameValue->clear();
    ui->lbl_hotspotPasswdValue->clear();
    ui->lbl_hotspotStatusValue->setText(tr("not connect"));
    ui->lbl_wlanValue->clear();
    QString strResult;
    QString strCmd = QString("nmcli con show --active|grep hotspot | awk '{print $1}'");
    strResult = g_wifiInterface->executeLinuxCmd(strCmd);

    if(strResult.isEmpty())
    {
        QMessageBox::information(this,"information",tr("Hotspot not connected!"));
        return ;
    }

    bool retFlag = g_wifiInterface->hotspotDisconnect();
    if(retFlag == true)
    {
        QMessageBox::information(this,"information",tr("successfully deactivated!"));
        g_hotspotConnectFalg = 0;
        g_hotspotName.clear();
        ui->btn_hotspotBuild->setText(tr("up"));g_hotspotUpFlag = 0;
        ui->stackedWidget_3->setCurrentIndex(0);
    }
    else if(retFlag == false)
    {
        QMessageBox::critical(this,"information",tr("deactivated failed!"));
    }
}

void wifi::wifi_scan_refresh()
{
    if(g_openFlag == 1)
    {
        emit wifi_scan_msg();
    }
}

void wifi::languageReload()
{
    ui->retranslateUi(this);
    g_wifiConnectDialogWg->languageReload();
    emit get_hotspot_sql_msg();
    int flag = ui->wifi_Switch->isToggled();
    if(flag == 1)
    ui->wifi_Switch->setToggle(true);
    else
    ui->wifi_Switch->setToggle(false);
}

void wifi::setWifiFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realY = g_screen->physicalDotsPerInchY();
    qreal realWidth = g_screenWidth / realX * 2.54;
    qreal realHeight = g_screenHeight / realY *2.54;
    QFont font;
    if(g_screenFlag)
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
    g_wifiConnectDialogWg->show();
    g_wifiConnectDialogWg->hide();
    g_showFlag = 0;
    QWidget::closeEvent(event);
}

void wifi::showEvent(QShowEvent *event)
{
    g_scanTimer->start(10000);
    g_showFlag = 1;
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
    QString strResult = g_wifiInterface->executeLinuxCmd(strCmd);
    if(strResult == "0\n")
    {
        QStringList list = g_database.tableShow("hostpot");
        if(!list.isEmpty())
        {
            g_hotspotName = list.at(0);
            g_hotspotPasswd = list.at(1);
            ui->HtName->setText(g_hotspotName);
            ui->HtPasswd->setText(g_hotspotPasswd);
        }
        g_wifiInterface->hotspotDisconnect();
        g_hotspotConnectFalg = 0;
        g_hotspotName.clear();
        ui->btn_hotspotBuild->setText(tr("up"));g_hotspotUpFlag = 0;
        return;
    }
    if(g_wifiInterface->hotspotConnectFlag())
    {
        QStringList list = g_database.tableShow("hostpot");
        if(!list.isEmpty())
        {
            ui->lbl_hotspotNameValue->setText(list.at(0));
            ui->lbl_hotspotPasswdValue->setText(list.at(1));
            ui->lbl_wlanValue->setText(list.at(2));
            ui->lbl_hotspotStatusValue->setText("connection");
            ui->comboBox->setCurrentText(list.at(2));
            g_hotspotName = list.at(0);
            g_hotspotPasswd = list.at(1);
            ui->HtName->setText(g_hotspotName);
            ui->HtPasswd->setText(g_hotspotPasswd);

            if( ui->lbl_wlanValue->text() == "wlan0")
            {
                g_hotspotConnectFalg = 1;
            }
            else
            {
                g_hotspotConnectFalg = 0;
            }
            ui->stackedWidget->setCurrentIndex(2);
            ui->stackedWidget_3->setCurrentIndex(1);
            ui->btn_hotspotBuild->setText(tr("change"));g_hotspotUpFlag = 1;
        }
    }
    else
    {
        QStringList list = g_database.tableShow("hostpot");
        if(!list.isEmpty())
        {
            g_hotspotName = list.at(0);
            g_hotspotPasswd = list.at(1);
            ui->HtName->setText(g_hotspotName);
            ui->HtPasswd->setText(g_hotspotPasswd);
        }
    }
}

void wifi::wifiListShow(QString nameStr,int signal,QString status_str)
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
        g_openFlag = 1;
        if(g_showFlag == 1)
        {
            g_scanTimer->start(10000);
        }
        for(int i = 0; i < ui->WifiListWidget->count(); i++)
        {
            ui->WifiListWidget->setItemHidden(ui->WifiListWidget->item(i), false);
        }
        g_wifiInterface->wifiEnableFlag(true);
        wifi_status_show();
        ui->stackedWidget->setCurrentIndex(0);
        ui->btn_wifiRemove->setEnabled(true);
        ui->btn_wifiChangePasswd->setEnabled(true);
        g_openLabel->setText(tr("  off"));
        g_openLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        g_openLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    }
    else
    {
        g_openFlag = 0;
        for(int i = 0; i < ui->WifiListWidget->count(); i++)
        {
            ui->WifiListWidget->setItemHidden(ui->WifiListWidget->item(i), true);
        }
        g_wifiInterface->wifiEnableFlag(false);
        g_wifiInterface->wifiDisconnect();
        g_pMovie->stop();
        g_scanTimer->stop();g_statusShowTimer->stop();
        g_loadLabel->close();
        wifi_status_show();
        ui->btn_wifiRemove->setEnabled(false);
        ui->btn_wifiChangePasswd->setEnabled(false);
        g_openLabel->setText(tr("on   "));
        g_openLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        g_openLabel->setStyleSheet("color: rgba(255, 255, 255,200);");
    }
}

void wifi::setSwitchText()
{
    g_openLabel = new QLabel(ui->wifi_Switch);
    g_horLayout = new QHBoxLayout();
    g_openLabel->setText(tr("  off"));
    g_horLayout->addWidget(g_openLabel);
    g_openLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    g_openLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    ui->wifi_Switch->setLayout(g_horLayout);
}
