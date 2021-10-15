#include "timeset.h"
#include "ui_timeset.h"

timeset::timeset(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::timeset)
{
    ui->setupUi(this);
}

timeset::~timeset()
{
    delete ui;
}
