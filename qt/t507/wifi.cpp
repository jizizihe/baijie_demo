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
    //WifiConnectDialog->resize(400,300);
    //WifiConnectDialog->setStyleSheet("QDialog {border:2px solid gray;}");

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

    connect(WifiConnectDialog,SIGNAL(wifi_connect_dialog_signal(QString,QString)),WifiThread,SLOT(wifi_connect_thread(QString,QString)));
}

wifi::~wifi()
{
    delete ui;
    delete wifi_bt_t;
    //delete WifiExistDialog;
    delete WifiConnectDialog;
}

void wifi::BtnChange_flag(bool flag)
{
    //qDebug() << "LINe:" << __LINE__<<"switchflag " << switchflag;
    flag = ui->WifiSwitch->isToggled();

    if(flag == 1) // open
    {
        if(ui->WifiListWidget->count() == 0)
        {
            LoadLabel->show();
            pMovie->start();
            emit wifi_scan_msg();
        }

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

    switch (signal_type)
    {
    case wifi_scan_signal:
        ScanResult = strResult;
        scanlist = ScanResult.split("\n");
        scanlist.removeAll(QString(""));

        ui->WifiListWidget->clear();
        flag = ui->WifiSwitch->isToggled();
        //qDebug() << "line:" << __LINE__ << "flag:" << flag;

        if(flag == 0)
        {
            return ;
        }

        for(int i = 0; i < scanlist.size(); i++)
        {
            QString tmp = scanlist.at(i);
            //qDebug() << tmp;
            ui->WifiListWidget->addItem(tmp.left(tmp.size() - 1));
        }
        ui->WifiListWidget->setCurrentRow(0);
        ui->WifiListWidget->setStyleSheet(R"(
            QListWidget { outline: none; border:1px solid gray; color: black; }
            QListWidget::Item { width: 400px; height: 50px; }
            QListWidget::Item:hover { background: #4CAF50; color: white; }
            QListWidget::item:selected { background: #e7e7e7; color: #f44336; }
            QListWidget::item:selected:!active { background: lightgreen; }
           )");
        ui->WifiListWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:30px;}");
        ui->WifiListWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{height:30px;}");
        ui->WifiListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        break;
    case wifi_connect_signal :
        qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;
        if(strResult == QString(1))
        {
            QMessageBox::information(this,"information",tr("Connection succeeded!"));
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
    pMovie->stop();
    LoadLabel->close();
}

void wifi::ListWidgeItem_clicked()
{    
    QString infosecurity;
    qDebug() << "line:" << __LINE__ << "istWidgeItem_clicked";
    QString wifi_name = ui->WifiListWidget->currentItem()->text();
    qDebug() << "line:" << __LINE__ << "currentItem()->text = " << wifi_name;

    int flag = wifi_bt_t->wifi_connect_exist(wifi_name);
    if(flag == 1)
    {
        ui->WifiInfoNameLab->setText(wifi_name);
        infosecurity = wifi_bt_t->get_wifisecurity(wifi_name);
        ui->WifiInfoSecurityLab->setText(infosecurity);

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

void wifi::on_WifiConnectBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void wifi::on_WifiQualityBtn_clicked()
{
    int flag;
    QString strResult;

    ui->stackedWidget->setCurrentIndex(1);

    flag = ui->WifiSwitch->isToggled();
    qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "flag:" << flag;

    if(flag == 1) // open
    {
        strResult = wifi_bt_t->get_wifisignalquality();
        qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< "strResult" << strResult;
    }
    else
    {
        strResult = QString(tr("Please turn on the WiFi switch"));
    }

    ui->WifiText->setText(strResult);
}

void wifi::on_WifiStatusBtn_clicked()
{
    int flag;
    QString strResult;

    ui->stackedWidget->setCurrentIndex(1);

    flag = ui->WifiSwitch->isToggled();
    //qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "flag:" << flag;

    if(flag == 1) // open
    {
        strResult = wifi_bt_t->get_wifistatus();
        qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< "strResult" << strResult;
    }
    else
    {
        strResult = QString(tr("Please turn on the WiFi switch"));
    }
    ui->WifiText->setText(strResult);
}

void wifi::on_HotspotConBtn_clicked()
{
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


void wifi::on_toolBox_currentChanged(int index)
{
//    qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "index:" << index;

    if(index == 0)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(index == 1)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void wifi::on_ChangePasswdBtn_clicked()
{
    WifiConnectDialog->show();
    WifiConnectDialog->SetWifiNameText(ui->WifiListWidget->currentItem()->text());
    WifiConnectDialog->SetWifiOkBtnText("OK");

}

void wifi::on_WifiBackBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void wifi::on_WifiActiveBtn_clicked()
{
    QString wifi_name = ui->WifiListWidget->currentItem()->text();
    //qDebug() << "line:" << __LINE__ << "currentItem()->text = " << wifi_name;

    emit wifi_activation_msg(wifi_name);
    LoadLabel->show();
    pMovie->start();
}

void wifi::on_WifiExistRemoveBtn_clicked()
{
    QString wifi_name = ui->WifiListWidget->currentItem()->text();
    qDebug() << "FUNC:" << __FUNCTION__<< "line:" << __LINE__ << "currentItem()->text = " << wifi_name;

    QString strResult = wifi_bt_t->wifi_connection_remove(wifi_name);
    qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< strResult;

    bool downResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);
    qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< downResult;

    if(downResult == 1)
    {
        QMessageBox::information(this,"information",tr("remove succeeded!"));
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        QMessageBox::critical(this,"information",tr("remove failed!"));
    }
}

void wifi::on_HotspotBuildBtn_clicked()
{
    QString HtName = ui->HtName->text();
    QString HtPasswd = ui->HtPasswd->text();

    if(HtName == "" || HtPasswd == "")
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

    //wifi_bt_t->hotspot_connect(HtName ,HtPasswd);
}

void wifi::on_HotspotDownBtn_clicked()
{
    QString strResult;

    strResult = wifi_bt_t->hotspot_disconnect();
    qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< "strResult" << strResult;

    if(strResult == QString(1))
    {
        QMessageBox::information(this,"information",tr("successfully disconnected!"));
    }
    else if(strResult == QString(""))
    {
        QMessageBox::critical(this,"information",tr("disconnect failed!"));
    }
    else
    {
        QMessageBox::information(this,"information",strResult);
    }
}


void wifi::language_reload()
{
    ui->retranslateUi(this);
}
