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
}

serial_set_port2::~serial_set_port2()
{
    delete ui;
}

