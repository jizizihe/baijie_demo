 #include "wifi.h"
#include "ui_wifi.h"
#include <QSpinBox>
#include <QToolBox>
#include <QScreen>
#include <QTimer>
#include <QLine>
#include<QSpacerItem>

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;
static QString HtName;
static QString HtPasswd;
static QTimer *timer;
static int hotconnect_falg;
static QString HtWlan;
static int open_flag;

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wifi)
{
    ui->setupUi(this);

    qApp->installEventFilter(this);
    wifi_bt_t = new wifi_bt_interface(this);

    WifiConnectDialog = new WifiConDialog(this);

    screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();

    if(s_width < s_height)
    {
        screen_flag = 1;
    }
    open_flag = 1;
    wifi_font();
    screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();

    LoadLabel = new QLabel(this);
    LoadLabel->move(screen_width/2 ,screen_height/2);
    pMovie = new QMovie("://button_image/loading.webp");
    LoadLabel->setFixedSize(50, 50);
    LoadLabel->setScaledContents(true);
    LoadLabel->setMovie(pMovie);
    ui->label_hoststaus->setText("not connect");
    //hotspot_sql();
    myThread = new QThread(this);
    WifiThread = new wifi_thread();

    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(wifi_refresh()));
    connect(this,SIGNAL(ToThread()),WifiThread,SLOT(Thread_Fun()));
    connect(this,SIGNAL(wifi_scan_msg()),WifiThread,SLOT(wifi_scan_thread()));
    connect(this,SIGNAL(wifi_activation_msg(QString)),WifiThread,SLOT(wifi_activation_thread(QString)));
    connect(WifiConnectDialog,SIGNAL(wifidial_close()),this,SLOT(wifidailog_hide()));

    connect(WifiThread,SIGNAL(send_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    WifiThread->moveToThread(myThread);
    myThread->start();

    //emit ToThread();
    //emit wifi_scan_msg();
    ui->stackedWidget->setCurrentIndex(3);
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget_3->setCurrentIndex(0);

    connect(ui->WifiListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ListWidgeItem_clicked()));
    connect(ui->WifiListWidget_2,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ListWidgeItem2_clicked()));
    connect(this,SIGNAL(hotspot_build_msg(QString,QString,QString)),WifiThread,SLOT(hotspot_build_thread(QString,QString,QString)));
    connect(WifiThread,SIGNAL(wifi_info_fresh_msg(QString)),this,SLOT(wifi_info_fresh(QString)));

    connect(WifiConnectDialog,SIGNAL(wifi_connect_dialog_signal(QString,QString)),WifiThread,SLOT(wifi_connect_thread(QString,QString)));
    connect(WifiConnectDialog,SIGNAL(wifi_info_fresh_msg(QString)),this,SLOT(wifi_info_fresh(QString)));
    connect(WifiThread,SIGNAL(wifi_wait_end_msg()),WifiConnectDialog,SLOT(wifi_wait_end_func()));
    connect(this,SIGNAL(scan_wlan()),WifiThread,SLOT(wlan_scan()));
    connect(WifiThread,SIGNAL(rewlan_scan(QString)),this,SLOT(scan_hotwlan(QString)));
    emit scan_wlan();
}

wifi::~wifi()
{
    delete ui;
    delete wifi_bt_t;
    //delete WifiExistDialog;
    delete WifiConnectDialog;

    delete WifiThread;
    delete myThread;
}

