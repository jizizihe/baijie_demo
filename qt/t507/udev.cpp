#include "udev.h"
#include "ui_udev.h"
#include "udev_interface.h"
#include "gpio_interface.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QTime>
#include <QMessageBox>
#include <QPalette>

static int sim_flag;

udev::udev(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::udev)
{
    sim_flag = 0;
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
//    qDebug() << "<<<<<-------------------"<< file_name <<"-------------------->>>>>>";
    QString out_f = QString("*****************************************************************");

    array = proc->readAllStandardOutput().data();

    ui->message->setText(array);

    QFile file(file_name);

    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << array;
    file.flush();
    file.close();

    static bool flag = false;


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
            ui->message->append(QString(("%1\nThe New USB device is:\n%2%3")).arg(out_f).arg(temp.data()).arg(out_f));
            ui->usb_label->setText(tr("Find the New USB device"));
        }
        else
            ui->usb_label->setText(tr("New USB device is not found"));

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
            if("/dev/mmcblk1p1" == array[x].left(14))
            {
                flag = true;
                i = x;
                break;
            }
        }


        if(flag)
        {
            ui->message->append(QString(("%1\nThe New SD card is :\n%2%3\n")).arg(out_f).arg(array[i].data()).arg(out_f));
            ui->sd_label->setText(tr("Find the New Sd card"));
        }
        else
            ui->sd_label->setText(tr("New SD card is not found"));

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
            if(array[x] == "ppp0")
            {
                flag = true;
                sim_flag = 0;
                i = x;
                break;
            }
        }

        if(flag)
        {
            ui->message->append(QString(("%1\nThe New SIM card is:\n4G_IP:%2%3")).arg(out_f).arg(array[i+1].data()).arg(out_f));
            ui->sim_label->setText(tr("Find the New SIM card"));
        }
        else
        {
            ++sim_flag;
            ui->sim_label->setText(tr("New SIM card is not found"));
        }

    }
    file.flush();
    file.close();
}

void udev::on_usb_detection_clicked()
{
    ui->usb_label->setText("");

    file_name = QString("/data/1.txt");

    proc->start(QString("rm %1").arg(file_name));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);

    proc->start(QString("touch %1").arg(file_name));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);

    proc->start("lsusb");

//    QString status = get_new_usb();
//    if(status == "USB OK")
//    {

//    }
//    else
//    {

//    }
}

void udev::on_sd_detection_clicked()
{
    ui->sd_label->setText("");

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

    ui->sim_label->setText(" ");

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

    QString temp = ui->sim_label->text();

    if(temp == "New SIM card is not found")
    {
        QMessageBox::information(NULL,NULL, QString(tr("That may be a long time!!Please note the 4G LED on the board when it appears fast flashing!!")));
        file_name = "";

        int gpio_port = calc_port_num('h',12);
        gpio_export(gpio_port);
        gpio_set_state(gpio_port,"out");
        gpio_set_value(gpio_port,0);

        proc->start("nmcli connection delete ppp0");
        proc->waitForStarted(-1);
        proc->waitForFinished(-1);
        sleep(1);

        gpio_set_value(gpio_port,1);

        sleep(1);

        proc->start("nmcli connection add con-name ppp0 ifname ttyUSB2 autoconnect yes \
                    type gsm apn 3gnet user uninet password uninet");
        proc->waitForStarted(-1);
        proc->waitForFinished(-1);

//        QTime add = QTime::currentTime().addSecs(20);
//        while(QTime::currentTime() < add)
//        {
//            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
//        }

    }
}

void udev::on_return_2_clicked()
{
    emit Mysignal();
}

void udev::language_reload()
{
    ui->retranslateUi(this);
}
