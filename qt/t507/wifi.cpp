#include "wifi.h"
#include "ui_wifi.h"
#include <QSpinBox>

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wifi)
{
    ui->setupUi(this);

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
    connect(WifiThread,SIGNAL(send_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    WifiThread->moveToThread(myThread);
    myThread->start();

    //emit ToThread();
    emit wifi_scan_msg();

    connect(ui->WifiListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ListWidgeItem_clicked()));
    connect(this,SIGNAL(hotspot_build_msg(QString,QString)),WifiThread,SLOT(hotspot_build_thread(QString,QString)));


    wifi_exist_dialog();
    wifi_connect_dialog();
}

wifi::~wifi()
{
    delete ui;
    delete WifiExistDialog;
    delete WifiConnectDialog;
}

void wifi::BtnChange_flag(bool flag)
{
    bool wifi_open_flag;
    //qDebug() << "LINe:" << __LINE__<<"switchflag " << switchflag;

    //int flag;
    flag = ui->WifiSwitch->isToggled();
    //qDebug() << "line:" << __LINE__ << "flag:" << flag;


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
    case wifi_activation_signal:
    case hotspot_build_signal:
        qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;
        if(strResult == QString(1))
        {
            QMessageBox::information(this,"information","Connection succeeded!");
        }
        else
        {
            QMessageBox::critical(this,"information","Connection failed!");
        }
        break;
    default:
        break;
    }
    pMovie->stop();
    LoadLabel->close();
}

void wifi::wifi_exist_dialog()
{
    WifiExistDialog = new QDialog;
    WifiExistDialog->resize(this->width()/2,this->height()/3*2);
    WifiExistDialog->setStyleSheet("QDialog {border:2px solid gray;}");

    WifiActiveBtn = new QPushButton("Join Network",WifiExistDialog);
    WifiActiveBtn->resize(200,50);
    WifiActiveBtn->move(20,250);
    WifiExistRemoveBtn = new QPushButton("Remove Network",WifiExistDialog);
    WifiExistRemoveBtn->resize(200,50);
    WifiExistRemoveBtn->move(250,250);
    WifiModifyBtn = new QPushButton("Change Password",WifiExistDialog);
    WifiModifyBtn->resize(200,50);
    WifiModifyBtn->move(20,330);
    ExistDialogCloseBtn = new QPushButton("close",WifiExistDialog);
    ExistDialogCloseBtn->resize(200,50);
    ExistDialogCloseBtn->move(250,330);

    connect(WifiActiveBtn,SIGNAL(clicked(bool)),this,SLOT(WifiActiveBtn_clicked()));
    connect(WifiExistRemoveBtn,SIGNAL(clicked(bool)),this,SLOT(WifiExistRemoveBtn_clicked()));
    connect(WifiModifyBtn,SIGNAL(clicked(bool)),this,SLOT(WifiModifyBtn_clicked()));
    connect(ExistDialogCloseBtn,SIGNAL(clicked(bool)),this,SLOT(ExistDialogCloseBtn_clicked()));
    connect(this,SIGNAL(wifi_activation_msg(QString)),WifiThread,SLOT(wifi_activation_thread(QString)));

}

void wifi::WifiActiveBtn_clicked()
{
    QString wifi_name = ui->WifiListWidget->currentItem()->text();
    qDebug() << "line:" << __LINE__ << "currentItem()->text = " << wifi_name;

//    LoadLabel->show();
//    pMovie->start();

    emit wifi_activation_msg(wifi_name);

}

void wifi::WifiExistRemoveBtn_clicked()
{
    QString wifi_name = ui->WifiListWidget->currentItem()->text();
    qDebug() << "FUNC:" << __FUNCTION__<< "line:" << __LINE__ << "currentItem()->text = " << wifi_name;

    QString strResult = wifi_bt_t->wifi_connection_remove(wifi_name);
    qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< strResult;

    bool downResult=strResult.contains("successfully deleted",Qt::CaseInsensitive);
    qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< downResult;

    if(downResult == 1)
    {
        QMessageBox::information(this,"information","remove succeeded!");
    }
    else
    {
        QMessageBox::critical(this,"information","remove failed!");
    }

}

void wifi::WifiModifyBtn_clicked()
{
    QString wifi_name = ui->WifiListWidget->currentItem()->text();
    qDebug() << "FUNC:" << __FUNCTION__<< "line:" << __LINE__ << "currentItem()->text = " << wifi_name;

    QString wifi_passwd;
    QString dlgTitle="input password dialog";
    QString txtLabel="please input password";
    QString defaultInput="password";
    QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
    //QLineEdit::EchoMode echoMode=QLineEdit::Password;//密码输入
    bool ok=false;
    QString text = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode,defaultInput, &ok);
    int length = text.length();
    if(length < 8)
    {
        QMessageBox::critical(this,"information","The number of password digits must be greater than 8");
        ok=false;
    }

    if (ok && !text.isEmpty())
    {
        wifi_passwd = text;
    }
    else
    {
        return;
    }
    qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< "wifi_passwd" << wifi_passwd;

    bool strResult = wifi_bt_t->wifi_modify(wifi_name,wifi_passwd);
    qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< strResult;

    if(strResult == 1)
    {
        QMessageBox::information(this,"information","change succeeded!");
    }
    else
    {
        QMessageBox::critical(this,"information","change failed!");
    }
}

