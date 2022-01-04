#include "wificondialog.h"
#include "ui_wificondialog.h"

WifiConDialog::WifiConDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiConDialog)
{
    ui->setupUi(this);
    wifi_bt_t = new wifi_bt_interface(this);

    WifiLoadLabel = new QLabel(this);
    WifiLoadLabel->move(this->size().width()/2,this->size().height()/2);
    WifiMovie = new QMovie("://t507_button_image/loading.webp");
    WifiLoadLabel->setFixedSize(50, 50);
    WifiLoadLabel->setScaledContents(true);
    WifiLoadLabel->setMovie(WifiMovie);
    //pMovie->start();

}

WifiConDialog::~WifiConDialog()
{
    delete ui;

    delete WifiLoadLabel;
    delete WifiMovie;
    delete wifi_bt_t;
}

void WifiConDialog::wifi_wait_end_func()
{
    WifiMovie->stop();
    WifiLoadLabel->close();
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
        WifiLoadLabel->show();
        WifiMovie->start();

        emit wifi_connect_dialog_signal(wifi_name, wifi_passwd);
    }
    else if(WifiOkBtnText == "OK")  //wifi change password
    {
        bool strResult = wifi_bt_t->wifi_modify(wifi_name,wifi_passwd);
        //qDebug() << "FUNC:" << __FUNCTION__<< "--LINE--: " << __LINE__<< strResult;

        if(strResult == true)
        {
            QMessageBox::information(this,"information",tr("change succeeded!"));
            wifi_bt_t->wifi_passwd_change(wifi_name,wifi_passwd);

            emit wifi_info_fresh_msg(wifi_name);
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

QString WifiConDialog::GetWifiNameText()
{
    return ui->NamelineEdit->text();
}

QString WifiConDialog::GetPasswdText()
{
    return ui->PasswdlineEdit->text();
}


QString WifiConDialog::GetWifiOkBtnText()
{
    return (ui->WifiOkBtn->text());
}

void WifiConDialog::SetWifiOkBtnText(QString WifiOkBtnText)
{
    ui->WifiOkBtn->setText(WifiOkBtnText);
}


bool WifiConDialog::event(QEvent *event)
{
    /*
    QWidget *currentItem;
    currentItem = QApplication::focusWidget();
    qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "currentItem:" <<currentItem;

    if(event->type()==QEvent::MouseButtonPress)
    {
        currentItem = QApplication::focusWidget();
        qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "currentItem:" <<currentItem;

        this->setFocusPolicy(Qt::NoFocus);
        qDebug() << tr("MyLineEdit event functions");

        currentItem = QApplication::focusWidget();
        qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "currentItem:" <<currentItem;
    }
    */
    return QWidget::event(event);

}


void WifiConDialog::language_reload()
{
    ui->retranslateUi(this);

}
