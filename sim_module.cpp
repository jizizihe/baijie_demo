#include "sim_module.h"
#include "ui_sim_module.h"
#include <QScreen>

static QScreen *screen;
static int s_width;
static int s_height;
static int screen_flag;
static int open_flag;

sim_module::sim_module(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sim_module)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();

    if(s_width < s_height)
    {
        screen_flag = 1;ui->line_2->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    open_flag = 1;
    sim_font();
    timer = new QTimer(this);
    LoadLabel = new QLabel(this);
    pMovie = new QMovie("://button_image/loading.webp");
    LoadLabel->setFixedSize(50, 50);
    LoadLabel->setScaledContents(true);
    LoadLabel->setMovie(pMovie);
    wifi_bt_interface_w = new wifi_bt_interface(this);
    myThread = new QThread(this);
    SimThread = new wifi_thread();
    ui->sim_Switch->setCheckedColor(QColor(100, 225, 100, 150));
    connect(ui->sim_Switch,SIGNAL(toggled(bool)),this,SLOT(btnChangeFlag(bool)));
    connect(timer,SIGNAL(timeout()),this,SLOT(status_4g_refresh()));
    connect(this,SIGNAL(sim_disconnect_msg()),SimThread,SLOT(sim_disconnect_thread_func()));
    connect(this,SIGNAL(sim_connect_msg()),SimThread,SLOT(sim_connect_thread_func()));
    connect(this,SIGNAL(sim_status_msg()),SimThread,SLOT(sim_status_thread_func()));
    connect(this,SIGNAL(sim_4gstatus()),SimThread,SLOT(sim_4gstatus()));
    connect(SimThread,SIGNAL(sim_send_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    connect(this,SIGNAL(sim_activation(int)),SimThread,SLOT(sim_activation(int)));
    SimThread->moveToThread(myThread);
    myThread->start();
    emit sim_4gstatus();

    emit sim_status_msg();ui->stackedWidget->setCurrentIndex(2);
}

sim_module::~sim_module()
{
    delete ui;
    delete wifi_bt_interface_w;
}

void sim_module::showEvent(QShowEvent *event)
{
    timer->start(3000);
}

void sim_module::on_retBtn_clicked()
{
    timer->stop();
    emit Mysignal();
}

void sim_module::recv_msg(int signal_type, QString strResult)
{
    QStringList list;
    switch (signal_type)
    {
    case sim_disconnect_signal:
        pMovie->stop();
        LoadLabel->hide();
        if(strResult == QString(1))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G disconnect succeeded!"),
                             0,this);
             mesg.addButton(tr("OK"),QMessageBox::YesRole);
             if(screen_flag == 1)
             mesg.move(s_width*2/3,s_height/3);
             else
             mesg.move(s_width/3,s_height/3);
             mesg.exec();
             ui->stackedWidget->setCurrentIndex(2);
             emit sim_4gstatus();
             timer->start(3000);
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G disconnect failed!"),
                             0,this);
             mesg.addButton(tr("OK"),QMessageBox::YesRole);
             if(screen_flag == 1)
             mesg.move(s_width*2/3,s_height/3);
             else
             mesg.move(s_width/3,s_height/3);
             mesg.exec();
        }
        break;
    case sim_delete_signal:
        //qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;
        if(strResult == QString(1))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G delete succeeded!"),
                             0,this);
             mesg.addButton(tr("OK"),QMessageBox::YesRole);
             if(screen_flag == 1)
             mesg.move(s_width*2/3,s_height/3);
             else
             mesg.move(s_width/3,s_height/3);
             mesg.exec();
             emit sim_4gstatus();
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G delete failed!"),
                             0,this);
             mesg.addButton(tr("OK"),QMessageBox::YesRole);
             if(screen_flag == 1)
             mesg.move(s_width*2/3,s_height/3);
             else
             mesg.move(s_width/3,s_height/3);
             mesg.exec();
        }
        break;
    case sim_connect_signal:
        pMovie->stop();
        LoadLabel->hide();
        if(strResult == QString(1))
        {
            QMessageBox::information(this,"information",tr("4G connect succeeded!"));
            ui->stackedWidget->setCurrentIndex(0);
            emit sim_4gstatus();
        }
        else
        {
            QMessageBox::critical(this,"information",tr("4G connect failed!"));
        }
        break;
    case sim_4gstatus_signal:
        //ui->textEdit_2->setText(strResult);
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
               ui->revision_label->setText(tmp);
           else if(i==1)
               ui->hardsup_label->setText(tmp);
           else if(i==2)
               ui->equip_label->setText(tmp);
           else if(i==3)
               ui->port_label->setText(tmp);
           else if(i==4)
               ui->status_label->setText(tmp);
           else if(i==5)
               ui->power_label->setText(tmp);
           else if(i==6)
               ui->access_label->setText(tmp);
           else if(i==7)
               ui->signal_label->setText(tmp);
           else if(i==8)
               ui->ip_label->setText(tmp);
           else if(i==9)
               ui->operat_label->setText(tmp);
        }
        //ui->stackedWidget->setCurrentIndex(2);
        break;
    case sim_status_signal:

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
                   ui->name_label->setText(tmp);
               }
               else if(i==1)
               {
                   tmp = tmp.section(":",1,1);
                   ui->state_label->setText(tmp);
               }
               else if(i==2)
               {
                   tmp = tmp.section(":",1,1);
                   ui->vpn_label->setText(tmp);
               }
               else if(i==3)
               {
                   tmp = tmp.section(":",1,1);
                   ui->auto_label->setText(tmp);
               }
               else if(i==4)
               {
                   tmp = tmp.section(":",1,1);
                   ui->address_label->setText(tmp);
               }
            }

        }
        break;
    default:
        break;
    }
}

