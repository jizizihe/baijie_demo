#include "wifi_condialog.h"
#include "ui_wificondialog.h"
#include <QScreen>

static int g_screenWidth;
static int g_screenHeight;
static int g_connectFlag;
static QScreen *g_screen;
static qreal g_realX;
static qreal g_realY;

WifiConDialog::WifiConDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiConDialog)
{
    ui->setupUi(this);
    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();
    g_realX = g_screen->physicalDotsPerInchX();
    g_realY = g_screen->physicalDotsPerInchY();
    setWifiDialogFont();
    g_wifiLoadLabel = new QLabel(this);
    g_wifiMovie = new QMovie("://button_image/loading.webp");
    g_wifiLoadLabel->setFixedSize(50, 50);
    g_wifiLoadLabel->setScaledContents(true);
    g_wifiLoadLabel->setMovie(g_wifiMovie);
    ui->btn_cancel->setText(tr("cancel"));
    g_wifiLoadLabel->move(this->size().width()/2,this->size().height()/2);
}

WifiConDialog::~WifiConDialog()
{
    delete ui;
    delete g_wifiLoadLabel;
    delete g_wifiMovie;
}

void WifiConDialog::wifi_wait_end_func()
{
    g_wifiMovie->stop();
    g_wifiLoadLabel->close();
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
    g_wifiLoadLabel->show();
    g_wifiMovie->start();
    if(g_connectFlag == 0)  //wifi connect
    {
        emit wifi_connect_dialog_msg(wifiName, wifiPasswd);
    }
    else if(g_connectFlag == 1)  //wifi change password
    {
        emit wifi_modify_pass_msg(wifiName,wifiPasswd);
    }
}

void WifiConDialog::wifi_modify_pass(bool strResult)
{
    QString wifiName =  ui->NamelineEdit->text();
    QString wifiPasswd =  ui->PasswdlineEdit->text();
    g_wifiLoadLabel->close();
    g_wifiMovie->stop();
    if(strResult == true)
    {
        QMessageBox::information(this,"information",tr("change succeeded!"));
        g_database.updateWiFiTable("wifiPasswd",wifiName,wifiPasswd);
        emit wifi_status_show_msg();
        ui->NamelineEdit->clear();
        ui->PasswdlineEdit->clear();
        this->close();
    }
    else
    {
        g_wifiLoadLabel->close();
        g_wifiMovie->stop();
        g_database.updateWiFiTable("wifiPasswd",wifiName,wifiPasswd);
        QMessageBox::critical(this,"information",tr("connect failed,the password wrong!"));
        emit wifi_status_show_msg();
    }
}

void WifiConDialog::on_btn_cancel_clicked()
{
    ui->NamelineEdit->clear();
    ui->PasswdlineEdit->clear();
    this->close();
    emit wifi_dialog_close_msg();
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

QString WifiConDialog::getWifiOkBtnText()
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
    g_connectFlag = flag;
}

void WifiConDialog::languageReload()
{
    ui->retranslateUi(this);
}

void WifiConDialog::setWifiDialogFont()
{
    qreal realWidth = g_screenWidth / g_realX * 2.54;
    QFont font;

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

    ui->lbl_wifi->setFont(font);
    ui->lbl_wifiName->setFont(font);
    ui->lbl_wifiPasswd->setFont(font);
    ui->NamelineEdit->setFont(font);
    ui->PasswdlineEdit->setFont(font);
    ui->btn_cancel->setFont(font);
    ui->btn_ok->setFont(font);
}
