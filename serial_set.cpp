#include "serial_set.h"
#include "ui_serial_set.h"
#include <QDebug>
#include <QScreen>

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;

serial_set::serial_set(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serial_set)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();

    if(s_width < s_height)
    {
        screen_flag = 1;
    }
    serial_set_font();
    m_portNameList = getPortNameList();
    ui->SerialCb->addItems(m_portNameList);
    ui->SerialCb->setCurrentIndex(0);

    ui->SendBtn->setText(QString(tr("cancel")));
    this->setAttribute(Qt::WA_ShowWithoutActivating,true);
    this->setFocusPolicy(Qt::NoFocus);
}

serial_set::~serial_set()
{
    delete ui;
}

QStringList serial_set::getPortNameList()
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

void serial_set::on_OpenBtn_clicked()
{
   QString port = ui->SerialCb->currentText();
   QString baud = ui->BaudrateCb->currentText();
   int data = ui->DatabitsCb->currentText().toInt();
   QString stopbit = ui->StopbitCb->currentText();
   emit port1_set(port,baud,data,stopbit);

   QMessageBox mesg(QMessageBox::Information,
                    tr("QMessageBox::information()"),
                    tr("set successfully!"),
                    0,this);
    mesg.setAttribute(Qt::WA_ShowWithoutActivating,true);
    mesg.setFocusPolicy(Qt::NoFocus);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    if(screen_flag == 1)
    mesg.move(s_width*2/3,s_height/3);
    mesg.exec();
    hide_flag = 1;
   emit port1_re();

}

void serial_set::on_SendBtn_clicked()
{
    //QMessageBox::information(this,"information",tr("set failed!"));
     hide_flag = 0;
     emit port1_re();

}

void serial_set::language_reload()
{
     ui->retranslateUi(this);
}

void serial_set::serial_set_font()
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
    ui->PartNamelab_26->setFont(font);
    ui->PartNamelab_27->setFont(font);
    ui->PartNamelab_28->setFont(font);
    ui->PartNamelab_29->setFont(font);
    ui->PartNamelab_30->setFont(font);
    ui->DatabitsCb->setFont(font);
    ui->BaudrateCb->setFont(font);
    ui->ParityCb->setFont(font);
    ui->SerialCb->setFont(font);
    ui->StopbitCb->setFont(font);
    ui->OpenBtn->setFont(font);
    ui->SendBtn->setFont(font);
    ui->groupBox->setFont(font);
}
