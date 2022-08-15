#include "serial_set_port2.h"
#include "ui_serial_set_port2.h"
#include <QScreen>
#include <QDebug>

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;

serial_set_port2::serial_set_port2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serial_set_port2)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();

    if(s_width < s_height)
    {
        screen_flag = 1;
    }
    serial_set2_font();
    m_portNameList = getPortNameList();
    ui->SerialCb_2->addItems(m_portNameList);
    ui->SerialCb_2->setCurrentIndex(1);
    this->setAttribute(Qt::WA_ShowWithoutActivating,true);
    this->setFocusPolicy(Qt::NoFocus);
}

serial_set_port2::~serial_set_port2()
{
    delete ui;
}

QStringList serial_set_port2::getPortNameList()
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
void serial_set_port2::on_OK_Btn_clicked()
{
    QString port = ui->SerialCb_2->currentText();
    QString baud = ui->BaudrateCb_2->currentText();
    int data = ui->DatabitsCb_2->currentText().toInt();
    QString Stopbit = ui->StopbitCb_2->currentText();
    QMessageBox::information(this,"information",tr("set successfully!"));
    emit port2_set(port,baud,data,Stopbit);
    emit port2_re();
}

void serial_set_port2::on_SendBtn_2_clicked()
{
   // QMessageBox::information(this,"information",tr("set failed!"));
    emit port2_re();
}

void serial_set_port2::language_reload()
{
     ui->retranslateUi(this);
}

void serial_set_port2::serial_set2_font()
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
    ui->PartNamelab_23->setFont(font);
    ui->PartNamelab_24->setFont(font);
    ui->PartNamelab_25->setFont(font);
    ui->PartNamelab_22->setFont(font);
    ui->PartNamelab_21->setFont(font);
    ui->DatabitsCb_2->setFont(font);
    ui->BaudrateCb_2->setFont(font);
    ui->ParityCb_2->setFont(font);
    ui->SerialCb_2->setFont(font);
    ui->StopbitCb_2->setFont(font);
    ui->OK_Btn->setFont(font);
    ui->SendBtn_2->setFont(font);
    ui->groupBox_2->setFont(font);
}
