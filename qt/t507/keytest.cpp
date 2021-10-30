#include "keytest.h"
#include "ui_keytest.h"

keytest::keytest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::keytest)
{
    ui->setupUi(this);
//    this->resize(800,600);

    retBt = new QPushButton(this);
    retBt->setFixedSize(100,40);
//    retBt->setText(tr("return"));
    retBt->setIcon(QIcon(":/t507_button_image/return.png"));
    retBt->move(10,10);

    KeyLabel = new QLabel(this);
    KeyLabel->setText(tr("key Test"));
    KeyLabel->resize(200,100);
    KeyLabel->move(400,20);
    QFont ft;
    ft.setPointSize(18);
    KeyLabel->setFont(ft);

    StartBt = new QPushButton(this);
    StartBt->setText(tr("start"));
    StartBt->resize(200,50);
    StartBt->move(50,200);

    StopBt = new QPushButton(this);
    StopBt->setText(tr("stop"));
    StopBt->resize(200,50);
    StopBt->move(50,300);

    keyText = new QTextEdit(this);
    keyText->resize(500,400);
    keyText->move(400,150);

    connect(retBt,SIGNAL(clicked(bool)),this,SLOT(retBt_clicked()));
    connect(StartBt,SIGNAL(clicked(bool)),this,SLOT(startBt_clicked()));
    connect(StopBt,SIGNAL(clicked(bool)),this,SLOT(stopBt_clicked()));
    connect(task,SIGNAL(message(QString)),this,SLOT(recvmsg(QString)));
}

keytest::~keytest()
{
    delete ui;
}

void keytest::retBt_clicked()
{
    emit Mysignal();
}

void keytest::startBt_clicked()
{
    qDebug() << "startBt_clicked!";
    this->keyText->append(tr("start test!"));
    this->task->start();

}

void keytest::stopBt_clicked()
{
    this->keyText->append(tr("stop test!"));
    this->task->terminate();
    this->task->wait();
}

void keytest::recvmsg(QString str)
{
    qDebug() << str;
    this->keyText->append(tr("key pressed!"));
}

void keytest::language_reload()
{
//    retBt->setText(tr("return"));
    KeyLabel->setText(tr("key Test"));
    StartBt->setText(tr("start"));
    StopBt->setText(tr("stop"));

}
