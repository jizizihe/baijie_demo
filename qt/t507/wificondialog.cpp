#include "wificondialog.h"
#include "ui_wificondialog.h"

WifiConDialog::WifiConDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiConDialog)
{
    ui->setupUi(this);
    wifi_bt_t = new wifi_bt_interface(this);

}

WifiConDialog::~WifiConDialog()
{
    delete ui;
    delete wifi_bt_t;
}

void WifiConDialog::on_WifiOkBtn_clicked()
{
    QString WifiOkBtnText;
    QString wifi_name =  ui->NamelineEdit->text();
    QString wifi_passwd =  ui->PasswdlineEdit->text();

    int length = wifi_passwd.length();
    if(length < 8)
    {
        QMessageBox::information(this,"information",tr("The number of password digits must be greater than 8"));
        return ;
    }

    WifiOkBtnText = GetWifiOkBtnText();
    qDebug() << "LINE:" << __LINE__ << "__FUNC:"<<__FUNCTION__<< "WifiOkBtnText"<< WifiOkBtnText;

    if(WifiOkBtnText == "connect")  //wifi connect
    {
        emit wifi_connect_dialog_signal(wifi_name, wifi_passwd);
    }
    else if(WifiOkBtnText == "OK")  //wifi change password
    {
        bool strResult = wifi_bt_t->wifi_modify(wifi_name,wifi_passwd);
        //qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< strResult;

        if(strResult == true)
        {
            QMessageBox::information(this,"information",tr("change succeeded!"));
            ui->NamelineEdit->clear();
            ui->PasswdlineEdit->clear();
            this->close();
        }
        else
        {
            QMessageBox::critical(this,"information",tr("change failed!"));
        }

    }
}

void WifiConDialog::on_WifiCancelBtn_clicked()
{
    ui->NamelineEdit->clear();
    ui->PasswdlineEdit->clear();
    this->close();
}

void WifiConDialog::SetWifiNameText(QString wifinanme)
{
    ui->NamelineEdit->setText(wifinanme);
}

void WifiConDialog::SetPasswdText(QString wifipasswd)
{
    ui->PasswdlineEdit->setText(wifipasswd);
}

QString WifiConDialog::GetWifiOkBtnText()
{
    return (ui->WifiOkBtn->text());
}

void WifiConDialog::SetWifiOkBtnText(QString WifiOkBtnText)
{
    ui->WifiOkBtn->setText(WifiOkBtnText);
}


void WifiConDialog::language_reload()
{
    ui->retranslateUi(this);
}
