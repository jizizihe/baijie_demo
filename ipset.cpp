#include "ipset.h"
#include "ui_ipset.h"
#include <QScreen>

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;
static int open_flag;
static QString static_ip;
static int btn_up_flag;

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
        screen_flag = 1;ui->line_2->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    ipset_font();

    QRegExp a("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    ui->ipAddrLineEdit->setValidator(new QRegExpValidator(a,this));
    ui->gatewaylineEdit->setValidator(new QRegExpValidator(a,this));
    ui->nameLineEdit->setReadOnly(true);
    ui->masklineEdit->setPlaceholderText("0-32");

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
    ui->widget->setEnabled(false);
    ui->widget->setStyleSheet("border-radius: 12px;padding: 6px;"
                              "background-color: rgb(217, 217, 217,30);border:1px solid gray;");
    ui->okBtn->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                             "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                             "padding: 6px;outline: none; ");
    ui->modStaticIpBtn->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                                      "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                                      "padding: 6px;outline: none; ");
    ui->nameLineEdit->setStyleSheet("color: rgb(186, 189, 182);");
    ui->masklineEdit->setStyleSheet("color: rgb(186, 189, 182);");
    ip_settext();
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
    if(open_flag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open the ethernet!"));
        return;
    }
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
    if(open_flag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open the ethernet!"));
        return;
    }
    //ui->okBtn->setText(tr("up"));
    ui->stackedWidget->setCurrentIndex(1);
}

void ipset::on_modStaticIpBtn_clicked()
{
    ui->ipAddrLineEdit->clear();
    ui->gatewaylineEdit->clear();
    ui->masklineEdit->clear();
}

