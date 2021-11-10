#include "serial.h"
#include "ui_serial.h"

serial::serial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serial)
{
    ui->setupUi(this);

    m_portNameList = getPortNameList();
    ui->SerialCb->addItems(m_portNameList);
    ui->SerialCb_2->addItems(m_portNameList);
    if(ui->SerialCb_2->count()>2)
    {
        ui->SerialCb->setCurrentIndex(0);
        ui->SerialCb_2->setCurrentIndex(1);
    }
}

serial::~serial()
{
    delete ui;
}

QStringList serial::getPortNameList()
{
    QStringList serialPortName;
    //查找可用的串口
    foreach(const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())//读取串口信息
    {
        serialPortName << Info.portName();
        //qDebug()<<"portname: "<<Info.portName();//调试时可以看的串口信息
    }
    return serialPortName;
}

void serial::on_OpenBtn_clicked()
{
    int OpenFlag = 0;
    //对串口进行一些初始化
    if(ui->OpenBtn->text()==tr("open"))
    {
         PortA = new serial_thread(COM0,ui->SerialCb->currentText(), ui->BaudrateCb->currentText().toInt(),&OpenFlag);
         if(OpenFlag == 1)
         {
             QMessageBox::information(this,"information",tr("success!"));
         }
         else
         {
             QMessageBox::critical(this,"information",tr("failed!"));
         }

        //接收从子线程传输数据的信号
        connect(PortA,SIGNAL(receive_data(int,QString)),this,SLOT(on_showData(int,QString)));//,Qt::QueuedConnection
        connect(this,SIGNAL(writePort_sig(int,QByteArray)),PortA,SLOT(write_data(int,QByteArray)));
        connect(this,SIGNAL(closePort_sig(int)),PortA,SLOT(closePort(int)));
        //关闭设置菜单使能
        ui->SerialCb->setEnabled(false);
        ui->BaudrateCb->setEnabled(false);
        ui->OpenBtn->setText(tr("close"));
    }
    else
    {
        //关闭线程
        if(PortA!=nullptr)
        {
            emit closePort_sig(COM0);
        }
        //恢复设置菜单使能
        ui->SerialCb->setEnabled(true);
        ui->BaudrateCb->setEnabled(true);
        ui->OpenBtn->setText(tr("open"));
    }

}

void serial::on_OpenBtn_2_clicked()
{
    int OpenFlag = 0;
    //对串口进行一些初始化
    if(ui->OpenBtn_2->text()==tr("open"))
    {
        PortB = new serial_thread(COM1,ui->SerialCb_2->currentText(), ui->BaudrateCb_2->currentText().toInt(),&OpenFlag);
        if(OpenFlag == 1)
        {
            QMessageBox::information(this,"information",tr("success!"));
        }
        else
        {
            QMessageBox::critical(this,"information",tr("failed!"));
        }
       //接收从子线程传输数据的信号
        connect(PortB,SIGNAL(receive_data(int,QString)),this,SLOT(on_showData(QString)));//,Qt::QueuedConnection
        connect(this,SIGNAL(writePort_sig(int,QByteArray)),PortB,SLOT(write_data(int,QByteArray)));
        connect(this,SIGNAL(closePort_sig(int)),PortB,SLOT(closePort(int)));

       //关闭设置菜单使能
       ui->SerialCb_2->setEnabled(false);
       ui->BaudrateCb_2->setEnabled(false);
       ui->OpenBtn_2->setText(tr("close"));
    }
    else
    {
        qDebug() << "LINE: " << __LINE__ << "CLOSE BUTTON" << endl;
        //关闭线程
        if(PortB!=nullptr)
        {
             emit closePort_sig(COM1);
        }
        //恢复设置菜单使能
        ui->SerialCb_2->setEnabled(true);
        ui->BaudrateCb_2->setEnabled(true);
        ui->OpenBtn_2->setText(tr("open"));
    }

}

void serial::on_showData(QString buff)//用于显示数据
{
    qDebug() << "LINE:" << __LINE__ << buff << endl;

    ui->recvEdit->appendPlainText(buff);
}

void serial::on_SendBtn_clicked()
{
    qDebug() << "LINE:" << __LINE__ << "on_sendBtn_clicked" << endl;

    emit writePort_sig(COM0,ui->SendEdit->text().toUtf8());

}

void serial::on_SendBtn_2_clicked()
{
    qDebug() << "LINE:" << __LINE__ << "on_sendBtn_clicked" << endl;

    emit writePort_sig(COM1,ui->SendEdit->text().toUtf8());
}

void serial::on_CleanBtn_clicked()
{
    ui->SendEdit->clear();
}

void serial::on_CleanBtn_2_clicked()
{
    ui->recvEdit->clear();
}

void serial::on_retBtn_clicked()
{
    emit Mysignal();
}

void serial::language_reload()
{
    ui->retranslateUi(this);
}
