#include "wifi.h"
#include "ui_wifi.h"
#include <QSpinBox>
#include <QToolBox>

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wifi)
{
    ui->setupUi(this);

    wifi_bt_t = new wifi_bt_interface(this);

    WifiConnectDialog = new WifiConDialog(this);

    // 设置状态、样式
    ui->WifiSwitch->setToggle(true);
    ui->WifiSwitch->setCheckedColor(QColor(100, 225, 100));
    connect(ui->WifiSwitch,SIGNAL(toggled(bool)),this,SLOT(BtnChange_flag(bool)));

    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();
    //qDebug()<<screen_width<<screen_height;

    LoadLabel = new QLabel(this);
    //LoadLabel->resize(100,100);
    LoadLabel->move(screen_width/2 ,screen_height/2);
    pMovie = new QMovie("://t507_button_image/loading.webp");
    LoadLabel->setFixedSize(50, 50);
    LoadLabel->setScaledContents(true);
    LoadLabel->setMovie(pMovie);
    //pMovie->start();

    myThread = new QThread(this);
    WifiThread = new wifi_thread();
    connect(this,SIGNAL(ToThread()),WifiThread,SLOT(Thread_Fun()));
    connect(this,SIGNAL(wifi_scan_msg()),WifiThread,SLOT(wifi_scan_thread()));
    connect(this,SIGNAL(wifi_activation_msg(QString)),WifiThread,SLOT(wifi_activation_thread(QString)));

    connect(WifiThread,SIGNAL(send_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    WifiThread->moveToThread(myThread);
    myThread->start();

    //emit ToThread();
    emit wifi_scan_msg();

    connect(ui->WifiListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ListWidgeItem_clicked()));
    connect(this,SIGNAL(hotspot_build_msg(QString,QString)),WifiThread,SLOT(hotspot_build_thread(QString,QString)));
    connect(WifiThread,SIGNAL(wifi_info_fresh_msg(QString)),this,SLOT(wifi_info_fresh(QString)));

    connect(WifiConnectDialog,SIGNAL(wifi_connect_dialog_signal(QString,QString)),WifiThread,SLOT(wifi_connect_thread(QString,QString)));
    connect(WifiConnectDialog,SIGNAL(wifi_info_fresh_msg(QString)),this,SLOT(wifi_info_fresh(QString)));
    connect(WifiThread,SIGNAL(wifi_wait_end_msg()),WifiConnectDialog,SLOT(wifi_wait_end_func()));

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

void wifi::BtnChange_flag(bool flag)
{
    //qDebug() << "LINe:" << __LINE__<<"switchflag " << switchflag;
    flag = ui->WifiSwitch->isToggled();

    if(flag == 1) // open
    {
//        if(ui->WifiListWidget->count() == 0)
//        {
            LoadLabel->show();
            pMovie->start();
            emit wifi_scan_msg();
//        }

        for(int i = 0; i < ui->WifiListWidget->count(); i++)
        {
            ui->WifiListWidget->setItemHidden(ui->WifiListWidget->item(i), false);
        }
        wifi_bt_t->wifi_enable(true);
    }
    else //close
    {
        for(int i = 0; i < ui->WifiListWidget->count(); i++)
        {
            ui->WifiListWidget->setItemHidden(ui->WifiListWidget->item(i), true);
        }
        wifi_bt_t->wifi_enable(false);
        pMovie->stop();
        LoadLabel->close();
    }
}

void wifi::recv_msg(int signal_type, QString strResult)
{
    int flag;
    QStringList scanlist;
    QString ScanResult;
    QString wifi_name;
    pMovie->stop();
    LoadLabel->close();

    switch (signal_type)
    {
    case wifi_scan_signal:
        ScanResult = strResult;
        scanlist = ScanResult.split("\n");
        scanlist.removeAll(QString(""));

        ui->WifiListWidget->clear();
        flag = ui->WifiSwitch->isToggled();

        //qDebug() << "line:" << __LINE__ << "strResult:" << strResult;

        if(flag == 0)
            return ;

        for(int i = 0; i < scanlist.size(); i++)
        {
            QString tmp = scanlist.at(i);
            //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "tmp:" << tmp;
            ui->WifiListWidget->addItem(tmp.left(tmp.size()));
        }
        ui->WifiListWidget->setCurrentRow(0);

//        ui->WifiListWidget->setStyleSheet("background-color:transparent");

//        ui->WifiListWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:30px;}");
//        ui->WifiListWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{height:30px;}");
        ui->WifiListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        break;
    case wifi_connect_signal :
        qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;
        if(strResult == QString(1))
        {
//            QMessageBox::information(this,"information",tr("Connection succeeded!"));
            wifi_bt_t->wifi_passwd_write(WifiConnectDialog->GetWifiNameText(),WifiConnectDialog->GetPasswdText());

            wifi_info_fresh(WifiConnectDialog->GetWifiNameText());
            ui->stackedWidget->setCurrentIndex(3);

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
        //qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;
        if(strResult == QString(1))
        {
            QMessageBox::information(this,"information",tr("Connection succeeded!"));
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
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "wifi_name"<<wifi_name;

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
    qDebug() << "line:" << __LINE__ << "istWidgeItem_clicked";
    QString wifi_name = ui->WifiListWidget->currentItem()->text();
    qDebug() << "line:" << __LINE__ << "currentItem()->text = " << wifi_name;

    int flag = wifi_bt_t->wifi_connect_exist(wifi_name);
    if(flag == 1)
    {
        //qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__;
        wifi_info_fresh(wifi_name);

        if(QString("no") == WifiInfo.active)
        {
            emit wifi_activation_msg(wifi_name);
            LoadLabel->show();
            pMovie->start();
        }
        ui->ChangePasswdBtn->show();
        ui->WifiExistRemoveBtn->show();
        ui->stackedWidget->setCurrentIndex(3);
    }
    else
    {
        WifiConnectDialog->show();
        WifiConnectDialog->SetWifiNameText(ui->WifiListWidget->currentItem()->text());
        WifiConnectDialog->SetWifiOkBtnText("connect");
    }
}

void wifi::on_ReturnBtn_clicked()
{
    WifiConnectDialog->close();
    emit Mysignal();
}

void wifi::WifiStatus_show()
{
//    QString strCmd = QString("iw dev wlan0 link | grep SSID |awk '{print $2}'");
    QString strCmd = QString("iw dev wlan0 link | grep SSID |awk '{for(i=2;i<=NF;i++){printf \"%s \", $i}; printf \"\\n\"}'");
    QString wifi_name = wifi_bt_t->executeLinuxCmd(strCmd);
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "wifi_name" << wifi_name;

    if(wifi_name.isEmpty())
    {
        ui->WifiInfoNameLab->setText("Not connected");
        ui->WifiInfoPasswdLab->clear();
        ui->WifiInfoActiveLab->clear();
        ui->WifiInfoSignalLab->clear();
        ui->WifiInfoSecurityLab->clear();
        ui->WifiInfoIpaddressLab->clear();
    }
    else
    {
        wifi_info_fresh(wifi_name.remove(" \n"));
    }
    ui->stackedWidget->setCurrentIndex(3);
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
    WifiStatus_show();
}

void wifi::on_WifiConnectBtn_clicked()
{

    ui->stackedWidget->setCurrentIndex(0);
}

void wifi::on_HotspotConBtn_clicked()
{
    QString HtPasswd;
    ui->HtName->clear();
    ui->HtPasswd->clear();

    QString strCmd = QString("nmcli con show |grep hotspot | awk '{print $1}'");
    QString  HtName= wifi_bt_t->executeLinuxCmd(strCmd);
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "HtName" << HtName;

    if(!HtName.isEmpty())
    {
//        qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__;
        HtPasswd = wifi_bt_t->wifi_passwd_read(HtName.remove("\n"));
//        qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "HtPasswd" << HtPasswd;
        ui->HtName->setText(HtName.remove(0,7));
        ui->HtPasswd->setText(HtPasswd);
    }

    ui->stackedWidget->setCurrentIndex(2);
}

void wifi::on_RefreshBtn_clicked()
{
    int flag;

    flag = ui->WifiSwitch->isToggled();
    qDebug() << "Line:" << __LINE__ << "flag:" << flag;

    if(flag == 1) // open
    {
        LoadLabel->show();
        pMovie->start();
        emit wifi_scan_msg();
    }
    return;
}

void wifi::on_ChangePasswdBtn_clicked()
{
    QString wifi_name = ui->WifiInfoNameLab->text();
    qDebug() << "--LINE--: " << __LINE__<< "FUNC:" << __FUNCTION__<< "currentItem()->text = " << wifi_name;
    if(wifi_name == QString("Not connected"))
    {
        return ;
    }

    WifiConnectDialog->show();
    WifiConnectDialog->SetWifiNameText(wifi_name);
    WifiConnectDialog->SetWifiOkBtnText("OK");
}

void wifi::on_WifiExistRemoveBtn_clicked()
{
    QString wifi_name;
    QString strResult;

    QMessageBox::StandardButton reply;

    wifi_name = ui->WifiInfoNameLab->text();
    qDebug() << "--LINE--: " << __LINE__<< "FUNC:" << __FUNCTION__<< "currentItem()->text = " << wifi_name;
    if(wifi_name == QString("Not connected"))
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
        qDebug() << "--LINE--: " << __LINE__<< "FUNC:" << __FUNCTION__<< strResult;

        bool downResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);
        qDebug() << "--LINE--: " << __LINE__<< "FUNC:" << __FUNCTION__<< downResult;

        if(downResult == 1)
        {
            //QMessageBox::information(this,"information",tr("remove succeeded!"));
            ui->stackedWidget->setCurrentIndex(0);
            wifi_bt_t->wifi_passwd_delete(wifi_name);
        }
        else
        {
            QMessageBox::critical(this,"information",tr("remove failed!"));
        }
    }
}

void wifi::on_HotspotBuildBtn_clicked()
{
    QString HtName = ui->HtName->text();
    QString HtPasswd = ui->HtPasswd->text();

    if(HtName.isEmpty() || HtPasswd.isEmpty())
    {
        QMessageBox::information(this,"information",tr("Input cannot be empty!"));
        return;
    }

    if(HtPasswd.length() < 8)
    {
        QMessageBox::information(this,"information",tr("The number of password digits must be greater than 8"));
        return ;
    }

    emit hotspot_build_msg(HtName,HtPasswd);
    LoadLabel->show();
    pMovie->start();

}

void wifi::on_HotspotDownBtn_clicked()
{
    QString strResult;

    QString strCmd = QString("nmcli con show --active|grep hotspot | awk '{print $1}'");
    strResult = wifi_bt_t->executeLinuxCmd(strCmd);
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__<< "strResult" << strResult;

    if(strResult.isEmpty())
    {
        QMessageBox::information(this,"information",tr("Hotspot not connected!"));
        return ;
    }

    bool retflag = wifi_bt_t->hotspot_disconnect();
    if(retflag == true)
    {
        QMessageBox::information(this,"information",tr("successfully deactivated!"));
    }
    else if(retflag == false)
    {
        QMessageBox::critical(this,"information",tr("deactivated failed!"));
    }
}

void wifi::language_reload()
{
    ui->retranslateUi(this);
}

