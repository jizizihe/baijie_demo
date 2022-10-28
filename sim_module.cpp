#include "sim_module.h"
#include "ui_sim_module.h"
#include <QScreen>

static QScreen *screen;
static int screenWidth;
static int screenHeight;
static int screenFlag;
static int openFlag;
static QLabel *openLabel;
static QHBoxLayout *horLayout;

sim_module::sim_module(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sim_module)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line_2->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    openFlag = 1;
    simFont();
    timer = new QTimer(this);
    loadLabel = new QLabel(this);
    pMovie = new QMovie("://button_image/loading.webp");
    loadLabel->setFixedSize(50, 50);
    loadLabel->setScaledContents(true);
    loadLabel->setMovie(pMovie);
    if(screenFlag == 1)
    {
        loadLabel->move(screenHeight/2,screenWidth/2);
    }
    else
    {
        loadLabel->move(screenWidth/2,screenHeight/2 );
    }
    wifi_bt_interfaceWg = new wifi_bt_interface(this);
    myThread = new QThread(this);
    SimThread = new wifi_thread();
    ui->sim_Switch->setCheckedColor(QColor(100, 225, 100, 150));
    switchSetText();

    connect(ui->sim_Switch,SIGNAL(toggled(bool)),this,SLOT(switch_change_flag(bool)));
    connect(timer,SIGNAL(timeout()),this,SLOT(module_status_refresh()));
    connect(this,SIGNAL(sim_disconnect_msg()),SimThread,SLOT(sim_disconnect_thread()));
    connect(this,SIGNAL(sim_connect_msg()),SimThread,SLOT(sim_connect_thread()));
    connect(this,SIGNAL(sim_status_msg()),SimThread,SLOT(sim_status_thread()));
    connect(this,SIGNAL(sim_module_status_msg()),SimThread,SLOT(sim_module_status()));
    connect(SimThread,SIGNAL(send_sim_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    connect(this,SIGNAL(sim_activation_msg(int)),SimThread,SLOT(sim_activation(int)));

    SimThread->moveToThread(myThread);
    myThread->start();
    emit sim_module_status_msg();
    emit sim_status_msg();
    ui->stackedWidget->setCurrentIndex(2);
}

sim_module::~sim_module()
{
    delete ui;
    delete wifi_bt_interfaceWg;
    delete openLabel;
    delete horLayout;
}

void sim_module::showEvent(QShowEvent *event)
{
    timer->start(3000);
}

void sim_module::on_btn_ret_clicked()
{
    timer->stop();
    emit sim_back_msg();
}

void sim_module::recv_msg(int signalType, QString strResult)
{
    QStringList list;
    switch (signalType)
    {
    case simDisconnectSignal:
        pMovie->stop();
        loadLabel->hide();
        if(strResult == QString(1))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G disconnect succeeded!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenHeight/3,screenWidth*2/3);
            else
                mesg.move(screenWidth,screenHeight/3);
            mesg.exec();
            ui->stackedWidget->setCurrentIndex(2);
            emit sim_module_status_msg();
            timer->start(3000);
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G disconnect failed!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenHeight/3,screenWidth*2/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
        }
        break;
    case simDeleteSignal:
        if(strResult == QString(1))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G delete succeeded!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenHeight/3,screenWidth*2/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
            emit sim_module_status_msg();
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G delete failed!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenHeight/3,screenWidth*2/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
        }
        break;
    case simConnectSignal:
        pMovie->stop();
        loadLabel->hide();
        if(strResult == QString(1))
        {
            QMessageBox::information(this,"information",tr("4G connect succeeded!"));
            ui->stackedWidget->setCurrentIndex(0);
            emit sim_module_status_msg();
        }
        else
        {
            QMessageBox::critical(this,"information",tr("4G connect failed!"));
        }
        break;
    case simModuleStatusSignal:
        if(strResult.isEmpty())
        {
            ui->stackedWidget->setCurrentIndex(3);
            timer->stop();
            return;
        }
        list = strResult.split("\n");
        for(int i=0;i<list.size();i++)
        {
            QString tmp = list.at(i);
            if(i==0)
                ui->lbl_revisionValue->setText(tmp);
            else if(i==1)
                ui->lbl_hardsupValue->setText(tmp);
            else if(i==2)
                ui->lbl_equipValue->setText(tmp);
            else if(i==3)
                ui->lbl_portsValue->setText(tmp);
            else if(i==4)
                ui->lbl_statusValue->setText(tmp);
            else if(i==5)
                ui->lbl_powerStateValue->setText(tmp);
            else if(i==6)
                ui->lbl_accessTechValue->setText(tmp);
            else if(i==7)
                ui->lbl_signalQualityValue->setText(tmp);
            else if(i==8)
                ui->lbl_ipSupportedValue->setText(tmp);
            else if(i==9)
                ui->lbl_operatorName->setText(tmp);
        }
        break;
    case simStatusSignal:
        if(strResult.isEmpty())
        {
            // QMessageBox::critical(this,"information",tr("4G get status failed!\nPlease connect 4G first!"));
        }
        else
        {
            list = strResult.split("\n");
            for(int i=0;i<list.size();i++)
            {
                QString tmp = list.at(i);
                if(i==0)
                {
                    tmp = tmp.section(":",1,1);
                    ui->lbl_nameValue->setText(tmp);
                }
                else if(i==1)
                {
                    tmp = tmp.section(":",1,1);
                    ui->lbl_stateValue->setText(tmp);
                }
                else if(i==2)
                {
                    tmp = tmp.section(":",1,1);
                    ui->lbl_vpnValue->setText(tmp);
                }
                else if(i==3)
                {
                    tmp = tmp.section(":",1,1);
                    ui->lbl_autocpnnectValue->setText(tmp);
                }
                else if(i==4)
                {
                    tmp = tmp.section(":",1,1);
                    ui->lbl_addressValue->setText(tmp);
                }
            }

        }
        break;
    default:
        break;
    }
}