void wifi::ExistDialogCloseBtn_clicked()
{
//    myThread->quit();
//    myThread->wait();
    WifiExistDialog->close();
}


void wifi::wifi_connect_dialog()
{
    WifiConnectDialog = new QDialog;
    WifiConnectDialog->resize(this->width()/2,this->height()/3*2);
    WifiConnectDialog->setStyleSheet("QDialog {border:2px solid gray;}");

    WifiNameLabel = new QLabel("wifiname",WifiConnectDialog);
    WifiNameLabel->move(50,80);
    WifiPasswdLabel = new QLabel("password",WifiConnectDialog);
    WifiPasswdLabel->move(50,150);

    WifiNameText = new QLineEdit(WifiConnectDialog);
    WifiNameText->resize(200,50);
    WifiNameText->move(200,80);
    WifiPasswdText = new QLineEdit(WifiConnectDialog);
    WifiPasswdText->resize(200,50);
    WifiPasswdText->move(200,150);

    WifiConnectBtn = new QPushButton("connect",WifiConnectDialog);
    WifiConnectBtn->resize(200,50);
    WifiConnectBtn->move(50,250);
    ConnectDialogCloseBtn = new QPushButton("close",WifiConnectDialog);
    ConnectDialogCloseBtn->resize(200,50);
    ConnectDialogCloseBtn->move(300,250);

    connect(ConnectDialogCloseBtn,SIGNAL(clicked(bool)),this,SLOT(ConnectDialogCloseBtn_clicked()));
    connect(WifiConnectBtn,SIGNAL(clicked(bool)),this,SLOT(DialogConnectBtn_clicked()));
    connect(this,SIGNAL(wifi_connect_msg(QString,QString)),WifiThread,SLOT(wifi_connect_thread(QString,QString)));
}

void wifi::ListWidgeItem_clicked()
{
    qDebug() << "line:" << __LINE__ << "istWidgeItem_clicked()";
    QString wifi_name = ui->WifiListWidget->currentItem()->text();
    qDebug() << "line:" << __LINE__ << "currentItem()->text = " << wifi_name;

    int flag = wifi_bt_t->wifi_connect_exist(wifi_name);
    if(flag == 1)
    {
        WifiExistDialog->show();
    }
    else
    {
        WifiNameText->setText(ui->WifiListWidget->currentItem()->text());
        WifiConnectDialog->show();
    }

}

void wifi::ConnectDialogCloseBtn_clicked()
{
//    myThread->quit();
//    myThread->wait();
    WifiConnectDialog->close();
}


void wifi::DialogConnectBtn_clicked()
{
    qDebug() << "--line--: " << __LINE__<< "FUNC:" << __FUNCTION__<< "---------------------";


    QString wifi_name =  WifiNameText->text();
    QString wifi_passwd =  WifiPasswdText->text();

    if(wifi_passwd == "")
    {
        //qDebug() << "please choose first ";
        QMessageBox::information(this,"information","please insert first!");
        return ;
    }

    emit wifi_connect_msg(wifi_name, wifi_passwd);
}

void wifi::on_ReturnBtn_clicked()
{
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
        strResult = QString("Please turn on the WiFi switch");
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
        strResult = QString("Please turn on the WiFi switch");
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
    qDebug() << "line:" << __LINE__ << "flag:" << flag;

    if(flag == 1) // open
    {
        LoadLabel->show();
        pMovie->start();
        emit wifi_scan_msg();
    }
    else
    {
        return;
    }
}

void wifi::on_HotspotBuildBtn_clicked()
{
    QString HtName = ui->HtName->text();
    QString HtPasswd = ui->HtPasswd->text();

    if(HtName == "" || HtPasswd == "")
    {
        QMessageBox::information(this,"information","Input cannot be empty!");
        return;
    }

    if(HtPasswd.length() < 8)
    {
        QMessageBox::information(this,"information","The number of password digits must be greater than 8");
        return ;
    }

    emit hotspot_build_msg(HtName,HtPasswd);
    //wifi_bt_t->hotspot_connect(HtName ,HtPasswd);
}

void wifi::on_HotspotDownBtn_clicked()
{
    QString strResult;

    strResult = wifi_bt_t->hotspot_disconnect();
    qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< "strResult" << strResult;

    if(strResult == QString(1))
    {
        QMessageBox::information(this,"information","successfully disconnected!");
    }
    else if(strResult == QString(""))
    {
        QMessageBox::critical(this,"information","disconnect failed!");
    }
    else
    {
        QMessageBox::information(this,"information",strResult);
    }
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