void wifi::recv_msg(int signal_type, QString strResult)
{
    int flag;
    QStringList scanlist;
    QString ScanResult;
    QString wifi_name;

    QString tmp,nameStr,signalStr;
    pMovie->stop();
    LoadLabel->close();

    switch (signal_type)
    {
    case wifi_scan_signal:
        ScanResult = strResult;
        scanlist = ScanResult.split("\n");
        scanlist.removeAll(QString(""));
        ui->WifiListWidget->clear();
        flag = open_flag;
        if(flag == 0)
            return ;

        for(int i = 0; i < scanlist.size(); i++)
        {
            tmp = scanlist.at(i);
//            ui->WifiListWidget->addItem(tmp.left(tmp.size()));
            nameStr = tmp.split(":").at(0);
            if(nameStr.isEmpty())
            {
                continue;
            }
            signalStr = tmp.split(":").at(1);
            int signal = signalStr.toInt();
            wifi_signalshow(nameStr,signal);
        }
        ui->WifiListWidget->setCurrentRow(0);

//        ui->WifiListWidget->setStyleSheet("background-color:transparent");
        ui->WifiListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        break;
    case wifi_connect_signal :
        //qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;
        if(strResult == QString(1))
        {
            wifi_bt_t->wifi_passwd_write(WifiConnectDialog->GetWifiNameText(),WifiConnectDialog->GetPasswdText());

            wifiDB.insert_table2("wifiPasswd",WifiConnectDialog->GetWifiNameText(),WifiConnectDialog->GetPasswdText());

            wifi_info_fresh(WifiConnectDialog->GetWifiNameText());
            ui->stackedWidget->setCurrentIndex(3);
            ui->stackedWidget_2->setCurrentIndex(0);
            ui->WifiListWidget->setEnabled(true);

            WifiConnectDialog->SetWifiNameText("");
            WifiConnectDialog->SetPasswdText("");
            WifiConnectDialog->close();
        }
        else
        {
            QMessageBox::critical(this,"information",tr("Connection failed!"));
        }
        break;
    case wifi_activation_signal:
        if(strResult != QString(1))
        {
            wifi_name = ui->WifiListWidget->currentItem()->text();
            //emit wifi_activation_msg(wifi_name);
            wifi_info_fresh(wifi_name);
        }
        break;
    case hotspot_build_signal:
        if(strResult == QString(1))
        {
            if(ui->HotspotBuildBtn->text() == "up")
            {
                QMessageBox::information(this,"information",tr("Connection succeeded!"));
            }
            else
            {
                QMessageBox::information(this,"information",tr("Change succeeded!"));
            }
            ui->label_hostpotname->setText(HtName);
            ui->label_hotspass->setText(HtPasswd);
            ui->label_WLAN->setText(HtWlan);
            ui->label_hoststaus->setText("connection");
            QStringList list = wifiDB.table_show("hostpot");
            if(!list.isEmpty())
            {
                QString name = list.at(0);
                wifiDB.delete_record_by_name("hostpot",name);
            }
            wifiDB.insert_table3("hostpot",HtName,HtPasswd,HtWlan);
            //wifiDB.table_debug("hotspot");
            if(HtWlan == "wlan0")
            {
                hotconnect_falg = 1;
            }
            else
            {
                hotconnect_falg = 0;
            }
             ui->stackedWidget->setCurrentIndex(2);
             ui->stackedWidget_3->setCurrentIndex(1);
             if(ui->HotspotBuildBtn->text() == "up")
             {
                 ui->HotspotBuildBtn->setText("change");
             }
        }
        else
        {
            QMessageBox::critical(this,"information",tr("Connection failed!"));
        }
        break;
    default:
        break;
    }
//    pMovie->stop();
//    LoadLabel->close();
}

void wifi::wifi_info_fresh(QString wifi_name)
{
    WifiInfo = wifi_bt_t->get_wifistatus(wifi_name);
    ui->WifiInfoPasswdLab->setText(WifiInfo.passwd);
    ui->WifiInfoNameLab->setText(WifiInfo.name);
    ui->WifiInfoActiveLab->setText(WifiInfo.active);
    ui->WifiInfoSignalLab->setText(WifiInfo.signal);
    ui->WifiInfoSecurityLab->setText(WifiInfo.secrity);
    ui->WifiInfoIpaddressLab->setText(WifiInfo.ip_address);
}

void wifi::ListWidgeItem_clicked()
{    
    QListWidgetItem *item = ui->WifiListWidget->currentItem();

    QWidget* pwig = ui->WifiListWidget->itemWidget(item); // 获取里面的QWidget
    QList<QLabel*> labelList = pwig->findChildren<QLabel*>();  // 获取所有的Qlabel

//    QString wifi_name = ui->WifiListWidget->currentItem()->text();
    QString wifi_name = labelList.at(0)->text();

//    int flag = wifi_bt_t->wifi_connect_exist(wifi_name);
//    if(flag == 1)
//    {
//        wifi_info_fresh(wifi_name);

//        if(QString("no") == WifiInfo.active)
//        {
//            emit wifi_activation_msg(wifi_name);
//            LoadLabel->show();
//            pMovie->start();
//        }
//        ui->ChangePasswdBtn->show();
//        ui->WifiExistRemoveBtn->show();
//        ui->stackedWidget->setCurrentIndex(3);
//        ui->stackedWidget_2->setCurrentIndex(0);
//        ui->WifiListWidget->setEnabled(true);
//    }
//    else
//    {
        ui->WifiListWidget->setEnabled(false);
        ui->WifiListWidget->setSelectionMode(QAbstractItemView::NoSelection);
        ui->WifiListWidget->setFocusPolicy(Qt::NoFocus);
      //  this->setWindowModality(Qt::ApplicationModal);
        if(screen_flag == 1)
        {
           WifiConnectDialog->resize(s_height*2/3,s_width*3/5);
           WifiConnectDialog->move(s_width-((s_width-WifiConnectDialog->height())/2),s_height/2-WifiConnectDialog->width()/2);
        }
        else
        {
           WifiConnectDialog->resize(width()*3/5,height()*3/5);
        }
        WifiConnectDialog->show();
        this->setAttribute(Qt::WA_ShowWithoutActivating,true);
        this->setFocusPolicy(Qt::NoFocus);
        WifiConnectDialog->activateWindow();WifiConnectDialog->setFocus();
        WifiConnectDialog->SetWifiNameText(wifi_name);
        WifiConnectDialog->SetWifiOkBtnText(tr("connect"));
 //   }
}

