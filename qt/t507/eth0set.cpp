#include "eth0set.h"
#include "ui_eth0set.h"


extern "C"
{
    #include "speedtest.h"
}

bool is_virtual_network_card_or_loopback(QString str_card_name);

eth0set::eth0set(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::eth0set)
{
    ui->setupUi(this);
    connect(this,SIGNAL(StartTestSpeed()),this,SLOT(speedtest()));
    connect(&popup,SIGNAL(addipinfor(QString,QString,QString)),this,SLOT(increaseip(QString,QString,QString)));
    connect(&popup,SIGNAL(modipinfor(QString,QString,QString)),this,SLOT(modifyip(QString,QString,QString)));
    connect(&popup,SIGNAL(delipinfor(QString)),this,SLOT(deleteip(QString)));
}

eth0set::~eth0set()
{
    delete ui;
}

void eth0set::on_pushButton_clicked()
{
    emit Mysignal();
}

void eth0set::on_pushButton_8_clicked()
{
    QProcess cmd ;
    cmd.start("nmcli con show");
    cmd.waitForFinished();
    ui->textEdit->setText(QString::fromLocal8Bit(cmd.readAllStandardOutput().data()));


}

void eth0set::on_pushButton_2_clicked()
{
    QString cmd_addstaticip = "nmcli con add con-name static-ip ifname eth0 autoconnect yes type ethernet ip4 \
                        192.168.0.122 gw4 192.168.0.1";
    QProcess cmd_addsip;
    cmd_addsip.start(cmd_addstaticip);
    cmd_addsip.waitForFinished();
    cmd_addsip.start("nmcli con up static-ip");
    cmd_addsip.waitForFinished();
    ui->textEdit->setText(QString::fromLocal8Bit(cmd_addsip.readAllStandardOutput().data()));
}

void eth0set::on_pushButton_3_clicked() //set dynamic ip
{

    QString cmd_delstaticip = "udhcpc -i eth0";
    QProcess cmd_delsip;
    cmd_delsip.start(cmd_delstaticip);
    cmd_delsip.waitForFinished();
    ui->textEdit->setText(QString::fromLocal8Bit(cmd_delsip.readAllStandardOutput()));

}


void eth0set::on_pushButton_4_clicked() //add static ip
{
    popup.bnttype = "add ip";
    emit popup.addip();
    this->hide();
    popup.show();
}

void eth0set::on_pushButton_5_clicked() //mod static ip
{
    popup.bnttype = "mod ip";
    emit popup.modip();
    this->hide();
    popup.show();

}

void eth0set::on_pushButton_6_clicked() //del static ip
{
    popup.bnttype = "del ip";
    emit popup.delip();
    this->hide();
    popup.show();
}

void eth0set::on_pushButton_7_clicked()  //get current speed
{
    emit StartTestSpeed();
}

void eth0set::speedtest()
{
    long int rate,late_rate;
    QCoreApplication::processEvents();
    float speed;

    getCurrentDownloadRates(&rate);
    sleep(2);
    getCurrentDownloadRates(&late_rate);
    speed = (float)(late_rate - rate)/1;
    QString data = QString("Current speed is %1 Bytes/s").arg(speed);
    ui->textEdit->setText(data);
    qDebug("speed: %f\n",speed);
//    QTimer *timer = new QTimer(this);
//    timer->start(2000);
//    connect(timer, SIGNAL(timeout()),this,SLOT(speedtest()));
}

void eth0set::on_pushButton_9_clicked()
{
    QProcess cmd ;
    cmd.start("ifconfig");
    cmd.waitForFinished();
    ui->textEdit->setText(QString::fromLocal8Bit(cmd.readAllStandardOutput().data()));
}

void eth0set::increaseip(QString ip_name,QString ip_addr,QString ip_gateway)
{
    popup.hide();
    this->show();
    QString cmd_addstaticip = QString("nmcli con add con-name %1 ifname eth0 autoconnect yes type ethernet ip4 %2 gw4 %3") \
            .arg(ip_name).arg(ip_addr).arg(ip_gateway);
    QString cmd_ipup = QString("nmcli con up %1").arg(ip_name);

    qDebug() << cmd_addstaticip;
    QProcess cmd_addsip;
    cmd_addsip.start(cmd_addstaticip);
    cmd_addsip.waitForFinished();
    cmd_addsip.start(cmd_ipup);
    cmd_addsip.waitForFinished();
    ui->textEdit->setText(QString::fromLocal8Bit( cmd_addsip.readAllStandardOutput().data()));
}

void eth0set::modifyip(QString ip_name,QString ip_addr,QString ip_gateway)
{
    popup.hide();
    this->show();
    QString cmd_addstaticip = QString("nmcli con mod %1 ipv4.address %2,%3").arg(ip_name).arg(ip_addr).arg(ip_gateway);
    QString cmd_ipup = QString("nmcli con up %1").arg(ip_name);
    qDebug() << cmd_addstaticip;
    QProcess cmd_addsip;
    cmd_addsip.start(cmd_addstaticip);
    cmd_addsip.waitForFinished();
    cmd_addsip.start(cmd_ipup);
    cmd_addsip.waitForFinished();
    ui->textEdit->setText(QString::fromLocal8Bit( cmd_addsip.readAllStandardOutput().data()));
}

void eth0set::deleteip(QString ip_name)
{
    popup.hide();
    this->show();
    QString cmd_addstaticip = QString("nmcli con del %1").arg(ip_name);
    qDebug() << cmd_addstaticip;
    QProcess cmd_addsip;
    cmd_addsip.start(cmd_addstaticip);
    cmd_addsip.waitForFinished();
    ui->textEdit->setText(QString::fromLocal8Bit( cmd_addsip.readAllStandardOutput().data()));
}
