#include "sim_module.h"
#include "ui_sim_module.h"

sim_module::sim_module(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sim_module)
{
    ui->setupUi(this);
}

sim_module::~sim_module()
{
    delete ui;
}