void ipset::on_delStaticIpBtn_clicked()
{
    if(open_flag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open the ethernet!"));
        return;
    }
    ui->widget->setEnabled(false);
    ui->widget->setStyleSheet("border-radius: 12px;padding: 6px;"
                              "background-color: rgb(217, 217, 217,30);border:1px solid gray;");
    ui->okBtn->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                             "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                             "padding: 6px;outline: none; ");
    ui->modStaticIpBtn->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                                      "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                                      "padding: 6px;outline: none; ");
    ui->nameLineEdit->setStyleSheet("color: rgb(186, 189, 182);");
    ui->masklineEdit->setStyleSheet("color: rgb(186, 189, 182);");
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
        network_enable(true);
        return;
    }

    bool result = delete_static_ip();
    if(result == true)
    {
        //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "result:"<<result;
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
       // database_w.delete_record_by_name("ip_static",ui->ipAddrLineEdit->text());
        ui->ipAddrLineEdit->clear();
        ui->gatewaylineEdit->clear();
        ui->masklineEdit->clear();
        ui->okBtn->setText(tr("up"));btn_up_flag = 0;
        network_enable(true);
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
    if(ui->nameLineEdit->text().isEmpty() || ui->ipAddrLineEdit->text().isEmpty() || ui->gatewaylineEdit->text().isEmpty() || ui->masklineEdit->text().isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please complete the information"),
                         0,this);
        mesg.addButton(tr("up"),QMessageBox::YesRole);
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

    if(mesg.clickedButton() == yesButton)
    {
        if((ui->ipAddrLineEdit->text().length() <= 9) || (ui->gatewaylineEdit->text().length() <= 9))
        {
            QMessageBox::information(this,"information",tr("Format error, please re-enter."));
            return;
        }
        int mask = ui->masklineEdit->text().toInt();
        if((mask > 32)||(mask < 0))
        {
            QMessageBox::information(this,"information",tr("Please enter the correct subnet mask."));
            return;
        }
        if((ui->ipAddrLineEdit->text().length() <= 9) || (ui->gatewaylineEdit->text().length() <= 9))
        {
            QMessageBox::information(this,"information",tr("Format error, please re-enter"));
            return;
        }

//        if(btn_up_flag == 0)
//        {
//            if(static_ip == ui->ipAddrLineEdit->text())
//            {
//                result = modify_static_ip(ui->ipAddrLineEdit->text());
//            }
//            else
//            {
                result = add_static_ip(ui->nameLineEdit->text(),ui->ipAddrLineEdit->text(),ui->masklineEdit
                                       ->text(),ui->gatewaylineEdit->text());
//            }
//            static_ip = ui->ipAddrLineEdit->text();
//        }
//        else if(btn_up_flag == 1)
//        {
//            result = modify_static_ip(ui->ipAddrLineEdit->text());
//        }

        if(result == true)
        {
            networkInfo = get_network_info();
            ui->textEdit->setText(networkInfo);
            ui->stackedWidget->setCurrentIndex(0);
           // QStringList list = database_w.table_show("ip_static");
          //  if(!list.isEmpty())
          //  {
          //      QString ipstr = list.at(0);
          //      database_w.delete_record_by_name("ip_static",ipstr);
          //  }
          //  database_w.insert_table1("ip_static",ui->ipAddrLineEdit->text());
            //              QString str = ui->okBtn->text();
            //               if(!QString::compare(str,QString("up"),Qt::CaseSensitive))
            //               {
            //                   ui->okBtn->setText(tr("change"));
            //               }
            if(btn_up_flag == 0)
            {  btn_up_flag = 1;ui->okBtn->setText(tr("change"));}
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
    ui->okBtn->setFont(font);
    ui->btn_open->setFont(font);
    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->gatewaylineEdit->setFont(font);
    ui->masklineEdit->setFont(font);
    ui->pushButton->setFont(font);
}

void ipset::on_btn_open_clicked()
{
    if(open_flag == 0)
    {
        open_flag = 1;
        ui->btn_open->setText(tr("close"));
        network_enable(true);
        QString networkInfo = get_network_info();
        ui->textEdit->setText(networkInfo);
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        open_flag = 0;
        ui->btn_open->setText(tr("open"));
        network_enable(false);
        ui->textEdit->setText("");
    }
}

void ipset::ip_settext()
{
    if(true == is_static_ip_exist())
    {
        QString strCmd = QString("ip route show");
        QString strResult = executeLinuxCmd(strCmd);
        strResult = strResult.section(" ",2,2);
        ui->gatewaylineEdit->setText(strResult);
        strCmd = QString("ifconfig eth0 | awk '{print $2}'|awk 'NR==2'");
        strResult = executeLinuxCmd(strCmd);
        ui->ipAddrLineEdit->setText(strResult);
        ui->widget->setEnabled(true);
        ui->widget->setStyleSheet("border-radius: 12px;padding: 6px;background-color:rgba(255, 255, 255, 80);"
                                  "border:1px solid gray;");
        ui->okBtn->setStyleSheet("background-color: rgba(100, 225, 100, 120);border-style: outset;border-width:  2px;"
                                 "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(0, 0, 0, 100);"
                                 "padding: 6px;outline: none; ");
        ui->modStaticIpBtn->setStyleSheet("background-color: rgba(100, 225, 100, 120);border-style: outset;border-width:  2px;"
                                          "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(0, 0, 0, 100);"
                                          "padding: 6px;outline: none; ");
        ui->nameLineEdit->setStyleSheet("color: rgb(0, 0, 0);");
        ui->masklineEdit->setStyleSheet("color: rgb(0, 0, 0);");
//        QString gateway;

//        for(int i = 9;i < 12;i++)
//        {
//            if(strResult.at(i) == '.')
//            {
//                gateway = strResult.left(i+1);
//                gateway.append("1");
//                break;
//            }
//        }
//        qDebug() << gateway;
    }
}

void ipset::on_pushButton_clicked()
{
    ui->widget->setEnabled(true);
    ui->widget->setStyleSheet("border-radius: 12px;padding: 6px;background-color:rgba(255, 255, 255, 60);"
                              "border:1px solid gray;");
    ui->okBtn->setStyleSheet("background-color: rgba(100, 225, 100, 120);border-style: outset;border-width:  2px;"
                             "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(0, 0, 0, 100);"
                             "padding: 6px;outline: none; ");
    ui->modStaticIpBtn->setStyleSheet("background-color: rgba(100, 225, 100, 120);border-style: outset;border-width:  2px;"
                                      "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(0, 0, 0, 100);"
                                      "padding: 6px;outline: none; ");
    ui->nameLineEdit->setStyleSheet("color: rgb(0, 0, 0);");
    ui->masklineEdit->setStyleSheet("color: rgb(0, 0, 0);");
}
