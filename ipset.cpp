#include "ipset.h"
#include "ui_ipset.h"
#include <QScreen>

static int g_screenWidth;
static int g_screenHeight;
static int g_openFlag = 1;
static int g_btnUpFlag;
static QScreen *g_screen;
static QLabel *g_swicthLabel;
static QHBoxLayout *g_horLayout;

ipset::ipset(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ipset)
{
    ui->setupUi(this);
    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:25px;}");
    ui->stackedWidget->setCurrentIndex(0);
    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();

    QRegExp a("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");  //ip regular expression
    ui->ipAddrLineEdit->setValidator(new QRegExpValidator(a,this));
    ui->gatewaylineEdit->setValidator(new QRegExpValidator(a,this));
    ui->nameLineEdit->setReadOnly(true);
    ui->masklineEdit->setPlaceholderText("1-32");

    QString networkInfo = getNetworkInfo();
    ui->textEdit->setText(networkInfo);
    ui->stackedWidget->setCurrentIndex(0);

    ui->widget->setEnabled(false);
    ui->widget->setStyleSheet("border-radius: 12px;padding: 6px;"
                              "background-color: rgb(217, 217, 217,30);border:1px solid gray;");
    ui->btn_ok->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                              "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                              "padding: 6px;outline: none; ");
    ui->btn_clear->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                                 "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                                 "padding: 6px;outline: none; ");
    ui->nameLineEdit->setStyleSheet("color: rgb(186, 189, 182);");
    ui->masklineEdit->setStyleSheet("color: rgb(186, 189, 182);");
    ui->ip_Switch->setCheckedColor(QColor(100, 225, 100, 150));
    setIpFont();
    setIpText();
    setSwitchText();
    connect(ui->ip_Switch,SIGNAL(toggled(bool)),this,SLOT(switch_change_flag(bool)));
}

ipset::~ipset()
{
    delete ui;
    delete g_swicthLabel;
    delete g_horLayout;
}

void ipset::languageReload()
{
    ui->retranslateUi(this);
    int flag = ui->ip_Switch->isToggled();
    if(flag == 1)
    ui->ip_Switch->setToggle(true);
    else
    ui->ip_Switch->setToggle(false);
}

void ipset::on_btn_ret_clicked()
{
    emit ipset_back_msg();
    ui->textEdit->setText("");
}

void ipset::on_btn_ipShow_clicked()
{
    if(g_openFlag == 0)
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the ethernet!"),
                         0,this);
        mesg.addButton(tr("ok"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        return;
    }

    QString networkInfo = getNetworkInfo();
    ui->textEdit->setText(networkInfo);
    ui->stackedWidget->setCurrentIndex(0);
}

void ipset::on_btn_setIp_clicked()
{
    if(g_openFlag == 0)
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the ethernet!"),
                         0,this);
        mesg.addButton(tr("ok"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        return;
    }
    ui->stackedWidget->setCurrentIndex(1);
}

void ipset::on_btn_clear_clicked()
{
    ui->ipAddrLineEdit->clear();
    ui->gatewaylineEdit->clear();
    ui->masklineEdit->clear();
}

