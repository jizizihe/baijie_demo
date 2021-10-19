#include "udev.h"
#include "ui_udev.h"
//#include "interface_gpio.h"
//#include "gpio_interface.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QTime>
#include <QMessageBox>
#include <QPalette>



udev::udev(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::udev)
{
    ui->setupUi(this);
    proc = new QProcess;

    connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(readoutput()));
}

udev::~udev()
{
    delete ui;
    proc->close();
    delete proc;
    proc = 0;
}

void udev::readoutput()
{
    qDebug() << "<<<<<-------------------"<< file_name <<"-------------------->>>>>>";
    array = proc->readAllStandardOutput().data();

    ui->message->setText(array);

    QFile file(file_name);

    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << array;
    file.flush();
    file.close();;

    static bool flag = false;

    QString out_f = QString("******************************************************************************************");



    if(file_name == "/data/1.txt")
    {
        file.open(QIODevice::ReadOnly);
        QByteArray array[10];
        int i;
        for(i = 0; !file.atEnd();++i)
        {
            array[i] = file.readLine();
        }

        QByteArray temp;
        for(int x = 0; x < i; x++)
        {
            temp = array[x];
            array[x] = array[x].remove(0,28).left(4);
            if(array[x] == "0001" || array[x] == "0125" || array[x] == "0101" || array[x] == "0002" || array[x] == "772b")
            {
                flag = false;
            }
            else
            {
                flag = true;
                i = x;
                break;
            }
        }
        if(flag)
        {
//            QPalette pale = ui->message->palette();
//            pale.setColor(QPalette::Text,Qt::red);
////            ui->message->setPalette(pale);

//            // 先保存当前的文字颜色
//            auto cur_text_color = m_textline->textColor();
//            // 设置当前行要使用的颜色，假设为红色
//            m_textline->setTextColor(Qt::red);
//            // 写入一行内容
//            QString str = u8"This a test line";
//            m_textline.append(str);
//            // 最后恢复原来的颜色
//            m_textline->setTextColor(cur_text_color);

            ui->message->append(QString("%1\nThe New USB device is:\n%2%3").arg(out_f).arg(temp.data()).arg(out_f));
            ui->usb_lineEdit->setText("Find the New USB device");
        }
        else
            ui->usb_lineEdit->setText("New USB device is not found");

        qDebug() << "lsusb***********>>>";
    }
    else if(file_name == "/data/2.txt")
    {
        flag = false;
        file.open(QIODevice::ReadOnly);

        QByteArray array[140];
        int i;

        for(i = 0; !file.atEnd(); i++)
        {
            array[i] = file.readLine();
        }

        for(int x = i - 2; x < i; x++)
        {
            qDebug() << "---<<>>---array:" <<array[x];
            if("/dev/mmcblk1p1" == array[x].left(14))
            {
                flag = true;
                i = x;
                break;
            }
        }


        if(flag)
        {
//            QPalette pale = ui->message->palette();
//            pale.setColor(QPalette::Text,Qt::red);
//            ui->message->setPalette(pale);
            ui->message->append(QString("%1\nThe New SD card is :\n%2%3\n").arg(out_f).arg(array[i].data()).arg(out_f));
            ui->sd_lineEdit->setText("Find the New Sd card");
        }
        else
            ui->sd_lineEdit->setText("New SD card is not found");

        qDebug() << "fdisk -l ***********>>>";
    }
    else if(file_name == "/data/3.txt")
    {
        flag = false;
        file.open(QIODevice::ReadOnly);
        QByteArray array[35];
        int i;

        for(i = 0; !file.atEnd(); ++i)
        {
            array[i] = file.readLine();
        }

        for(int x = 0; x < i; x++)
        {
            if("    " == array[x].left(4) || "\n" == array[x].left(1))
            {
                continue;
            }
            array[x] = array[x].left(4);
            qDebug() << "---<<>>---array:" <<array[x];
            if(array[x] == "ppp0")
            {
                flag = true;
                i = x;
                break;
            }
        }
        qDebug() << "******************* 4G IP :"<<array[i+1].remove(0,8);

        if(flag)
        {
//            QPalette pale = ui->message->palette();
//            pale.setColor(QPalette::Text,Qt::red);
//            ui->message->setPalette(pale);
            ui->message->append(QString("%1\nThe New SIM card is:\n4G_IP:%2%3").arg(out_f).arg(array[i+1].data()).arg(out_f));
            ui->sim_lineEdit->setText("Find the New SIM card");
        }
        else
            ui->sim_lineEdit->setText("New SIM card is not found");
        qDebug() << "ifconfig***********>>>";
    }
    file.flush();
    file.close();
}



void udev::interface_gpio(int val)
{
    int	port_num;
    FILE *p=NULL;
    char str[256];


    port_num = 236;
    p = fopen("/sys/class/gpio/export","w");
    fprintf(p,"%d",port_num);
    fclose(p);
    sprintf(str, "/sys/class/gpio/gpio%d/direction", port_num);
    p = fopen(str,"w");
    fprintf(p,"out");
    fclose(p);

    sprintf(str, "/sys/class/gpio/gpio%d/value", port_num);
    p = fopen(str,"w");
    fprintf(p,"%d",val>0 ? 1 : 0);
    fclose(p);
    qDebug() << "gpio port ph12: set to" << val;
}


void udev::on_usb_detection_clicked()
{
    ui->usb_lineEdit->setText("");

    file_name = QString("/data/1.txt");

    proc->start(QString("rm %1").arg(file_name));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);

    proc->start(QString("touch %1").arg(file_name));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);

    proc->start("lsusb");
}

void udev::on_sd_detection_clicked()
{
    ui->sd_lineEdit->setText("");

    file_name = QString("/data/2.txt");

    proc->start(QString("rm %1").arg(file_name));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);

    proc->start(QString("touch %1").arg(file_name));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);

    proc->start("fdisk -l");
}

void udev::on_sim_detection_clicked()
{

    ui->sim_lineEdit->setText(" ");

    file_name = QString("/data/3.txt");

    proc->start(QString("rm %1").arg(file_name));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);

    proc->start(QString("touch %1").arg(file_name));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);

    proc->start("ifconfig");
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);


    QString temp = ui->sim_lineEdit->text();
    qDebug() << "temp:" << temp;
    if(temp == "New SIM card is not found")
    {
        QMessageBox::information(NULL,NULL, QString("That may be a long time!!Please note the 4G LED on the board when it appears fast flashing!!"));
        file_name = "";
        qDebug() << "OH No!!!";
        interface_gpio(0);
//        int gpio_num = calc_port_num('h',12);
//        gpio_export(gpio_num);
//        gpio_set_state(gpio_num,"out");
//        gpio_set_value(gpio_num,0);

        qDebug() << "interface_gpio set : 0";
        proc->start("nmcli connection delete ppp0");
        proc->waitForStarted(-1);
        proc->waitForFinished(-1);
        sleep(2);
        interface_gpio(1);
//        gpio_set_value(gpio_num,1);
//        gpio_unexport(gpio_num);
        sleep(1);
        proc->start("nmcli connection add con-name ppp0 ifname ttyUSB2 autoconnect yes \
                    type gsm apn 3gnet user uninet password uninet");
        proc->waitForStarted(-1);
        proc->waitForFinished(-1);
        qDebug() << "interface_gpio set : 1";

//        sleep(20);  //block exec
        QTime add = QTime::currentTime().addSecs(20);
        while(QTime::currentTime() < add)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }

    }
}

void udev::on_return_2_clicked()
{
    emit Mysignal();
}