void wifi::on_ReturnBtn_clicked()
{
    WifiConnectDialog->close();
    timer->stop();
    emit Mysignal();
}

void wifi::WifiStatus_show()
{
    QString strCmd = QString("iw dev wlan0 link | grep SSID |awk '{for(i=2;i<=NF;i++){printf \"%s \", $i}; printf \"\\n\"}'");
    QString wifi_name = wifi_bt_t->executeLinuxCmd(strCmd);
    //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "wifi_name" << wifi_name;
    QStringList list = wifiDB.table_show("wifiPasswd");

    if(wifi_name.isEmpty() || (hotconnect_falg == 1) || (open_flag == 0)||(list.isEmpty()))
    {
        ui->WifiInfoNameLab->clear();
        ui->WifiInfoPasswdLab->clear();
        ui->WifiInfoActiveLab->setText(tr("not connect"));
        ui->WifiInfoSignalLab->clear();
        ui->WifiInfoSecurityLab->clear();
        ui->WifiInfoIpaddressLab->clear();
    }
    else
    {
        wifi_info_fresh(wifi_name.remove(" \n"));
    }
    ui->stackedWidget->setCurrentIndex(3);
    ui->stackedWidget_2->setCurrentIndex(0);
}

void wifi::on_WifiStatusBtn_clicked()
{
    /*
    QMessageBox::StandardButton reply;

//    strCmd = QString("nmcli con show --active |grep hotspot | wc -l");
    QString strCmd = QString("nmcli con show --active |grep hotspot | awk '{print $1}'");
    QString strResult = wifi_bt_t->executeLinuxCmd(strCmd);
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "strResult" << strResult;

    //if(strResult.remove("\n") != QString("0"))
    if(!strResult.isEmpty())
    {
        reply = QMessageBox::question(this,tr("QMessageBox::question()"),tr("Are you sure you want to deactivated hotspot?"),
                                      QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
        if(reply == QMessageBox::Cancel)
        {
            return;
        }
        else
        {
            strCmd = QString("nmcli con down %1").arg(strResult);
            strResult = wifi_bt_t->executeLinuxCmd(strCmd);
            qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "strResult" << strResult;
        }
        QThread::sleep(3);
    }
    */
    if(hotconnect_falg == 1)
    {
        QMessageBox::critical(this,"information",tr("Please turn off the hotspot first!"));
        return;
    }
        timer->stop();
        WifiStatus_show();
}

void wifi::on_WifiConnectBtn_clicked()
{
    timer->start(5000);
    if(hotconnect_falg == 1)
    {
      QMessageBox::critical(this,"information",tr("Please turn off the hotspot first!"));
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
        int flag = open_flag;
        if(flag == 1)
        {
          emit wifi_scan_msg();
        }
    }
}

void wifi::on_HotspotConBtn_clicked()
{
    ui->HotspotBuildBtn->setFocus();
    ui->HtName->clear();
    ui->HtPasswd->clear();

   // QString strCmd = QString("nmcli con show |grep hotspot | awk '{print $1}'");
   //QString  HtName= wifi_bt_t->executeLinuxCmd(strCmd);
   // qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "HtName" << HtName;

    if(!HtName.isEmpty())
    {
        ui->HtName->setText(HtName);
        ui->HtPasswd->setText(HtPasswd);
        ui->comboBox->setCurrentText(HtWlan);
    }
    timer->stop();
    ui->stackedWidget->setCurrentIndex(2);
    if((ui->stackedWidget_3->currentIndex() == 0)&&(ui->HotspotBuildBtn->text() == "change"))
    {
        ui->stackedWidget_3->setCurrentIndex(1);
    }
}

