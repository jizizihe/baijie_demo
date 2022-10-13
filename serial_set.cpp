#include "serial_set.h"
#include "ui_serial_set.h"
#include <QDebug>
#include <QScreen>

serial_set::serial_set(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serial_set)
{
    ui->setupUi(this);
}

serial_set::~serial_set()
{
    delete ui;
}


