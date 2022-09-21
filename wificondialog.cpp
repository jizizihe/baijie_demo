#include "wificondialog.h"
#include "ui_wificondialog.h"
#include <QScreen>

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;
static qreal realX;
static qreal realY;
static int btn_connectflag;

WifiConDialog::WifiConDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiConDialog)
{
    ui->setupUi(this);
    wifi_bt_t = new wifi_bt_interface(this);
    screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();

    if(s_width < s_height)
    {
        screen_flag = 1;
    }
    realX = screen->physicalDotsPerInchX();
    realY = screen->physicalDotsPerInchY();
    wifidial_font();
    WifiLoadLabel = new QLabel(this);
    WifiLoadLabel->move(this->size().width()/2,this->size().height()/2);
    WifiMovie = new QMovie("://button_image/loading.webp");
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
    QString wifi_name =  ui->NamelineEdit->text();
    QString wifi_passwd =  ui->PasswdlineEdit->text();

    int length = wifi_passwd.length();
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
    if(btn_connectflag == 0)  //wifi connect
    {
        emit wifi_connect_dialog_signal(wifi_name, wifi_passwd);
    }
    else if(btn_connectflag == 1)  //wifi change password
    {
        emit wifi_modify_pass(wifi_name,wifi_passwd);
    }
}

void WifiConDialog::wifi_modifypass(bool strResult)
{
    QString wifi_name =  ui->NamelineEdit->text();
    QString wifi_passwd =  ui->PasswdlineEdit->text();
    WifiLoadLabel->close();
    WifiMovie->stop();
    if(strResult == true)
    {
        QMessageBox::information(this,"information",tr("change succeeded!"));
        database_w.update_wifitable("wifiPasswd",wifi_name,wifi_passwd);
        emit wifi_showrefresh();
        ui->NamelineEdit->clear();
        ui->PasswdlineEdit->clear();
        this->close();
    }
    else
    {
        WifiLoadLabel->close();
        WifiMovie->stop();
        database_w.update_wifitable("wifiPasswd",wifi_name,wifi_passwd);
        QMessageBox::critical(this,"information",tr("connect failed,the password wrong!"));
        emit wifi_showrefresh();
    }
}

void WifiConDialog::on_WifiCancelBtn_clicked()
{
    ui->NamelineEdit->clear();
    ui->PasswdlineEdit->clear();
    this->close();
    emit wifidial_close();
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

void WifiConDialog::SetWifiOkBtnText(int flag)
{
    if(flag == 0)
    {
        ui->WifiOkBtn->setText(tr("connect"));
    }
    else
    {
      ui->WifiOkBtn->setText(tr("ok"));
    }
    btn_connectflag = flag;
   // WifiLoadLabel->show();
   // WifiMovie->start();
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

void WifiConDialog::wifidial_font()
{
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

    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->NamelineEdit->setFont(font);
    ui->PasswdlineEdit->setFont(font);
    ui->WifiCancelBtn->setFont(font);
    ui->WifiOkBtn->setFont(font);
}
