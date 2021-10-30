#include "ipset.h"
#include "ui_ipset.h"
#include "ipset_interface.h"


bool is_virtual_network_card_or_loopback(QString str_card_name);

ipset::ipset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ipset)
{
    ui->setupUi(this);

    timer = new QTimer();

    ui->pushButton_7->installEventFilter(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(speedtest()));

    connect(this,SIGNAL(StartTestSpeed()),this,SLOT(speedtest()));
    connect(&popup,SIGNAL(go_back()),this,SLOT(gobackmenu()));
    connect(&popup,SIGNAL(setdyninfor(QString)),this,SLOT(setdynamicip(QString)));
    connect(&popup,SIGNAL(addipinfor(QString,QString,QString)),this,SLOT(increaseip(QString,QString,QString)));
    connect(&popup,SIGNAL(modipinfor(QString,QString)),this,SLOT(modifyip(QString,QString)));
    connect(&popup,SIGNAL(delipinfor(QString)),this,SLOT(deleteip(QString)));
    connect(&popup,SIGNAL(go_back()),this,SLOT(gobackmenu()));
}

ipset::~ipset()
{
    delete ui;
}

void ipset::on_pushButton_clicked()
{
    emit Mysignal();
    ui->textEdit->setText("");
}

void ipset::on_pushButton_8_clicked()
{
    ui->textEdit->setText(getallip());
}

void ipset::on_pushButton_3_clicked() //set dynamic ip
{
    popup.bnttype = "set dynamic ip";
    emit popup.setdip();
    this->hide();
    popup.show();
    ui->textEdit->setText("");
}


void ipset::on_pushButton_4_clicked() //add static ip
{
    popup.bnttype = "add ip";
    emit popup.addip();
    this->hide();
    popup.show();
    ui->textEdit->setText("");
}

void ipset::on_pushButton_5_clicked() //mod static ip
{
    popup.bnttype = "mod ip";
    emit popup.modip();
    this->hide();
    popup.show();
    ui->textEdit->setText("");
}

void ipset::on_pushButton_6_clicked() //del static ip
{
    popup.bnttype = "del ip";
    emit popup.delip();
    this->hide();
    popup.show();
    ui->textEdit->setText("");
}

void ipset::on_pushButton_7_clicked()  //get current speed
{
    emit StartTestSpeed();

}

void ipset::speedtest() //speedtest
{
    QCoreApplication::processEvents();
    double speed;
    speed = getspeed();
    if(speed > 1048576.0)
    {
        speed = speed/1048576.0;
        QString str= QString::number(speed,'f',1);
        ui->textEdit->setText(QString(tr("Current speed is %1 MBytes/s")).arg(str));
        return;
    }
    if(speed > 1024.0)
    {

        speed = speed/1024.0;
        QString str= QString::number(speed,'f',1);
        ui->textEdit->setText(QString(tr("Current speed is %1 KBytes/s")).arg(str));
        return;
    }
    QString str= QString::number(speed,'f',1);
    ui->textEdit->setText(QString(tr("Current speed is %1 Bytes/s")).arg(str));


}

void ipset::on_pushButton_9_clicked()  //ifconfig
{
    ui->textEdit->setText(getipconfig());
}

void ipset::setdynamicip(QString net_card)
{
    popup.hide();
    this->show();
    ui->textEdit->setText(setdip(net_card));
}

void ipset::increaseip(QString ip_name,QString net_card,QString ip_addr)
{
    popup.hide();
    this->show();
    ui->textEdit->setText(addstaticip(ip_name, net_card, ip_addr));
}

void ipset::modifyip(QString ip_name,QString ip_addr)
{
    popup.hide();
    this->show();
    ui->textEdit->setText(modstaticip(ip_name, ip_addr));
}

void ipset::deleteip(QString ip_name)
{
    popup.hide();
    this->show();
    ui->textEdit->setText(delstaticip(ip_name));
}

void ipset::gobackmenu()
{
    popup.hide();
    this->show();
}

bool ipset::eventFilter(QObject *watched, QEvent *event)
{
     if (watched==ui->pushButton_7)
     {
          if (event->type()==QEvent::FocusIn)
          {
            timer->start(1100);
          }
          else if (event->type()==QEvent::FocusOut)
          {
            timer->stop();
          }
     }

    return QWidget::eventFilter(watched,event);
}


void ipset::language_reload()
{
    ui->retranslateUi(this);
    popup.language_reload();
}
