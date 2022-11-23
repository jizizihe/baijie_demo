#include "sim.h"
#include "ui_sim_module.h"
#include <QScreen>
#include <QMessageBox>

static QScreen *g_screen;
static int g_screenWidth;
static int g_screenHeight;
static int g_openFlag;
static int g_showFlag;
static QLabel *g_openLabel;
static QHBoxLayout *g_horLayout;

sim_module::sim_module(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sim_module)
{
    ui->setupUi(this);
    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();
    g_openFlag = 1;
    setSimFont();
    g_timer = new QTimer(this);
    g_loadLabel = new QLabel(this);
    g_loadMovie = new QMovie("://button_image/loading.webp");
    g_loadLabel->setFixedSize(50, 50);
    g_loadLabel->setScaledContents(true);
    g_loadLabel->setMovie(g_loadMovie);
    g_loadLabel->move(g_screenWidth/2,g_screenHeight/2 );
    g_simInterface = new sim_interface(this);
    g_myThread = new QThread(this);
    g_SimThread = new sim_thread();
    ui->sim_Switch->setCheckedColor(QColor(100, 225, 100, 150));
    setSwitchText();

    connect(ui->sim_Switch,SIGNAL(toggled(bool)),this,SLOT(switch_change_flag(bool)));
    connect(g_timer,SIGNAL(timeout()),this,SLOT(module_status_refresh()));
    connect(this,SIGNAL(sim_disconnect_msg()),g_SimThread,SLOT(sim_disconnect_thread()));
    connect(this,SIGNAL(sim_connect_msg()),g_SimThread,SLOT(sim_connect_thread()));
    connect(this,SIGNAL(sim_status_msg()),g_SimThread,SLOT(sim_status_thread()));
    connect(this,SIGNAL(sim_module_status_msg()),g_SimThread,SLOT(sim_module_status()));
    connect(g_SimThread,SIGNAL(send_sim_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    connect(this,SIGNAL(sim_set_enble_msg(int)),g_SimThread,SLOT(sim_set_enbale(int)));

    g_SimThread->moveToThread(g_myThread);
    g_myThread->start();
    emit sim_module_status_msg();
    emit sim_status_msg();
    ui->stackedWidget->setCurrentIndex(2);
}

sim_module::~sim_module()
{
    delete ui;
    delete g_simInterface;
    delete g_openLabel;
    delete g_horLayout;
}

void sim_module::showEvent(QShowEvent *event)
{
    g_showFlag = 1;
    g_timer->start(3000);
    QWidget::showEvent(event);
}

void sim_module::on_btn_ret_clicked()
{
    g_timer->stop();
    g_showFlag = 0;
    emit sim_back_msg();
}

void sim_module::recv_msg(int signalType, QString strResult)
{
    QStringList list;
    switch (signalType)
    {
    case EnumSimDisconnectSignal:
        g_loadMovie->stop();
        g_loadLabel->hide();
        if(strResult == QString(1))
        {
            QMessageBox::information(this,"information",tr("4G disconnect succeeded!"));
            ui->stackedWidget->setCurrentIndex(2);
            emit sim_module_status_msg();
            g_timer->start(3000);
        }
        else
        {
            QMessageBox::information(this,"information",tr("4G disconnect succeeded!"));
        }
        break;
    case EnumSimDeleteSignal:
        if(strResult == QString(1))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G delete succeeded!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
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
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
        }
        break;
    case EnumSimConnectSignal:
        g_loadMovie->stop();
        g_loadLabel->hide();
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
    case EnumSimModuleStatusSignal:
        if(strResult.isEmpty())
        {
            ui->stackedWidget->setCurrentIndex(3);
            g_timer->stop();
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
                ui->lbl_operatorNameValue->setText(tmp);
        }
        break;
    case EnumSimStatusSignal:
        if(!strResult.isEmpty())
        {
            list = strResult.split("\n");
            for(int i=0;i<list.size();i++)
            {
                QString str = list.at(i);
                if(i==0)
                {
                    str = str.section(":",1,1);
                    ui->lbl_nameValue->setText(str);
                }
                else if(i==1)
                {
                    str = str.section(":",1,1);
                    ui->lbl_stateValue->setText(str);
                }
                else if(i==2)
                {
                    str = str.section(":",1,1);
                    ui->lbl_vpnValue->setText(str);
                }
                else if(i==3)
                {
                    str = str.section(":",1,1);
                    ui->lbl_autocpnnectValue->setText(str);
                }
                else if(i==4)
                {
                    str = str.section(":",1,1);
                    ui->lbl_addressValue->setText(str);
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
    QString strResult =  g_simInterface->executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);
    str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No 4G devices!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        g_timer->stop();
        return;
    }
    QString result = g_simInterface->executeLinuxCmd("nmcli con show --active |grep ppp0 |wc -l");
    g_timer->stop();
    if(g_openFlag == 1)
    {
        if(result == "1\n")
        {
            g_loadLabel->show();
            g_loadMovie->start();
            emit sim_disconnect_msg();
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Please connect the 4G!"),
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
                         tr("Please open the 4G!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
    }
}

void sim_module::on_btn_connect_clicked()
{  
    QString strCmd = QString("mmcli --list-modems");
    QString strResult =  g_simInterface->executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);
    str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No 4G devices!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        g_timer->stop();
        return;
    }
    g_timer->stop();
    g_loadLabel->show();
    g_loadMovie->start();
    QString result = g_simInterface->executeLinuxCmd("nmcli con show --active |grep ppp0 |wc -l");

    if(g_openFlag == 1)
    {
        g_loadLabel->show();
        if(result == "1\n")
        {
            ui->stackedWidget->setCurrentIndex(0);
            emit sim_status_msg();
            g_loadMovie->stop();
            g_loadLabel->hide();
        }
        else
        {
            emit sim_connect_msg();
            emit sim_status_msg();
        }
    }
    else
    {
        g_loadMovie->stop();
        g_loadLabel->hide();
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the 4G!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
    }
}


void sim_module::languageReload()
{
    ui->retranslateUi(this);
    int flag = ui->sim_Switch->isToggled();
    if(flag == 1)
    ui->sim_Switch->setToggle(true);
    else
    ui->sim_Switch->setToggle(false);
}

void sim_module::setSimFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;

    if(realWidth < 15)
    {
        font.setPointSize(12);
    }
    else if (realWidth < 18)
    {
        font.setPointSize(14);
    }
    else
    {
        font.setPointSize(17);
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
    QString strResult =  g_simInterface->executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No 4G devices!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        g_timer->stop();
        return;
    }
    if(g_openFlag == 1)
    {
        emit sim_module_status_msg();ui->stackedWidget->setCurrentIndex(2);
        g_timer->start(3000);
    }
    else
    {
        g_loadMovie->stop();
        g_loadLabel->hide();
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the 4G!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
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
        g_openLabel->setText(tr("  off"));
        g_openLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        g_openLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
        g_openFlag = 1;
        if(g_showFlag == 1)
        {
            emit sim_set_enble_msg(1);
            QString strCmd = QString("mmcli --list-modems");
            QString strResult =  g_simInterface->executeLinuxCmd(strCmd);
            QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
            if(str.isEmpty())
            {
                g_timer->stop();
                return;
            }
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(2);
            emit sim_module_status_msg();
            g_timer->start(3000);
        }
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(3);
        g_timer->stop();
        g_openFlag = 0;
        emit sim_set_enble_msg(0);
        g_openLabel->setText(tr("on   "));
        g_openLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        g_openLabel->setStyleSheet("color: rgba(255,255,255,200);");
    }
}

void sim_module::setSwitchText()
{
    g_openLabel = new QLabel(ui->sim_Switch);
    g_horLayout = new QHBoxLayout();
    g_openLabel->setText(tr("  off"));
    g_openLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    g_openLabel->setStyleSheet("color: rgba(0, 0, 0,180);");
    g_horLayout->addWidget(g_openLabel);
    ui->sim_Switch->setLayout(g_horLayout);
}
