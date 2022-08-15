#include "openfile.h"
#include "ui_openfile.h"

openfile::openfile(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::openfile)
{
    ui->setupUi(this);
}

openfile::~openfile()
{
    delete ui;
}
