#include "udev.h"
#include "ui_udev.h"
#include "interface_gpio.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QTime>
#include <QMessageBox>

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

    QFile file(file_name);

    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << array;
    file.flush();
    file.close();;

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

        for(int x = 0; x < i; x++)
        {
            array[x] = array[x].remove(0,28).left(4);
            if(array[x] == "0001" || array[x] == "0125" || array[x] == "0101" || array[x] == "0002")
            {
                flag = false;
            }
            else
            {
                flag = true;
                break;
            }
        }
        if(flag)
            ui->usb_lineEdit->setText("Find the new USB device");
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
                break;
            }
        }
        if(flag)
            ui->sd_lineEdit->setText("Find the new Sd card");
        else
            ui->sd_lineEdit->setText("New Sd card is not found");

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
                break;
            }
        }
        if(flag)
            ui->sim_lineEdit->setText("Find the new SIM card");
        else
            ui->sim_lineEdit->setText("New SIM card is not found");
        qDebug() << "ifconfig***********>>>";
    }
    file.flush();
    file.close();
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
    if(temp == "No new SIM card found")
    {
        QMessageBox::information(NULL,NULL, QString("Please note the 4G LED on the board when it appears fast flashing!!"));
        file_name = "";
        qDebug() << "OH No!!!";
        interface_gpio(0);
        qDebug() << "interface_gpio set : 0";
        proc->start("nmcli connection delete ppp0");
        proc->waitForStarted(-1);
        proc->waitForFinished(-1);
        sleep(2);
        interface_gpio(1);
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
