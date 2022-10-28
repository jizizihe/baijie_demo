#include "wificondialog.h"
#include "ui_wificondialog.h"
#include <QScreen>

static int screenWidth;
static int screenHeight;
static int screenFlag;
static int connectFlag;
static QScreen *screen;
static qreal realX;
static qreal realY;

WifiConDialog::WifiConDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiConDialog)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;
        this->setStyleSheet("WifiConDialog{border-image: url(:/button_image/all/background.jpg);border-style: outset;"
                            "border-radius:4px}"
                            "QDialog {border:1px solid gray;}"
                            "QPushButton{background-color: rgba(100, 225, 100, 120);border-style: outset;"
                            "border-width:  2px; border-radius: 10px;border-color: rgba(255, 225, 255, 30);"
                            "color:rgba(0, 0, 0, 100); padding: 6px; outline: none;}"
                            "QSlider::handle:horizontal { image: url(:/image/sliderHandle.png);}"
                            "QSlider::sub-page:horizontal { border-image: url(:/image/slider.png); }");
    }
    else
    {
        this->setStyleSheet("#WifiConDialog{background-image: url(:/button_image/all/background.jpg);"
                            "border-style: outset;border-radius:4px}"
                            "QDialog {border:1px solid gray;}"
                            "QPushButton{background-color: rgba(100, 225, 100, 120);border-style: outset;"
                            "border-width:  2px;border-radius: 10px; border-color: rgba(255, 225, 255, 30);"
                            "color:rgba(0, 0, 0, 100);padding: 6px;outline: none;}"
                            "QPushButton:hover{background-color:rgba(100,255,100, 100); border-color: rgba(255, 225, 255, 200);color:rgba(0, 0, 0, 200);}"
                            " QPushButton:pressed {background-color:rgba(100,255,100, 200);border-color: rgba(255, 225, 255, 30);"
                            "border-style: inset;color:rgba(0, 0, 0, 100);}"
                            "QSlider::handle:horizontal { image: url(:/image/sliderHandle.png);}"
                            "QSlider::sub-page:horizontal { border-image: url(:/image/slider.png); } ");
    }
    realX = screen->physicalDotsPerInchX();
    realY = screen->physicalDotsPerInchY();
    wifidialFont();
    WifiLoadLabel = new QLabel(this);
    WifiMovie = new QMovie("://button_image/loading.webp");
    WifiLoadLabel->setFixedSize(50, 50);
    WifiLoadLabel->setScaledContents(true);
    WifiLoadLabel->setMovie(WifiMovie);
    if(screenFlag == 1)
    {
        WifiLoadLabel->move(this->size().height()/2,this->size().width()/2);
    }
    else
    {
        WifiLoadLabel->move(this->size().width()/2,this->size().height()/2);
    }
}

WifiConDialog::~WifiConDialog()
{
    delete ui;
    delete WifiLoadLabel;
    delete WifiMovie;
}

void WifiConDialog::wifi_wait_end_func()
{
    WifiMovie->stop();
    WifiLoadLabel->close();
}

void WifiConDialog::on_btn_ok_clicked()
{
    QString wifiName =  ui->NamelineEdit->text();
    QString wifiPasswd =  ui->PasswdlineEdit->text();

    int length = wifiPasswd.length();
    if(length < 8)
    {
        QMessageBox::information(this,"information",tr("The number of password digits must be greater than 8"));
        QMessageBox msge(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("The number of password digits must be greater than 8"),
                         0,this);
        msge.addButton(tr("OK"),QMessageBox::AcceptRole);
        return ;
    }
    WifiLoadLabel->show();
    WifiMovie->start();
    if(connectFlag == 0)  //wifi connect
    {
        emit wifi_connect_dialog_msg(wifiName, wifiPasswd);
    }
    else if(connectFlag == 1)  //wifi change password
    {
        emit wifi_modify_pass_msg(wifiName,wifiPasswd);
    }
}

void WifiConDialog::wifi_modify_pass(bool strResult)
{
    QString wifiName =  ui->NamelineEdit->text();
    QString wifiPasswd =  ui->PasswdlineEdit->text();
    WifiLoadLabel->close();
    WifiMovie->stop();
    if(strResult == true)
    {
        QMessageBox::information(this,"information",tr("change succeeded!"));
        databaseWg.updateWiFiTable("wifiPasswd",wifiName,wifiPasswd);
        emit wifi_show_refresh_msg();
        ui->NamelineEdit->clear();
        ui->PasswdlineEdit->clear();
        this->close();
    }
    else
    {
        WifiLoadLabel->close();
        WifiMovie->stop();
        databaseWg.updateWiFiTable("wifiPasswd",wifiName,wifiPasswd);
        QMessageBox::critical(this,"information",tr("connect failed,the password wrong!"));
        emit wifi_show_refresh_msg();
    }
}

void WifiConDialog::on_btn_cancel_clicked()
{
    ui->NamelineEdit->clear();
    ui->PasswdlineEdit->clear();
    this->close();
    emit wifidial_close_msg();
}

void WifiConDialog::setWifiNameText(QString wifiNanme)
{
    ui->NamelineEdit->setText(wifiNanme);
}

void WifiConDialog::setPasswdText(QString wifiPasswd)
{
    ui->PasswdlineEdit->setText(wifiPasswd);
}

QString WifiConDialog::getWifiNameText()
{
    return ui->NamelineEdit->text();
}

QString WifiConDialog::getPasswdText()
{
    return ui->PasswdlineEdit->text();
}

QString WifiConDialog::GetWifiOkBtnText()
{
    return (ui->btn_ok->text());
}

void WifiConDialog::setWifiOkBtnText(int flag)
{
    if(flag == 0)
    {
        ui->btn_ok->setText(tr("connect"));
    }
    else
    {
        ui->btn_ok->setText(tr("change"));
    }
    connectFlag = flag;
}

void WifiConDialog::languageReload()
{
    ui->retranslateUi(this);
}

void WifiConDialog::wifidialFont()
{
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

    ui->lbl_wifi->setFont(font);
    ui->lbl_wifiName->setFont(font);
    ui->lbl_wifiPasswd->setFont(font);
    ui->NamelineEdit->setFont(font);
    ui->PasswdlineEdit->setFont(font);
    ui->btn_cancel->setFont(font);
    ui->btn_ok->setFont(font);
}
