#include "ipset.h"
#include "ui_ipset.h"

ipset::ipset(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ipset)
{
    ui->setupUi(this);
    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:25px;}");

    QRegExp a("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    ui->ipAddrLineEdit->setValidator(new QRegExpValidator(a,this));

    ui->networkSwitch->setCheckedColor(QColor(100, 225, 100));
    connect(ui->networkSwitch,SIGNAL(toggled(bool)),this,SLOT(btnChangeFlag(bool)));

    QString strCmd = QString("ifconfig | grep eth0");
    QString retResult = executeLinuxCmd(strCmd);
    if(false == retResult.isEmpty())
    {
        ui->networkSwitch->setToggle(true);
    }
    else
    {
        ui->networkSwitch->setToggle(false);
    }
}

ipset::~ipset()
{
    delete ui;
}

void ipset::btnChangeFlag(bool flag)
{
    //qDebug() << "LINE:" << __LINE__<<"switchflag " << switchflag;
    flag = ui->networkSwitch->isToggled();

    if(flag == 1) // open
    {
        network_enable(true);
        ui->ipShowBtn->setEnabled(true);
        ui->autoGetIpBtn->setEnabled(true);
        ui->setStaticIpBtn->setEnabled(true);
        ui->modStaticIpBtn->setEnabled(true);
        ui->delStaticIpBtn->setEnabled(true);
    }
    else //close
    {
        network_enable(false);
        ui->ipShowBtn->setEnabled(false);
        ui->autoGetIpBtn->setEnabled(false);
        ui->setStaticIpBtn->setEnabled(false);
        ui->modStaticIpBtn->setEnabled(false);
        ui->delStaticIpBtn->setEnabled(false);
    }
}

void ipset::language_reload()
{
    ui->retranslateUi(this);
}

void ipset::on_retBtn_clicked()
{
    emit ret_signal();
    ui->textEdit->setText("");
}

void ipset::on_ipShowBtn_clicked()
{
    QString networkInfo = get_network_info();
    ui->textEdit->setText(networkInfo);
    ui->stackedWidget->setCurrentIndex(0);
}

void ipset::on_autoGetIpBtn_clicked()
{
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << ":::::::::::::";

    bool result;
    ui->ipAddrLineEdit->clear();
    QCoreApplication::processEvents();
    if(get_current_ip("eth0") != "")
    {
        ui->textEdit->setText(tr("network is aready pull up!"));
    }
    else
    {
        result = automatically_get_ip();
        if(result == true)
        {
            qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "result:"<<result;
            QMessageBox::information(this,"information",tr("succeeded!"));
        }
        {
            QMessageBox::warning(this,"information",tr("failed!"));
        }
    }
    ui->stackedWidget->setCurrentIndex(0);
}

void ipset::on_setStaticIpBtn_clicked()
{
    ui->okBtn->setText(tr("ok"));
    ui->stackedWidget->setCurrentIndex(1);
}

void ipset::on_modStaticIpBtn_clicked()
{
    ui->ipAddrLineEdit->clear();

    if(false == is_static_ip_exist())
    {
        QMessageBox::information(this,"information",tr(" The current connection is not a static IP. Please modify it to a static IP before performing this operation"));
        QString networkInfo = get_network_info();
        ui->textEdit->setText(networkInfo);
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui->textEdit->setText("");
        ui->okBtn->setText(tr("change"));
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void ipset::on_delStaticIpBtn_clicked()
{
    ui->ipAddrLineEdit->clear();

    if(false == is_static_ip_exist())
    {
        QMessageBox::information(this,"information",tr(" The current connection is not a static IP. Please modify it to a static IP before performing this operation"));
        QString networkInfo = get_network_info();
        ui->textEdit->setText(networkInfo);
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }
    bool result = delete_static_ip();
    if(result == true)
    {
        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "result:"<<result;
        QMessageBox::information(this,"information",tr("delete static ip succeeded"));
    }
    else
    {
        QMessageBox::warning(this,"information",tr("delete static ip failed"));
    }
}

void ipset::on_okBtn_clicked()
{
    QMessageBox::StandardButton reply;
    bool result = false;
    QString networkInfo;
    if(ui->nameLineEdit->text().isEmpty() || ui->ipAddrLineEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL,NULL,tr("Please complete the information"),tr("OK"));
        return;
    }

    reply = QMessageBox::question(this,tr("QMessageBox::question()"),tr("Do you want to set it to current IP?"),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(reply == QMessageBox::No)
    {
        return;
    }
    else if(reply == QMessageBox::Yes)
    {
        if(ui->ipAddrLineEdit->text().length() <= 9)
        {
            QMessageBox::information(this,"information",tr("Format error, please re-enter"));
            return;
        }

        if(ui->okBtn->text() == QString(tr("ok")))
        {
//            qDebug() << "LINE:"<< __LINE__ << "ret:" << ret;
            result = add_static_ip(ui->nameLineEdit->text(),ui->ipAddrLineEdit->text());
        }
        else if(ui->okBtn->text() == QString(tr("change")))
        {
//            qDebug() << "LINE:"<< __LINE__ << "ret:" << ret;
            result = modify_static_ip(ui->ipAddrLineEdit->text());
        }

        if(result == true)
        {
            qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "result:"<<result;

            networkInfo = get_network_info();
            ui->textEdit->setText(networkInfo);
            ui->stackedWidget->setCurrentIndex(0);
        }
        else
        {
            QMessageBox::warning(this,"information",tr("set static ip failed"));
        }
    }
}

void ipset::on_backBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
