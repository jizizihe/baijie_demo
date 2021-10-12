#include "udev.h"
#include "ui_udev.h"

#include <QDebug>
#include <QProcess>

udev::udev(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::udev)
{
    ui->setupUi(this);
}

udev::~udev()
{
    delete ui;
}

void udev::on_pushButton_3_clicked()
{
    emit Mysignal();
}

void udev::on_usbdete_clicked()
{
    QProcess *pro = new QProcess();

    pro->start("lsusb");
}

void udev::on_simdete_clicked()
{

}
