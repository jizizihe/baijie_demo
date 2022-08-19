#include "ipset.h"
#include "ui_ipset.h"
#include <QScreen>

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;
static int open_flag;
static QString static_ip;

ipset::ipset(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ipset)
{
    ui->setupUi(this);
    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:25px;}");
    ui->stackedWidget->setCurrentIndex(0);
    screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();

    if(s_width < s_height)
    {
        screen_flag = 1;
    }
    ipset_font();

    QRegExp a("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    ui->ipAddrLineEdit->setValidator(new QRegExpValidator(a,this));

    //ui->networkSwitch->setCheckedColor(QColor(100, 225, 100));
   // connect(ui->networkSwitch,SIGNAL(toggled(bool)),this,SLOT(btnChangeFlag(bool)));

    QString strCmd = QString("ifconfig | grep eth0");
    QString retResult = executeLinuxCmd(strCmd);
    if(false == retResult.isEmpty())
    {
       // ui->networkSwitch->setToggle(true);
        ui->btn_open->setText(tr("close"));
        open_flag = 1;
    }
    else
    {
        //ui->networkSwitch->setToggle(false);
        ui->btn_open->setText(tr("open"));
        open_flag = 1;
    }
}

ipset::~ipset()
{
    delete ui;
}

//void ipset::btnChangeFlag(bool flag)
//{
//    //qDebug() << "LINE:" << __LINE__<<"switchflag " << switchflag;
//    flag = ui->networkSwitch->isToggled();

//    if(flag == 1) // open
//    {

//    }
//    else //close
//    {

//    }
//}

void ipset::language_reload()
{
    ui->retranslateUi(this);
}

void ipset::on_retBtn_clicked()
{
    emit ret_signal();
    ui->textEdit->setText("");
}

void ipset::on_ipShowBtn_clicked()
{
    QString networkInfo = get_network_info();
    ui->textEdit->setText(networkInfo);
    ui->stackedWidget->setCurrentIndex(0);
}

//void ipset::on_autoGetIpBtn_clicked()
//{
////    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << ":::::::::::::";

//    bool result;
//    ui->ipAddrLineEdit->clear();
//    QCoreApplication::processEvents();
//    if(get_current_ip("eth0") != "")
//    {
//        ui->textEdit->setText(tr("network is aready pull up!"));
//    }
//    else
//    {
//        result = automatically_get_ip();
//        if(result == true)
//        {
//            qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "result:"<<result;
//            QMessageBox mesg(QMessageBox::Information,
//                             tr("QMessageBox::information()"),
//                             tr("succeeded!"),
//                             0,this);
//            mesg.addButton(tr("OK"),QMessageBox::YesRole);
//            if(screen_flag == 1)
//            mesg.move(s_width*2/3,s_height/3);
//            else
//            mesg.move(s_width/3,s_height/3);
//            mesg.exec();
//        }
//        else
//        {
//            QMessageBox mesg(QMessageBox::Information,
//                             tr("QMessageBox::information()"),
//                             tr("failed!"),
//                             0,this);
//            mesg.addButton(tr("OK"),QMessageBox::YesRole);
//            if(screen_flag == 1)
//            mesg.move(s_width*2/3,s_height/3);
//            else
//            mesg.move(s_width/3,s_height/3);
//            mesg.exec();
//        }
//    }
//    ui->stackedWidget->setCurrentIndex(0);
//}

void ipset::on_setStaticIpBtn_clicked()
{
    ui->okBtn->setText(tr("ok"));
    ui->stackedWidget->setCurrentIndex(1);
}

