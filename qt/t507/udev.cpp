#include "udev.h"
#include "ui_udev.h"

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