void sim_module::on_SimDisconnectBtn_clicked()
{
    QString strCmd = QString("mmcli --list-modems");
    QString strResult =  wifi_bt_interface_w->executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        QMessageBox::information(this,"information",tr("No 4G devices!"));
        timer->stop();
        return;
    }
    QString result = wifi_bt_interface_w->executeLinuxCmd("nmcli con show --active |grep ppp0 |wc -l");
    timer->stop();
    if(open_flag == 1)
    {
        if(result == "1\n")
        {
            LoadLabel->show();
            LoadLabel->move(this->width()/2,this->height()/2);
            pMovie->start();
            emit sim_disconnect_msg();
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Please connect the 4g!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screen_flag == 1)
                mesg.move(s_width*2/3,s_height/3);
            else
                mesg.move(s_width/3,s_height/3);
            mesg.exec();

        }
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the 4g!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
            mesg.move(s_width*2/3,s_height/3);
        else
            mesg.move(s_width/3,s_height/3);
        mesg.exec();
    }
}

void sim_module::on_SimConnectBtn_clicked()
{  
    QString strCmd = QString("mmcli --list-modems");
    QString strResult =  wifi_bt_interface_w->executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        QMessageBox::information(this,"information",tr("No 4G devices!"));
        timer->stop();
        return;
    }
    timer->stop();
    LoadLabel->show();
    LoadLabel->move(this->width()/2,this->height()/2);
    pMovie->start();
    QString result = wifi_bt_interface_w->executeLinuxCmd("nmcli con show --active |grep ppp0 |wc -l");

    if(open_flag == 1)
    {

        LoadLabel->show();
        LoadLabel->move(this->width()/2,this->height()/2);
        if(result == "1\n")
        {
            ui->stackedWidget->setCurrentIndex(0);
            emit sim_status_msg();
            pMovie->stop();
            LoadLabel->hide();
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
        LoadLabel->hide();
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the 4g!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
            mesg.move(s_width*2/3,s_height/3);
        else
            mesg.move(s_width/3,s_height/3);
        mesg.exec();
    }
}


void sim_module::language_reload()
{
    ui->retranslateUi(this);
}

//void sim_module::on_SimStausBtn_clicked()
//{
//    qDebug() << "--line--: " << __LINE__<< "FILE" << __FILE__<<"func:" << __FUNCTION__;

//    emit sim_status_msg();
//}

void sim_module::sim_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
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
    ui->SimConnectBtn->setFont(font);
    ui->SimDisconnectBtn->setFont(font);
    ui->label_2->setFont(font);
    ui->label->setFont(font);
    ui->label_3->setFont(font);
    ui->statusBtn->setFont(font);
    ui->label_14->setFont(font);
    ui->label_15->setFont(font);
    ui->label_16->setFont(font);
    ui->label_17->setFont(font);
    ui->label_18->setFont(font);
    ui->name_label->setFont(font);
    ui->state_label->setFont(font);
    ui->auto_label->setFont(font);
    ui->address_label->setFont(font);
    ui->vpn_label->setFont(font);
}

//void sim_module::on_btn_open_clicked()
//{
//    if(open_flag == 0)
//    {
//        open_flag = 1;
//        ui->btn_open->setText(tr("close"));
//        emit sim_activation(1);
//        QString strCmd = QString("mmcli --list-modems");
//        QString strResult =  wifi_bt_interface_w->executeLinuxCmd(strCmd);
//        QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
//        if(str.isEmpty())
//        {
//            timer->stop();
//            return;
//        }
//        else
//        {
//            ui->stackedWidget->setCurrentIndex(2);
//            emit sim_4gstatus();
//            timer->start(3000);
//        }
//    }
//    else
//    {
//        ui->stackedWidget->setCurrentIndex(3);
//        timer->stop();
//        open_flag = 0;
//        ui->btn_open->setText(tr("open"));
//        emit sim_activation(0);
//    }
//}

void sim_module::on_statusBtn_clicked()
{
    QString strCmd = QString("mmcli --list-modems");
    QString strResult =  wifi_bt_interface_w->executeLinuxCmd(strCmd);
    QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
    if(str.isEmpty())
    {
        QMessageBox::information(this,"information",tr("No 4G devices!"));
        timer->stop();
        return;
    }
    if(open_flag == 1)
    {
        emit sim_4gstatus();ui->stackedWidget->setCurrentIndex(2);
     //    ui->stackedWidget->setCurrentIndex(2);
        timer->start(3000);
    }
    else
    {
        pMovie->stop();
        LoadLabel->hide();
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please open the 4g!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
            mesg.move(s_width*2/3,s_height/3);
        else
            mesg.move(s_width/3,s_height/3);
        mesg.exec();
    }
}

void sim_module::status_4g_refresh()
{
    emit sim_4gstatus();
}

void sim_module::btnChangeFlag(bool flag)
{
    flag = ui->sim_Switch->isToggled();
    if(flag == 1)
    {

        open_flag = 1;
        emit sim_activation(1);
        QString strCmd = QString("mmcli --list-modems");
        QString strResult =  wifi_bt_interface_w->executeLinuxCmd(strCmd);
        QString str = strResult.section("/",5,5);str = str.section(" ",0,0);
        if(str.isEmpty())
        {
            timer->stop();
            return;
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(2);
            emit sim_4gstatus();
            timer->start(3000);
        }
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(3);
        timer->stop();
        open_flag = 0;
        emit sim_activation(0);
    }
}