//void wifi::on_RefreshBtn_clicked()
//{
    //int flag;

//    flag = ui->WifiSwitch->isToggled();
//    qDebug() << "Line:" << __LINE__ << "flag:" << flag;

 //   if(flag == 1) // open
//    {
//        LoadLabel->show();
//        pMovie->start();
//        emit wifi_scan_msg();
 //   }
//    return;
//}

void wifi::on_ChangePasswdBtn_clicked()
{
    QString connectstr = ui->WifiInfoActiveLab->text();
    QString wifi_name = ui->WifiInfoNameLab->text();
    if(connectstr == QString(tr("not connect")))
    {
        return ;
    }
    if(screen_flag == 1)
    {
       WifiConnectDialog->resize(s_height*2/3,s_width*3/5);
       WifiConnectDialog->move(s_width-((s_width-WifiConnectDialog->height())/2),s_height/2-WifiConnectDialog->width()/2);
    }
    else
    {
       WifiConnectDialog->resize(width()*3/5,height()*3/5);
    }
    WifiConnectDialog->show();
    WifiConnectDialog->activateWindow();WifiConnectDialog->setFocus();

    WifiConnectDialog->SetWifiNameText(wifi_name);
    WifiConnectDialog->SetWifiOkBtnText("OK");
}

void wifi::on_WifiExistRemoveBtn_clicked()
{
    QString wifi_name;
    QString strResult;

    QMessageBox::StandardButton reply;

    QString connectstr = ui->WifiInfoActiveLab->text();
    wifi_name = ui->WifiInfoNameLab->text();
    if(connectstr == QString(tr("not connect")))
    {
        return ;
    }

    reply = QMessageBox::question(this,tr("QMessageBox::question()"),tr("Are you sure you want to delete it?"),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(reply == QMessageBox::No)
    {
        return;
    }
    else if(reply == QMessageBox::Yes)
    {
        strResult = wifi_bt_t->wifi_connection_remove(wifi_name);

        bool downResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);

        if(downResult == 1)
        {
            ui->stackedWidget->setCurrentIndex(0);
            //timer->start(5000);
            wifiDB.delete_record_by_name(QString("wifiPasswd"),wifi_name);
        }
        else
        {
            QMessageBox::critical(this,"information",tr("remove failed!"));
        }
    }
}

void wifi::on_HotspotBuildBtn_clicked()
{
    int ind = ui->stackedWidget_3->currentIndex();
    if(ind == 1)
    {
        ui->stackedWidget_3->setCurrentIndex(0);
        ui->HtName->setText(HtName);
        ui->HtPasswd->setText(HtPasswd);
        ui->comboBox->setCurrentText(HtWlan);
        return;
    }
    HtName = ui->HtName->text();
    HtPasswd = ui->HtPasswd->text();
    HtWlan = ui->comboBox->currentText();

    if(HtName.isEmpty() || HtPasswd.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Input cannot be empty!"),
                         0,this);
         mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
         mesg.setFocusPolicy(Qt::NoFocus);
         mesg.addButton(tr("OK"),QMessageBox::YesRole);
         if(screen_flag == 1)
         mesg.move(s_width*2/3,s_height/3);
         else
         mesg.move(s_width/3,s_height/3);
         mesg.exec();
        return;
    }

    if(HtPasswd.length() < 8)
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("The number of password digits must be greater than 8"),
                         0,this);
         mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
         mesg.setFocusPolicy(Qt::NoFocus);
         mesg.addButton(tr("OK"),QMessageBox::YesRole);
         if(screen_flag == 1)
         mesg.move(s_width*2/3,s_height/3);
         else
         mesg.move(s_width/3,s_height/3);
         mesg.exec();
        return ;
    }

    emit hotspot_build_msg(HtWlan,HtName,HtPasswd);
    LoadLabel->show();
    pMovie->start();
}