void sim_module::on_btn_disconnect_clicked()
{
    QString strCmd = QString("mmcli --list-modems");
    QString strResult =  wifi_bt_interfaceWg->executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No 4G devices!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenHeight/3,screenWidth*2/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        timer->stop();
        return;
    }
    QString result = wifi_bt_interfaceWg->executeLinuxCmd("nmcli con show --active |grep ppp0 |wc -l");
    timer->stop();
    if(openFlag == 1)
    {
        if(result == "1\n")
        {
            loadLabel->show();
            pMovie->start();
            emit sim_disconnect_msg();
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Please connect the 4G!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenHeight/3,screenWidth*2/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
        }
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the 4G!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenHeight/3,screenWidth*2/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
    }
}

void sim_module::on_btn_connect_clicked()
{  
    QString strCmd = QString("mmcli --list-modems");
    QString strResult =  wifi_bt_interfaceWg->executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No 4G devices!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenHeight/3,screenWidth*2/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        timer->stop();
        return;
    }
    timer->stop();
    loadLabel->show();
    pMovie->start();
    QString result = wifi_bt_interfaceWg->executeLinuxCmd("nmcli con show --active |grep ppp0 |wc -l");

    if(openFlag == 1)
    {

        loadLabel->show();
        if(result == "1\n")
        {
            ui->stackedWidget->setCurrentIndex(0);
            emit sim_status_msg();
            pMovie->stop();
            loadLabel->hide();
        }
        else
        {
            emit sim_connect_msg();
            emit sim_status_msg();
        }
    }
    else
    {
        pMovie->stop();
        loadLabel->hide();
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the 4G!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenHeight/3,screenWidth*2/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
    }
}


void sim_module::languageReload()
{
    ui->retranslateUi(this);
}

void sim_module::simFont()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
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
    ui->btn_connect->setFont(font);
    ui->btn_disconnect->setFont(font);
    ui->lbl_connectStatus->setFont(font);
    ui->lbl_4GStatus->setFont(font);
    ui->btn_status->setFont(font);
    ui->lbl_name->setFont(font);
    ui->lbl_state->setFont(font);
    ui->lbl_vpn->setFont(font);
    ui->lbl_autocpnnect->setFont(font);
    ui->lbl_address->setFont(font);
    ui->lbl_nameValue->setFont(font);
    ui->lbl_stateValue->setFont(font);
    ui->lbl_autocpnnectValue->setFont(font);
    ui->lbl_addressValue->setFont(font);
    ui->lbl_vpnValue->setFont(font);
    ui->lbl_4G->setFont(font);
}

void sim_module::on_btn_status_clicked()
{
    QString strCmd = QString("mmcli --list-modems");
    QString strResult =  wifi_bt_interfaceWg->executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No 4G devices!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenHeight/3,screenWidth*2/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        timer->stop();
        return;
    }
    if(openFlag == 1)
    {
        emit sim_module_status_msg();ui->stackedWidget->setCurrentIndex(2);
        timer->start(3000);
    }
    else
    {
        pMovie->stop();
        loadLabel->hide();
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the 4G!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenHeight/3,screenWidth*2/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
    }
}

void sim_module::module_status_refresh()
{
    emit sim_module_status_msg();
}

void sim_module::switch_change_flag(bool flag)
{
    flag = ui->sim_Switch->isToggled();
    if(flag == 1)
    {
        openLabel->setText(tr("  off"));
        openLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        openLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
        openFlag = 1;
        emit sim_activation_msg(1);
        QString strCmd = QString("mmcli --list-modems");
        QString strResult =  wifi_bt_interfaceWg->executeLinuxCmd(strCmd);
        QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
        if(str.isEmpty())
        {
            timer->stop();
            return;
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(2);
            emit sim_module_status_msg();
            timer->start(3000);
        }
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(3);
        timer->stop();
        openFlag = 0;
        emit sim_activation_msg(0);
        openLabel->setText(tr("on   "));
        openLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        openLabel->setStyleSheet("color: rgba(255,255,255,200);");
    }
}

void sim_module::switchSetText()
{
    openLabel = new QLabel(ui->sim_Switch);
    horLayout = new QHBoxLayout();
    openLabel->setText(tr("  off"));
    openLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    openLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    horLayout->addWidget(openLabel);
    ui->sim_Switch->setLayout(horLayout);
}
