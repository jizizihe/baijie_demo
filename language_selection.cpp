#include "language_selection.h"
#include "ui_language_selection.h"

language_selection::language_selection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::language_selection)
{
    ui->setupUi(this);
}

language_selection::~language_selection()
{
    delete ui;
}