void wifi::on_HotspotDownBtn_clicked()
{
    ui->label_hostpotname->clear();
    ui->label_hotspass->clear();
    ui->label_hoststaus->setText(tr("not connect"));
    ui->label_WLAN->clear();
    QString strResult;

    QString strCmd = QString("nmcli con show --active|grep hotspot | awk '{print $1}'");
    strResult = wifi_bt_t->executeLinuxCmd(strCmd);
    //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "strResult" << strResult;

    if(strResult.isEmpty())
    {
        QMessageBox::information(this,"information",tr("Hotspot not connected!"));
        return ;
    }

    bool retflag = wifi_bt_t->hotspot_disconnect();
    if(retflag == true)
    {
        QMessageBox::information(this,"information",tr("successfully deactivated!"));
        hotconnect_falg = 0;
        wifiDB.delete_record_by_name(QString("hostpot"),HtName);
        HtName.clear();
        ui->HotspotBuildBtn->setText("up");
        ui->stackedWidget_3->setCurrentIndex(0);
    }
    else if(retflag == false)
    {
        QMessageBox::critical(this,"information",tr("deactivated failed!"));
    }
}

void wifi::wifi_refresh()
{
    int flag = open_flag;
    if(flag == 1)
    {
        int x = ui->stackedWidget->currentIndex();
        if(x == 0)
        {
            emit wifi_scan_msg();
        }
    }
}

void wifi::language_reload()
{
    ui->retranslateUi(this);
    WifiConnectDialog->language_reload();
}

void wifi::wifi_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = s_width / realX * 2.54;
    qreal realHeight = s_height / realY *2.54;
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
 //   ui->groupBox->setFont(font);
    ui->WifiStatusBtn->setFont(font);
    ui->WifiConnectBtn->setFont(font);
    ui->HotspotConBtn->setFont(font);
    ui->ChangePasswdBtn->setFont(font);
    ui->WifiExistRemoveBtn->setFont(font);
    ui->label_1->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->label_4->setFont(font);
    ui->label_5->setFont(font);
    ui->label_7->setFont(font);
    ui->HotspotPasswd->setFont(font);
    ui->HotspotBuildBtn->setFont(font);
    ui->HotspotDownBtn->setFont(font);
    ui->HotspotName->setFont(font);
    ui->WifiInfoNameLab->setFont(font);
    ui->WifiInfoIpaddressLab->setFont(font);
    ui->WifiInfoPasswdLab->setFont(font);
    ui->WifiInfoSignalLab->setFont(font);
    ui->WifiInfoSecurityLab->setFont(font);
    ui->WifiInfoActiveLab->setFont(font);
    ui->label->setFont(font);
    ui->label_8->setFont(font);
    ui->label_9->setFont(font);
    ui->label_hostpotname->setFont(font);
    ui->label_hoststaus->setFont(font);
    ui->label_hotspass->setFont(font);
    ui->label_10->setFont(font);
    ui->comboBox->setFont(font);
    ui->label_WLAN->setFont(font);
    ui->groupBox_2->setFont(font);
    ui->HtName->setFont(font);
    ui->HtPasswd->setFont(font);
    ui->pushButton->setFont(font);
    ui->groupBox->setFont(font);
}

void wifi::closeEvent(QCloseEvent *event)
{
   WifiConnectDialog->show();
   WifiConnectDialog->hide();
   QWidget::closeEvent(event);
}

void wifi::showEvent(QShowEvent *event)
{
    timer->start(5000);
    emit wifi_scan_msg();
    QWidget::showEvent(event);
}

void wifi::wifidailog_hide()
{
   ui->WifiListWidget->setEnabled(true);
}

void wifi::scan_hotwlan(QString wlan)
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

void wifi::on_open_btn_clicked()
{
    if(open_flag == 0)
    {
        open_flag = 1;
        ui->open_btn->setText(tr("close"));
        LoadLabel->show();
        pMovie->start();
        emit wifi_scan_msg();
        timer->start();
        for(int i = 0; i < ui->WifiListWidget->count(); i++)
        {
            ui->WifiListWidget->setItemHidden(ui->WifiListWidget->item(i), false);
        }
        wifi_bt_t->wifi_enable(true);
    }
    else
    {
        open_flag = 0;
        ui->open_btn->setText(tr("open"));
        for(int i = 0; i < ui->WifiListWidget->count(); i++)
        {
            ui->WifiListWidget->setItemHidden(ui->WifiListWidget->item(i), true);
        }
        wifi_bt_t->wifi_enable(false);
        pMovie->stop();
        timer->stop();
        LoadLabel->close();
    }
}

