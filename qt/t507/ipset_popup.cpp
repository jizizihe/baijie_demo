#include "ipset_popup.h"
#include <QMessageBox>
ipset_popup::ipset_popup(QWidget *parent) :
    QDialog(parent)
{
//    list = QNetworkInterface::allAddresses();
//    for(int i=0;i<list.count();i++)
//    {
//     //只留192.168.1段
//        if( list.at(i).toString().contains("192.168.0."))
//        {
//            qDebug()<< list.at(i).toString();
//        }
//     }

    btn_OK = new QPushButton(tr("OK"),this);
    btn_OK->setGeometry(900,550,100,40);

    btn_return = new QPushButton(this);
    btn_return->setIcon(QIcon(":/t507_button_image/return.png"));
//    btn_return = new QPushButton(tr("return"),this);
    btn_return->setGeometry(10,10,100,40);

    label_netcard = new QLabel(tr("network card:"),this);
    label_netcard->setGeometry(270,200,180,40);
    line_netcard = new QLineEdit(this);
    line_netcard->setGeometry(450,200,200,40);

    label_addr = new QLabel(tr("ip address:"),this);
    label_addr->setGeometry(270,350,130,40);
    line_addr = new QLineEdit(this);
    line_addr->setGeometry(450,350,200,40);
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
    connect(this,SIGNAL(addip()),this,SLOT(getaddipinforma()));
    connect(this,SIGNAL(modip()),this,SLOT(getmodipinforma()));
}

ipset_popup::~ipset_popup()
{

}

void ipset_popup::remain()
{
    if("add ip" == this->bnttype)
    {
        if(this->line_addr->text().isEmpty() || this->line_netcard->text().isEmpty())
        {
            QMessageBox::warning(NULL,NULL,tr("Please complete the information"),tr("OK"));
            return;
        }
        int a = QMessageBox::question(NULL,NULL,tr("Do you want to set it to current IP?"),tr("yes"),tr("no"));
        if(a == 0)
        {
            emit addipinfor(this->line_netcard->text(),this->line_addr->text());       
        }
        else
        {
            return;
        }

    }
    else if("mod ip" == this->bnttype)
    {
        if(this->line_addr->text().isEmpty())
        {
            QMessageBox::warning(NULL,NULL,tr("Please complete the information"),tr("OK"));
            return;
        }
        int a = QMessageBox::question(NULL,NULL,tr("Do you want to set it to current IP?"),tr("yes"),tr("no"));
        if(a == 0)
        {
            emit modipinfor(this->line_addr->text());
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }

    screen_clear();
}

void ipset_popup::returnmain()
{
    emit go_back();
    screen_clear();
}

void ipset_popup::getaddipinforma()
{
    label_addr->setVisible(true);
    label_netcard->setVisible(true);
    line_addr->setVisible(true);
    line_netcard->setVisible(true);

    label_netcard->move(270,200);
    line_netcard->move(450,200);
    label_addr->move(270,350);
    line_addr->move(450,350);
}

void ipset_popup::getmodipinforma()
{
    label_addr->setVisible(true);
    label_netcard->setVisible(false);
    line_addr->setVisible(true);
    line_netcard->setVisible(false);

    label_addr->move(270,300);
    line_addr->move(450,300);
}

void ipset_popup::screen_clear()
{
    line_addr->setText("");
    line_netcard->setText("");
}

void ipset_popup::language_reload()
{

    label->setText(tr("Please do not have spaces in the name, otherwise the operation will fail!"));
    btn_OK->setText(tr("OK"));
//    btn_return->setText(tr("return"));
    label_netcard->setText(tr("network card:"));
    label_addr->setText(tr("ip address:"));

}
