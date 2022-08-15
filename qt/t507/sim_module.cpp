#include "sim_module.h"
#include "ui_sim_module.h"
#include <QScreen>

static QScreen *screen;
static int s_width;
static int s_height;
static int screen_flag;

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
        screen_flag = 1;
    }

    sim_font();
    wifi_bt_interface_w = new wifi_bt_interface(this);
    myThread = new QThread(this);
    SimThread = new wifi_thread();

    connect(this,SIGNAL(sim_disconnect_msg()),SimThread,SLOT(sim_disconnect_thread_func()));
    connect(this,SIGNAL(sim_connect_msg()),SimThread,SLOT(sim_connect_thread_func()));
    connect(this,SIGNAL(sim_status_msg()),SimThread,SLOT(sim_status_thread_func()));

    connect(SimThread,SIGNAL(sim_send_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    SimThread->moveToThread(myThread);
    myThread->start();
    ui->textEdit->setReadOnly(true);
}

sim_module::~sim_module()
{
    delete ui;
    delete wifi_bt_interface_w;
}

void sim_module::on_retBtn_clicked()
{
    emit Mysignal();
}

void sim_module::recv_msg(int signal_type, QString strResult)
{
    switch (signal_type)
    {
    case sim_disconnect_signal:
        qDebug() << "Line:" << __LINE__<< "FILE" << __FILE__<< "FUNC:" << __FUNCTION__ << "strResult:" << strResult;
        if(strResult == QString(1))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G Disconnection succeeded!"),
                             0,this);
             mesg.addButton(tr("OK"),QMessageBox::YesRole);
             if(screen_flag == 1)
             mesg.move(s_width*2/3,s_height/3);
             else
             mesg.move(s_width/3,s_height/3);
             mesg.exec();
             ui->textEdit->clear();
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G Disconnection failed!"),
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
        qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;
        if(strResult == QString(1))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G Delete failed!"),
                             0,this);
             mesg.addButton(tr("OK"),QMessageBox::YesRole);
             if(screen_flag == 1)
             mesg.move(s_width*2/3,s_height/3);
             else
             mesg.move(s_width/3,s_height/3);
             mesg.exec();
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("4G Delete failed!"),
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
        qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;
        if(strResult == QString(1))
        {
            QMessageBox::information(this,"information",tr("4G Connect succeeded!"));
        }
        else
        {
            QMessageBox::critical(this,"information",tr("4G Connect failed!"));
        }
        break;
    case sim_status_signal:
        qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;

        if(strResult.isEmpty())
        {
            QMessageBox::critical(this,"information",tr("4G get status failed!\nPlease connect 4G first!"));
        }
        else
        {
            ui->textEdit->setText(strResult);
//            QMessageBox::information(this,"information",strResult);
        }
        break;
    default:
        break;
    }
}

void sim_module::on_SimDisconnectBtn_clicked()
{
    qDebug() << "--line--: " << __LINE__<< "FILE" << __FILE__<<"func:" << __FUNCTION__;
    emit sim_disconnect_msg();
}

void sim_module::on_SimConnectBtn_clicked()
{
    qDebug() << "--line--: " << __LINE__<< "FILE" << __FILE__<<"func:" << __FUNCTION__;
    emit sim_connect_msg();
}


void sim_module::language_reload()
{
    ui->retranslateUi(this);
}

void sim_module::on_SimStausBtn_clicked()
{
    qDebug() << "--line--: " << __LINE__<< "FILE" << __FILE__<<"func:" << __FUNCTION__;

    emit sim_status_msg();
}

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
    ui->textEdit->setFont(font);
    ui->SimStausBtn->setFont(font);
    ui->label->setFont(font);
}