void wifi::hotspot_sql()
{
    if(wifi_bt_t->hotspot_sql())
    {
        QStringList list = wifiDB.table_show("hostpot");
        if(!list.isEmpty())
        {
            ui->label_hostpotname->setText(list.at(0));
            ui->label_hotspass->setText(list.at(1));
            ui->label_WLAN->setText(list.at(2));
            ui->label_hoststaus->setText("connection");
           // ui->comboBox->addItem(list.at(2));
            ui->comboBox->setCurrentText(list.at(2));
            HtName = list.at(0);
            HtPasswd = list.at(1);

            if( ui->label_WLAN->text() == "wlan0")
            {
                hotconnect_falg = 1;
            }
            else
            {
                hotconnect_falg = 0;
            }
            ui->stackedWidget->setCurrentIndex(2);
            ui->stackedWidget_3->setCurrentIndex(1);
            ui->HotspotBuildBtn->setText("change");
        }
    }
}

void wifi::wifi_signalshow(QString nameStr,int signal)

{
    QWidget *widget=new QWidget(this);
    QHBoxLayout *horLayout = new QHBoxLayout();
    QVBoxLayout *v1 = new QVBoxLayout();
    QVBoxLayout *v2 = new QVBoxLayout();
    QVBoxLayout *v3 = new QVBoxLayout();
    QVBoxLayout *v4 = new QVBoxLayout();
    QLabel *nameLabel = new QLabel(widget);
    QLabel *line1 = new QLabel(widget);
    QLabel *line2 = new QLabel(widget);
    QLabel *line3 = new QLabel(widget);
    QLabel *line4 = new QLabel(widget);
    QListWidgetItem *item = new QListWidgetItem();
    QSpacerItem *sparcer1 = new QSpacerItem(0,2,QSizePolicy::Fixed,QSizePolicy::Expanding);
    QSpacerItem *sparcer2 = new QSpacerItem(0,2,QSizePolicy::Fixed,QSizePolicy::Expanding);
    QSpacerItem *sparcer3 = new QSpacerItem(0,2,QSizePolicy::Fixed,QSizePolicy::Expanding);
    QSpacerItem *sparcer4 = new QSpacerItem(0,2,QSizePolicy::Fixed,QSizePolicy::Expanding);
    //QSpacerItem *sparcer5 = new QSpacerItem(2,0,QSizePolicy::Expanding,QSizePolicy::Fixed);
    nameLabel->setText(nameStr);

    horLayout->setContentsMargins(50,0,160,0);

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

void wifi::on_pushButton_clicked()
{
    QStringList list = wifiDB.table_show("wifiPasswd");
    ui->WifiListWidget_2->clear();
    if(!list.isEmpty())
    {
        for(int i = 0; i < list.size(); i++)
        {
            QString nameStr = list.at(i);
            QHBoxLayout *horLayout = new QHBoxLayout();
            horLayout->setContentsMargins(10,0,0,0);
            horLayout->setSpacing(0);

            QWidget *widget=new QWidget(this);
            QLabel *nameLabel = new QLabel(widget);
            QLabel *conLabel = new QLabel(widget);
            QListWidgetItem *item = new QListWidgetItem();
            QString name = ui->WifiInfoNameLab->text();
            if(!name.isEmpty())
            {
                if(!QString::compare(name,nameStr,Qt::CaseSensitive))
                {
                    conLabel->setText("connect");
                }
                else
                {
                    conLabel->setText("not connect");
                }
            }
            nameLabel->setText(nameStr);
            horLayout->addWidget(nameLabel);
            horLayout->addWidget(conLabel);
            horLayout->setStretchFactor(nameLabel,2);
            horLayout->setStretchFactor(conLabel,1);
            widget->setLayout(horLayout);
            ui->WifiListWidget_2->addItem(item);
            ui->WifiListWidget_2->setItemWidget(item, widget);
            i++;
        }
    }
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->WifiListWidget_2->setCurrentRow(0);
    ui->WifiListWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
}

void wifi::ListWidgeItem2_clicked()
{
    QListWidgetItem *item = ui->WifiListWidget_2->currentItem();
    QWidget* pwig = ui->WifiListWidget_2->itemWidget(item); // 获取里面的QWidget
    QList<QLabel*> labelList = pwig->findChildren<QLabel*>();  // 获取所有的Qlabel
    QString wifi_name = labelList.at(0)->text();

    int flag = wifi_bt_t->wifi_connect_exist(wifi_name);
    if(flag == 1)
    {
        wifi_info_fresh(wifi_name);
        if(QString("no") == WifiInfo.active)
        {
            emit wifi_activation_msg(wifi_name);
            LoadLabel->show();
            pMovie->start();
        }
        ui->stackedWidget->setCurrentIndex(3);
        ui->stackedWidget_2->setCurrentIndex(0);
    }
}