void ipset::on_btn_setAutoIp_clicked()
{
    if(g_openFlag == 0)
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the ethernet!"),
                         0,this);
        mesg.addButton(tr("ok"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        return;
    }
    ui->widget->setEnabled(false);
    ui->widget->setStyleSheet("border-radius: 12px;padding: 6px;"
                              "background-color: rgb(217, 217, 217,30);border:1px solid gray;");
    ui->btn_ok->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                              "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                              "padding: 6px;outline: none; ");
    ui->btn_clear->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                                 "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                                 "padding: 6px;outline: none; ");
    ui->nameLineEdit->setStyleSheet("color: rgb(186, 189, 182);");
    ui->masklineEdit->setStyleSheet("color: rgb(186, 189, 182);");

    if(false == isStaticIpExist())
    {
        QString strCmd = QString("nmcli con show --active |grep eth0");
        QString strResult = executeLinuxCmd(strCmd);
        if(strResult.isEmpty())
        {
            strCmd = QString("ifconfig eth0 up");
            executeLinuxCmd(strCmd);
            strCmd = QString("nmcli con show --active |grep eth0");
            strResult = executeLinuxCmd(strCmd);
            if(!strResult.isEmpty())
            {
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr("set auto ip succeeded!"),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                mesg.move(g_screenWidth/3,g_screenHeight/3);
                mesg.exec();
                QString networkInfo = getNetworkInfo();
                ui->textEdit->setText(networkInfo);
                ui->stackedWidget->setCurrentIndex(0);
                ui->ipAddrLineEdit->clear();
                ui->gatewaylineEdit->clear();
                ui->masklineEdit->clear();
                ui->btn_ok->setText(tr("up"));
                g_btnUpFlag = 0;
                networkEnable(true);
                return;
            }
            else
            {
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr("set auto ip failed"),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                mesg.move(g_screenWidth/3,g_screenHeight/3);
                mesg.exec();
            }
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("The current connection is auto IP!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();

            QString networkInfo = getNetworkInfo();
            ui->textEdit->setText(networkInfo);
            ui->stackedWidget->setCurrentIndex(0);
            networkEnable(true);
            return;
        }
        return;
    }

    bool result = deleteStaticIp();          // delete static ip
    if(result == true)
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("set auto ip succeeded!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        QString networkInfo = getNetworkInfo();
        ui->textEdit->setText(networkInfo);
        ui->stackedWidget->setCurrentIndex(0);
        ui->ipAddrLineEdit->clear();
        ui->gatewaylineEdit->clear();
        ui->masklineEdit->clear();
        ui->btn_ok->setText(tr("up"));
        g_btnUpFlag = 0;
        networkEnable(true);
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("set auto ip failed"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
    }
}

void ipset::on_btn_ok_clicked()
{
    bool result = false;
    QString networkInfo;
    if(ui->nameLineEdit->text().isEmpty() || ui->ipAddrLineEdit->text().isEmpty() || ui->gatewaylineEdit->text().isEmpty() || ui->masklineEdit->text().isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please complete the information"),
                         0,this);
        mesg.addButton(tr("up"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        return;
    }

    QMessageBox mesg(QMessageBox::Question,
                     tr("QMessageBox::question()"),
                     tr("Do you want to set it to current IP?"),
                     0,this);
    QPushButton *yesButton = mesg.addButton(tr("Yes"), QMessageBox::ActionRole);
    QPushButton *noButton = mesg.addButton(tr("No"),QMessageBox::ActionRole);
    mesg.move(g_screenWidth/3,g_screenHeight/3);
    mesg.exec();

    if(mesg.clickedButton() == yesButton)
    {
        if((ui->ipAddrLineEdit->text().length() <= 9) || (ui->gatewaylineEdit->text().length() <= 9))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Format error, please re-enter."),
                             0,this);
            mesg.addButton(tr("up"),QMessageBox::YesRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
            return;
        }
        int mask = ui->masklineEdit->text().toInt();
        if((mask > 32)||(mask < 1))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Please enter the correct subnet mask."),
                             0,this);
            mesg.addButton(tr("up"),QMessageBox::YesRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
            return;
        }
        if((ui->ipAddrLineEdit->text().length() <= 9) || (ui->gatewaylineEdit->text().length() <= 9))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Format error, please re-enter."),
                             0,this);
            mesg.addButton(tr("up"),QMessageBox::YesRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
            return;
        }

        result = addStaticIp(ui->nameLineEdit->text(),ui->ipAddrLineEdit->text(),ui->masklineEdit
                             ->text(),ui->gatewaylineEdit->text());
        if(result == true)
        {
            networkInfo = getNetworkInfo();
            ui->textEdit->setText(networkInfo);
            ui->stackedWidget->setCurrentIndex(0);
            if(g_btnUpFlag == 0)
            {
                g_btnUpFlag = 1;
                ui->btn_ok->setText(tr("change"));
            }
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("set static ip failed"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
        }
    }
    else if (mesg.clickedButton() == noButton)
    {
        return;
    }
}

void ipset::setIpFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
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
    ui->btn_setAutoIp->setFont(font);
    ui->btn_ipShow->setFont(font);
    ui->btn_clear->setFont(font);
    ui->btn_setIp->setFont(font);
    ui->nameLineEdit->setFont(font);
    ui->ipAddrLineEdit->setFont(font);
    ui->lbl_gatway->setFont(font);
    ui->lbl_subnetMask->setFont(font);
    ui->textEdit->setFont(font);
    ui->btn_ok->setFont(font);
    ui->lbl_ethernet->setFont(font);
    ui->lbl_ipAddress->setFont(font);
    ui->lbl_ipName->setFont(font);
    ui->gatewaylineEdit->setFont(font);
    ui->masklineEdit->setFont(font);
    ui->btn_setStaticIp->setFont(font);
}

