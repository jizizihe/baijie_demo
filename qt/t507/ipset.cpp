#include "ipset.h"
#include "ui_ipset.h"
#include "ipset_interface.h"


//bool is_virtual_network_card_or_loopback(QString str_card_name);

ipset::ipset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ipset)
{
    ui->setupUi(this);

    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:25px;}");

    connect(&popup,SIGNAL(go_back()),this,SLOT(gobackmenu()));
    connect(&popup,SIGNAL(addipinfor(QString,QString)),this,SLOT(increaseip(QString,QString)));
    connect(&popup,SIGNAL(modipinfor(QString)),this,SLOT(modifyip(QString)));
}

ipset::~ipset()
{
    delete ui;
}

void ipset::on_retbtn_clicked()  //return main menu
{
    emit Mysignal();
    ui->textEdit->setText("");
}

void ipset::on_delstaticip_clicked() //del static ip
{
    QString str = delstaticip();

    if(str == "")
    {
        ui->textEdit->setText(tr("It's not static ip yet,please press this button after setting the static IP."));
    }
    else
    {
        ui->textEdit->setText(str);
    }
}


void ipset::on_setstaticip_clicked() //set static ip
{
    popup.bnttype = "add ip";
    emit popup.addip();
    this->hide();
    popup.show();
    ui->textEdit->setText("");
}

void ipset::on_modstaticip_clicked() //mod static ip
{
    if(is_staticip() == "")
    {
        ui->textEdit->setText(tr("It's not static ip yet,please press this button after setting the static IP."));
    }
    else
    {
        popup.bnttype = "mod ip";
        emit popup.modip();
        this->hide();
        popup.show();
        ui->textEdit->setText("");
    }
}


void ipset::on_ipshowbtn_clicked()  //ifconfig
{
    QCoreApplication::processEvents();
    ui->textEdit->setText(getifconfig());
}

void ipset::on_networkupbtn_clicked()  //pull up network
{
    QCoreApplication::processEvents();
    if(GetCurrentIp("eth0") != "")
    {
        ui->textEdit->setText(tr("network is aready pull up!"));
    }
    else
    {
        ui->textEdit->setText(networkup());
    }
}


void ipset::increaseip(QString net_card,QString ip_addr)
{
    popup.hide();
    this->show();
    ui->textEdit->setText(addstaticip(net_card, ip_addr));
}

void ipset::modifyip(QString ip_addr)
{
    popup.hide();
    this->show();
    ui->textEdit->setText(modstaticip(ip_addr));
}

void ipset::gobackmenu()
{
    popup.hide();
    this->show();
}


void ipset::language_reload()
{
    ui->retranslateUi(this);
    popup.language_reload();
}



