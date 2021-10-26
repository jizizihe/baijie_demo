#include "touchscreen.h"
#include "ui_touchscreen.h"

touchscreen::touchscreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::touchscreen)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_AcceptTouchEvents);
    setAttribute(Qt::WA_StaticContents);

    scribbleArea = new touch();
    setCentralWidget(scribbleArea);

    btnclear = new CTouchButton(this);
    btnclear->setText(tr("clear"));
    btnclear->setGeometry(780,480,100,50);

    btnreturn = new CTouchButton(this);
    btnreturn->setText(tr("return"));
    btnreturn->setGeometry(900,480,100,50);

    connect(btnclear,SIGNAL(clicked()),this,SLOT(screenclear()));
    connect(btnreturn,SIGNAL(clicked()),this,SLOT(btnreturn_clicked()));
}

touchscreen::~touchscreen()
{
    delete ui;
}

void touchscreen::btnreturn_clicked()
{
    scribbleArea->clearImage();
    emit Mysignal();
}

void touchscreen::screenclear()
{
    scribbleArea->clearImage();
}

void touchscreen::language_reload()
{
    btnclear->setText(tr("clear"));
    btnreturn->setText(tr("return"));
}