void ipset::on_modStaticIpBtn_clicked()
{
    ui->ipAddrLineEdit->clear();

    if(false == is_static_ip_exist())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("The current connection is not a static IP. Please modify it to a static IP before performing this operation"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
        mesg.exec();
        QString networkInfo = get_network_info();
        ui->textEdit->setText(networkInfo);
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui->textEdit->setText("");
        ui->okBtn->setText(tr("change"));
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void ipset::on_delStaticIpBtn_clicked()
{
    ui->ipAddrLineEdit->clear();

    if(false == is_static_ip_exist())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("The current connection is auto IP!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
        mesg.exec();

        QString networkInfo = get_network_info();
        ui->textEdit->setText(networkInfo);
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }
    bool result = delete_static_ip();
    if(result == true)
    {
        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "result:"<<result;
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("set auto ip succeeded!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
        mesg.exec();
        QString networkInfo = get_network_info();
        ui->textEdit->setText(networkInfo);
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("set auto ip failed"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
        mesg.exec();
    }
}

void ipset::on_okBtn_clicked()
{
    bool result = false;
    QString networkInfo;
    if(ui->nameLineEdit->text().isEmpty() || ui->ipAddrLineEdit->text().isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please complete the information"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
        mesg.exec();

        return;
    }

    QMessageBox mesg(QMessageBox::Question,
                     tr("QMessageBox::question()"),
                     tr("Do you want to set it to current IP?"),
                     0,this);
     QPushButton *yesButton = mesg.addButton(tr("Yes"), QMessageBox::ActionRole);
     QPushButton *noButton = mesg.addButton(tr("No"),QMessageBox::ActionRole);
     if(screen_flag == 1)
     mesg.move(s_width*2/3,s_height/3);
     else
     mesg.move(s_width/3,s_height/3);
     mesg.exec();

       if (mesg.clickedButton() == yesButton) {
           if(ui->ipAddrLineEdit->text().length() <= 9)
           {
               QMessageBox::information(this,"information",tr("Format error, please re-enter"));
               return;
           }

           if(ui->okBtn->text() == QString(tr("ok")))
           {
               if(static_ip == ui->ipAddrLineEdit->text())
               {
                  result = modify_static_ip(ui->ipAddrLineEdit->text());
               }
               else
               {
                  result = add_static_ip(ui->nameLineEdit->text(),ui->ipAddrLineEdit->text());
               }
               static_ip = ui->ipAddrLineEdit->text();
           }
           else if(ui->okBtn->text() == QString(tr("change")))
           {
               result = modify_static_ip(ui->ipAddrLineEdit->text());
           }

           if(result == true)
           {
              // qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "result:"<<result;

               networkInfo = get_network_info();
               ui->textEdit->setText(networkInfo);
               ui->stackedWidget->setCurrentIndex(0);
           }
           else
           {
               QMessageBox mesg(QMessageBox::Information,
                                tr("QMessageBox::information()"),
                                tr("set static ip failed"),
                                0,this);
               mesg.addButton(tr("OK"),QMessageBox::YesRole);
               if(screen_flag == 1)
               mesg.move(s_width*2/3,s_height/3);
               else
               mesg.move(s_width/3,s_height/3);
               mesg.exec();
           }
       }
       else if (mesg.clickedButton() == noButton)
       {
           return;
       }
}

void ipset::on_backBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
} 

void ipset::ipset_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = s_width / realX * 2.54;
    qreal realHeight = s_height / realY *2.54;
    QFont font;
    if(screen_flag)
    {
        if(realHeight < 15)
        {
            font.setPointSize(12);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(14);
        }
        else
        {
            font.setPointSize(17);
        }

    }
    else
    {
        if(realWidth < 15)
        {
            font.setPointSize(12);
        }
        else if (realWidth < 17)
        {
            font.setPointSize(14);
        }
        else
        {
            font.setPointSize(17);
        }
    }
    ui->delStaticIpBtn->setFont(font);
    ui->ipShowBtn->setFont(font);
    ui->modStaticIpBtn->setFont(font);
    ui->setStaticIpBtn->setFont(font);
    ui->nameLineEdit->setFont(font);
    ui->ipAddrLineEdit->setFont(font);
    ui->label_6->setFont(font);
    ui->label_7->setFont(font);
    ui->textEdit->setFont(font);
    ui->backBtn->setFont(font);
    ui->okBtn->setFont(font);
}

void ipset::on_btn_open_clicked()
{
    if(open_flag == 0)
    {
        open_flag = 1;
        ui->btn_open->setText(tr("close"));
        network_enable(true);
        ui->ipShowBtn->setEnabled(true);
        ui->setStaticIpBtn->setEnabled(true);
        ui->modStaticIpBtn->setEnabled(true);
        ui->delStaticIpBtn->setEnabled(true);
    }
    else
    {
        open_flag = 0;
        ui->btn_open->setText(tr("open"));
        network_enable(false);
        ui->ipShowBtn->setEnabled(false);
        ui->setStaticIpBtn->setEnabled(false);
        ui->modStaticIpBtn->setEnabled(false);
        ui->delStaticIpBtn->setEnabled(false);
        ui->ipAddrLineEdit->clear();
    }
}