void ipset::setIpText()
{
    if(true == isStaticIpExist())
    {
        QString strCmd = QString("ip route show");
        QString strResult = executeLinuxCmd(strCmd);
        strResult = strResult.section(" ",2,2);
        ui->gatewaylineEdit->setText(strResult);
        strCmd = QString("ifconfig eth0 | awk '{print $2}'|awk 'NR==2'");
        strResult = executeLinuxCmd(strCmd);
        ui->ipAddrLineEdit->setText(strResult);
        ui->widget->setEnabled(true);
        ui->widget->setStyleSheet("border-radius: 12px;padding: 6px;background-color:rgba(255, 255, 255, 80);"
                                  "border:1px solid gray;");
        ui->btn_ok->setStyleSheet("background-color: rgba(100, 225, 100, 120);border-style: outset;border-width:  2px;"
                                  "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(0, 0, 0, 100);"
                                  "padding: 6px;outline: none; ");
        ui->btn_clear->setStyleSheet("background-color: rgba(100, 225, 100, 120);border-style: outset;border-width:  2px;"
                                     "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(0, 0, 0, 100);"
                                     "padding: 6px;outline: none; ");
        ui->nameLineEdit->setStyleSheet("color: rgb(0, 0, 0);");
        ui->masklineEdit->setStyleSheet("color: rgb(0, 0, 0);");
        ui->btn_ok->setText(tr("change"));
    }
}

void ipset::on_btn_setStaticIp_clicked()
{
    ui->widget->setEnabled(true);
    ui->widget->setStyleSheet("border-radius: 12px;padding: 6px;background-color:rgba(255, 255, 255, 60);"
                              "border:1px solid gray;");
    ui->btn_ok->setStyleSheet("background-color: rgba(100, 225, 100, 120);border-style: outset;border-width:  2px;"
                              "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(0, 0, 0, 100);"
                              "padding: 6px;outline: none; ");
    ui->btn_clear->setStyleSheet("background-color: rgba(100, 225, 100, 120);border-style: outset;border-width:  2px;"
                                 "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(0, 0, 0, 100);"
                                 "padding: 6px;outline: none; ");
    ui->nameLineEdit->setStyleSheet("color: rgb(0, 0, 0);");
    ui->masklineEdit->setStyleSheet("color: rgb(0, 0, 0);");
}

void ipset::switch_change_flag(bool flag)
{
    flag = ui->ip_Switch->isToggled();
    if(flag == 1)
    {
        g_openFlag = 1;
        networkEnable(true);
        QString networkInfo = getNetworkInfo();
        ui->textEdit->setText(networkInfo);
        ui->stackedWidget->setCurrentIndex(0);
        g_swicthLabel->setText(tr("  off"));
        g_swicthLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        g_swicthLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    }
    else
    {
        g_openFlag = 0;
        networkEnable(false);
        ui->textEdit->setText("");
        g_swicthLabel->setText(tr("on   "));
        g_swicthLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        g_swicthLabel->setStyleSheet("color: rgba(255,255,255,200)");
    }
}

void ipset::setSwitchText()
{
    g_swicthLabel = new QLabel(ui->ip_Switch);
    g_horLayout = new QHBoxLayout();
    g_swicthLabel->setText(tr("  off"));
    g_horLayout->addWidget(g_swicthLabel);
    g_swicthLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    g_swicthLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    ui->ip_Switch->setLayout(g_horLayout);
}
