#include "sim_module.h"
#include "ui_sim_module.h"

sim_module::sim_module(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sim_module)
{
    ui->setupUi(this);
    wifi_bt_interface_w = new wifi_bt_interface(this);

    myThread = new QThread(this);
    SimThread = new wifi_thread();

    connect(this,SIGNAL(sim_disconnect_msg()),SimThread,SLOT(sim_disconnect_thread_func()));
    connect(this,SIGNAL(sim_connect_msg()),SimThread,SLOT(sim_connect_thread_func()));
    connect(this,SIGNAL(sim_status_msg()),SimThread,SLOT(sim_status_thread_func()));

    connect(SimThread,SIGNAL(sim_send_msg(int, QString)),this,SLOT(recv_msg(int, QString)));
    SimThread->moveToThread(myThread);
    myThread->start();

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
            QMessageBox::information(this,"information",tr("4G Disconnection succeeded!"));
        }
        else
        {
            QMessageBox::critical(this,"information",tr("4G Disconnection failed!"));
        }
        break;
    case sim_delete_signal:
        qDebug() << "FUNC:" << __FUNCTION__<< "Line:" << __LINE__ << "strResult:" << strResult;
        if(strResult == QString(1))
        {
            QMessageBox::information(this,"information",tr("4G Delete succeeded!"));
        }
        else
        {
            QMessageBox::critical(this,"information",tr("4G Delete failed!"));
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
            QMessageBox::critical(this,"information",tr("4G get status failed!"));
        }
        else
        {
            QMessageBox::information(this,"information",strResult);
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
