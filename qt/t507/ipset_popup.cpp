#include "ipset_popup.h"
#include <QMessageBox>
ipset_popup::ipset_popup(QWidget *parent) :
    QDialog(parent)
{
    btn_OK = new QPushButton(tr("OK"),this);
    btn_OK->setGeometry(900,550,100,40);

    btn_return = new QPushButton(tr("return"),this);
    btn_return->setGeometry(10,10,100,40);

    label_Name = new QLabel(tr("name:"),this);
    label_Name->setGeometry(270,200,130,40);
    line_name = new QLineEdit(this);
    line_name->setGeometry(450,200,200,40);

    label_netcard = new QLabel(tr("network card:"),this);
    label_netcard->setGeometry(270,300,180,40);
    line_netcard = new QLineEdit(this);
    line_netcard->setGeometry(450,300,200,40);

    label_addr = new QLabel(tr("ip address:"),this);
    label_addr->setGeometry(270,400,130,40);
    line_addr = new QLineEdit(this);
    line_addr->setGeometry(450,400,200,40);
    QRegExp a("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    line_addr->setValidator(new QRegExpValidator(a,this));

    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    label = new QLabel(this);
    label->setPalette(pe);
    label->setGeometry(20,545,860,50);
    label->setText(tr("Please do not have spaces in the name, otherwise the operation will fail!"));

    connect(btn_OK,SIGNAL(clicked(bool)),this,SLOT(remain()));
    connect(btn_return,SIGNAL(clicked(bool)),this,SLOT(returnmain()));
    connect(this,SIGNAL(setdip()),this,SLOT(getsetdipinforma()));
    connect(this,SIGNAL(addip()),this,SLOT(getaddipinforma()));
    connect(this,SIGNAL(modip()),this,SLOT(getmodipinforma()));
    connect(this,SIGNAL(delip()),this,SLOT(getdelipinforma()));
}

ipset_popup::~ipset_popup()
{

}

void ipset_popup::remain()
{
    if("set dynamic ip" == this->bnttype)
    {
        if(this->line_netcard->text().isEmpty())
        {
            QMessageBox::warning(NULL,NULL,tr("Please complete the information"),tr("OK"));
            return;
        }

        int a = QMessageBox::question(NULL,NULL,tr("Are you sure you want to set this connection to get IP dynamically?"),tr("yes"),tr("no"));
        if(a == 0)
        {
            emit setdyninfor(this->line_netcard->text());
        }
        else
        {
            return;
        }
    }
    else if("add ip" == this->bnttype)
    {
        if(this->line_addr->text().isEmpty() || this->line_name->text().isEmpty() || this->line_netcard->text().isEmpty())
        {
            QMessageBox::warning(NULL,NULL,tr("Please complete the information"),tr("OK"));
            return;
        }
        int a = QMessageBox::question(NULL,NULL,tr("Do you want to set it to current IP?"),tr("yes"),tr("no"));
        if(a == 0)
        {
            emit addipinfor(this->line_name->text(),this->line_netcard->text(),this->line_addr->text());
        }
        else
        {
            return;
        }

    }
    else if("mod ip" == this->bnttype)
    {
        if(this->line_addr->text().isEmpty() || this->line_name->text().isEmpty())
        {
            QMessageBox::warning(NULL,NULL,tr("Please complete the information"),tr("OK"));
            return;
        }
        int a = QMessageBox::question(NULL,NULL,tr("Do you want to set it to current IP?"),tr("yes"),tr("no"));
        if(a == 0)
        {
            emit modipinfor(this->line_name->text(),this->line_addr->text());
        }
        else
        {
            return;
        }

    }
    else if("del ip" == this->bnttype)
    {
        if(this->line_name->text().isEmpty())
        {
            QMessageBox::warning(NULL,NULL,tr("Please complete the information"),tr("OK"));
            return;
        }

        int a = QMessageBox::question(NULL,NULL,tr("Are you sure you want to delete this connection?"),tr("yes"),tr("no"));
        if(a == 0)
        {
            emit delipinfor(this->line_name->text());
        }
        else
        {
            return;
        }
    }
    screen_clear();
}

void ipset_popup::returnmain()
{
    emit go_back();
    screen_clear();
}

void ipset_popup::getsetdipinforma()
{
    label_addr->setVisible(false);
    label_Name->setVisible(false);
    label_netcard->setVisible(true);
    line_addr->setVisible(false);
    line_name->setVisible(false);
    line_netcard->setVisible(true);

    label_netcard->move(270,300);
    line_netcard->move(450,300);
}

void ipset_popup::getaddipinforma()
{
    label_Name->setVisible(true);
    label_addr->setVisible(true);
    label_netcard->setVisible(true);
    line_name->setVisible(true);
    line_addr->setVisible(true);
    line_netcard->setVisible(true);

    label_Name->move(270,200);
    line_name->move(450,200);
    label_netcard->move(270,300);
    line_netcard->move(450,300);
    label_addr->move(270,400);
    line_addr->move(450,400);
}

void ipset_popup::getmodipinforma()
{
    label_Name->setVisible(true);
    label_addr->setVisible(true);
    label_netcard->setVisible(false);
    line_name->setVisible(true);
    line_addr->setVisible(true);
    line_netcard->setVisible(false);

    label_Name->move(270,200);
    line_name->move(450,200);
    label_addr->move(270,300);
    line_addr->move(450,300);
}

void ipset_popup::getdelipinforma()
{
    label_Name->setVisible(true);
    label_addr->setVisible(false);
    label_netcard->setVisible(false);
    line_name->setVisible(true);
    line_addr->setVisible(false);
    line_netcard->setVisible(false);

    label_Name->move(270,300);
    line_name->move(450,300);
}

void ipset_popup::screen_clear()
{
    line_name->setText("");
    line_addr->setText("");
    line_netcard->setText("");
}

void ipset_popup::language_reload()
{

    label->setText(tr("Please do not have spaces in the name, otherwise the operation will fail!"));
    btn_OK->setText(tr("OK"));
    btn_return->setText(tr("return"));
    label_Name->setText(tr("name:"));
    label_netcard->setText(tr("network card:"));
    label_addr->setText(tr("ip address:"));

}
