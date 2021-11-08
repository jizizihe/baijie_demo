#include "all_interface_test.h"
#include "ui_all_interface_test.h"

#include "udev_interface.h"
#include "gpio_interface.h"

#include <unistd.h>

#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QProcess>


void camera_interface()
{
    QProcess *proc = new QProcess();

    proc->start("bash", QStringList() << "-c" << "rm /data/yuv.jpg");
    proc->waitForFinished(-1);

    proc->start("bash",QStringList() << "-c" << "cd /data && csi_test_mplane");
    proc->waitForFinished(-1);

    proc->close();
    delete proc;
    proc = 0;

}


all_interface_test::all_interface_test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::all_interface_test)
{
    ui->setupUi(this);

}

all_interface_test::~all_interface_test()
{
    delete ui;
}


void all_interface_test::on_return_2_clicked()
{
    ui->textEdit->clear();
    ui->image->clear();
    emit Mysignal();
}

void all_interface_test::on_test_clicked()
{
    ui->textEdit->clear();
    ui->textEdit->append(tr("1. Note that the buzzer has or does not have sound!!"));

    int gpio_port = calc_port_num('b',8);
    gpio_export(gpio_port);
    gpio_set_state(gpio_port,"out");
    gpio_set_value(gpio_port,1);
    sleep(1);
    gpio_set_value(gpio_port,0);
    gpio_unexport(gpio_port);


    if(get_new_usb() == tr("USB Failed"))
        ui->textEdit->append(QString(tr("%1 %2")).arg("2.").arg(get_new_usb()));
    else
        ui->textEdit->append(tr("2. USB OK"));

    if(get_new_sd() == tr("SD Failed"))
        ui->textEdit->append(QString(tr("%1 %2")).arg("3.").arg(get_new_sd()));
    else
        ui->textEdit->append(tr("3. SD OK"));

    if(get_new_sim() == tr("SIM Failed"))
        ui->textEdit->append(QString(tr("%1 %2")).arg("4.").arg(get_new_sim()));
    else
        ui->textEdit->append(tr("4. SIM OK"));


    camera_interface();
    QImage image = QImage("/data/yuv.jpg");
    ui->image->setScaledContents(true);
    ui->image->setPixmap(QPixmap::fromImage(image));
    ui->image->show();
    ui->textEdit->append(tr("5. Whether the picture is displayed properly"));

}

void all_interface_test::on_clear_clicked()
{
    ui->textEdit->clear();
    ui->image->clear();
}

void all_interface_test::on_pushButton_3_clicked()
{
    QProcess *proc = new QProcess();

    proc->start("bash", QStringList() << "-c" << "rm /data/yuv.jpg");
    proc->waitForFinished();

    proc->start("bash",QStringList() << "-c" << "cd /data && csi_test_mplane");
    proc->waitForFinished();

    proc->close();
    delete proc;
    proc = 0;
}


void all_interface_test::on_pushButton_4_clicked()
{
    QPixmap pix;
    QImage image = QImage("/data/yuv.jpg");
//    image->load("./*.jpg");
    ui->image->setPixmap(pix.fromImage(image));
    ui->image->setScaledContents(true);
    ui->image->show();
}

void all_interface_test::language_reload()
{
    ui->retranslateUi(this);
}
