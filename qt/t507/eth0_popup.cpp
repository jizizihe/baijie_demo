#include "eth0_popup.h"
#include <QRegExpValidator>
#include <QPushButton>

eth0_popup::eth0_popup(QWidget *parent) :
    QDialog(parent)
{
    resize(400,300);
    QPushButton *remainmenu = new QPushButton("OK",this);
    remainmenu->setGeometry(900,550,100,40);
//    edit = new QLineEdit(this);
//    edit->setGeometry(300,200,300,30);

    label_Name = new QLabel("name:",this);
    label_Name->setGeometry(300,200,130,40);
    line_name = new QLineEdit(this);
    line_name->setGeometry(450,200,200,40);

    label_addr = new QLabel("ip address:",this);
    label_addr->setGeometry(300,300,130,40);
    line_addr = new QLineEdit(this);
    line_addr->setGeometry(450,300,200,40);
    QRegExp a("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    line_addr->setValidator(new QRegExpValidator(a,this));

    label_gateway = new QLabel("gateway:",this);
    label_gateway->setGeometry(300,400,130,40);
    line_gateway = new QLineEdit(this);
    line_gateway->setGeometry(450,400,200,40);
    QRegExp b("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    line_gateway->setValidator(new QRegExpValidator(b,this));


    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    QLabel *label = new QLabel(this);
    label->setPalette(pe);
    label->setGeometry(20,545,860,50);
    label->setText("Please do not have spaces in the name, otherwise the operation will fail!");

    connect(remainmenu,SIGNAL(clicked(bool)),this,SLOT(remain()));
    connect(this,SIGNAL(addip()),this,SLOT(getaddipinforma()));
    connect(this,SIGNAL(modip()),this,SLOT(getmodipinforma()));
    connect(this,SIGNAL(delip()),this,SLOT(getdelipinforma()));
}

eth0_popup::~eth0_popup()
{

}

void eth0_popup::remain()
{
    if("add ip" == this->bnttype)
    {
        emit addipinfor(this->line_name->text(),this->line_addr->text(),this->line_gateway->text());
    }
    else if("mod ip" == this->bnttype)
    {
        emit modipinfor(this->line_name->text(),this->line_addr->text(),this->line_gateway->text());
    }
    else if("del ip" == this->bnttype)
    {
        emit delipinfor(this->line_name->text());
    }
}

void eth0_popup::getaddipinforma()
{
    label_addr->setVisible(true);
    label_gateway->setVisible(true);
    line_addr->setVisible(true);
    line_gateway->setVisible(true);

    label_Name->move(300,200);
    line_name->move(450,200);
//    label_Name = new QLabel("name:",this);
//    label_Name->setGeometry(300,200,130,40);
//    line_name = new QLineEdit(this);
//    line_name->setGeometry(450,200,200,40);

//    label_addr = new QLabel("ip address:",this);
//    label_addr->setGeometry(300,300,130,40);
//    line_addr = new QLineEdit(this);
//    line_addr->setGeometry(450,300,200,40);
//    QRegExp a("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
//    line_addr->setValidator(new QRegExpValidator(a,this));

//    label_gateway = new QLabel("gateway:",this);
//    label_gateway->setGeometry(300,400,130,40);
//    line_gateway = new QLineEdit(this);
//    line_gateway->setGeometry(450,400,200,40);
//    QRegExp b("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
//    line_gateway->setValidator(new QRegExpValidator(b,this));
}

void eth0_popup::getmodipinforma()
{
    label_addr->setVisible(true);
    label_gateway->setVisible(true);
    line_addr->setVisible(true);
    line_gateway->setVisible(true);

    label_Name->move(300,200);
    line_name->move(450,200);
//    label_Name = new QLabel("name:",this);
//    label_Name->setGeometry(300,200,130,40);
//    line_name = new QLineEdit(this);
//    line_name->setGeometry(450,200,200,40);

//    label_addr = new QLabel("ip address:",this);
//    label_addr->setGeometry(300,300,130,40);
//    line_addr = new QLineEdit(this);
//    line_addr->setGeometry(450,300,200,40);
//    QRegExp a("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
//    line_addr->setValidator(new QRegExpValidator(a,this));

//    label_gateway = new QLabel("gateway:",this);
//    label_gateway->setGeometry(300,400,130,40);
//    line_gateway = new QLineEdit(this);
//    line_gateway->setGeometry(450,400,200,40);
//    QRegExp b("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
//    line_gateway->setValidator(new QRegExpValidator(b,this));
}

void eth0_popup::getdelipinforma()
{
    label_addr->setVisible(false);
    label_gateway->setVisible(false);
    line_addr->setVisible(false);
    line_gateway->setVisible(false);

    //label_Name = new QLabel("name:",this);
    label_Name->move(300,300);
    //line_name = new QLineEdit(this);
    line_name->move(450,300);
}
