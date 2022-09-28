#include "bluetooth.h"
#include "ui_bluetooth.h"
#include <QScreen>
#include <QProcess>
#include <QTimer>
#include <QListWidgetItem>

using namespace std;
static QScreen *screen;
static int screen_flag;
static int Width;
static int Height;
static int open_flag;
static QString BtAddress;
static QString Btname;
static QString Btcon;
static QTimer *timer;
static QTimer *timer_scan;
static QString connect_flag;
static int scan_flag;
static int scan_first;
static int show_flag;
static int pair_flag;
static int connecting_flag;

bluetooth::bluetooth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bluetooth)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    Width = screen->size().width();			//屏幕宽
    Height = screen->size().height();
    if(Width < Height)
    {
        screen_flag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    open_flag = 1;
    wifi_bt_t = new wifi_bt_interface(this);
    blue_font();

    LoadLabel = new QLabel(this);
    if(screen_flag == 1)
    {

    }
    else
    {
      LoadLabel->move(Width/2 - 150,Height/2 -50);
    }
    pMovie = new QMovie("://button_image/loading.webp");
    LoadLabel->setFixedSize(50, 50);
    LoadLabel->setScaledContents(true);
    LoadLabel->setMovie(pMovie);

    myThread = new QThread(this);
    BluetoothThread = new bluetooth_thread();
    timer = new QTimer(this);
    timer_scan = new QTimer(this);
    connect(timer_scan,SIGNAL(timeout()),this,SLOT(scan_refresh()));
    connect(timer,SIGNAL(timeout()),this,SLOT(connect_refresh()));
    connect(this,SIGNAL(bluetooth_scan_msg()),BluetoothThread,SLOT(bluetooth_scan_thread()));
    connect(this,SIGNAL(bluetooth_pair_msg(QString)),BluetoothThread,SLOT(bluetooth_pair_thread(QString)));
    connect(this,SIGNAL(bluetooth_connect_msg(QString)),BluetoothThread,SLOT(bluetooth_connect_thread(QString)));
    connect(this,SIGNAL(rm_shell()),wifi_bt_t,SLOT(rm_bluetoothshell()));
    connect(BluetoothThread,SIGNAL(send_msg(int, QString)),this,SLOT(recv_msg(int, QString)));

    BluetoothThread->moveToThread(myThread);
    myThread->start();
    LoadLabel->show();
    //LoadLabel->move(this->width()/2,this->height()/2);
    pMovie->start();
    ui->stackedWidget->setCurrentIndex(0);
    ui->BtNameListWidget->setDisabled(true);
    emit bluetooth_scan_msg();
    ui->BTScanBtn->setText("stop");

    scan_flag = 2;Btcon = "Btname";
}

bluetooth::~bluetooth()
{
    delete ui;
    delete BluetoothThread;
    delete myThread;

}

void bluetooth::BtnChange_flag(bool btflag)
{
    btflag = open_flag;

    if(btflag == 1) // open
    {
        wifi_bt_t->bluetooth_enable(true);

//        if(ui->BtNameListWidget->count() == 0)
//        {
//            LoadLabel->show();
//            pMovie->start();
//            emit bluetooth_signal(1,0);
//        }

        for(int i = 0; i < ui->BtNameListWidget->count(); i++)
        {
            ui->BtNameListWidget->setItemHidden(ui->BtNameListWidget->item(i), false);
        }
        ui->BTScanBtn->setDisabled(false);
        ui->BtNameListWidget->setDisabled(false);
    }
    else //close
    {
        wifi_bt_t->bluetooth_enable(false);

        for(int i = 0; i < ui->BtNameListWidget->count(); i++)
        {
            ui->BtNameListWidget->setItemHidden(ui->BtNameListWidget->item(i), true);
        }
        pMovie->stop();
        LoadLabel->hide();
        ui->BTScanBtn->setDisabled(true);
        ui->BtNameListWidget->setDisabled(true);
    }

}

void bluetooth::recv_msg(int signal_type,QString str)
{
    int flag;
    if(signal_type == Bt_scan_signal)
    {
        if(pair_flag == 1)
        {
            scan_first = 0;
            return;
        }
        if(connecting_flag == 1)
        {
            scan_first = 0;
            return;
        }
        ui->BtNameListWidget->clear();
        flag = open_flag;
        if(flag == 0)
            return ;
        BtScanList.clear();
        ui->BtNameListWidget->clear();
        BtPairList = database_w.table_show("bluetooth");
        QString name,status;
        QStringList paried_list1 = BtPairList;
        if(!BtPairList.isEmpty())
        {
            for(int i = 0;i<paried_list1.size();i++)
            {
                name = paried_list1.at(i);
                if(!QString::compare(Btcon,name,Qt::CaseSensitive))
                {
                    status = tr("connect");
                    bluetooth_listshow(name,status);
                    paried_list1.removeAt(i);
                    paried_list1.removeAt(i);
                    break;
                }
                i++;
            };
            for(int i = 0;i<paried_list1.size();i++)
            {
                name = paried_list1.at(i);
                status = "saved";
                i++;
                bluetooth_listshow(name,status);
            }
        }
        QStringList Btscan1 = str.split("\n"); int num=0;
        BtScanList = Btscan1;
        BtScanList.removeAll(QString(""));BtScanList.removeAll(QString("\t"));
        BtPairList = database_w.table_show("bluetooth");

        for(int i = 0; i < Btscan1.size(); i++)
        {
            QString tmp = Btscan1.at(i);
            QString tmp_address = tmp.trimmed().section("\t",0,0);
            tmp = tmp.trimmed().section("\t",1,1);
            if(BtPairList.size()<= 0)
            {
                bluetooth_listshow(tmp,"");
                //ui->BtNameListWidget->addItem(tmp);
            }
            else
            {
                for(int j = 0; j < BtPairList.size(); j++)
                {
                    if(tmp_address == QString(BtPairList.at(j+1)))
                    {
                        int num1 = 0;
                        if(num > 0)
                        {
                            num1 = i-num;
                        }
                        else
                        {
                            num1 = i;
                        }
                        num++;
                        BtScanList.removeAt(num1);
                        break;
                    }
                    else
                    {
                        j++;
                        if(j+1 == BtPairList.size())
                            bluetooth_listshow(tmp,"");
                       // ui->BtNameListWidget->addItem(tmp);
                    }

                }
            }
        }
       // ui->BtNameListWidget->setCurrentRow(0);
//        ui->BtNameListWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:40px;}");
//        ui->BtNameListWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{height:30px;}");
//        ui->BtNameListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        if(scan_first == 1)
        {
            if(scan_flag == 2)
            {
                ui->BtNameListWidget->setEnabled(true);
                pMovie->stop();
                LoadLabel->hide();
                ui->BTScanBtn->setText("scan");
                scan_flag = 1;
                if(show_flag == 1)
                timer_scan->start(3000);
            }
            else
            {
                if(pair_flag == 1)
                {
                    scan_first = 0;
                    return;
                }
               // emit bluetooth_scan_msg();
                timer_scan->start(2000);
            }
        }
        else
        {
            if(scan_flag == 0)
            {
                emit bluetooth_scan_msg();
            }
            else
            {
                ui->BtNameListWidget->setEnabled(true);
                pMovie->stop();
                LoadLabel->hide();
                ui->BTScanBtn->setText("scan");
                if(show_flag == 1)
                timer_scan->start(3000);
            }
        }
    }
    else if(signal_type == Bt_pair_signal)
    {
        if(str == "successful")
        {
            pair_flag = 0;
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("pair success!"),
                             0,this);
             mesg.addButton(tr("OK"), QMessageBox::ActionRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             else
             mesg.move(Width/3,Height/3);
             mesg.exec();
             int BtNameIndex = ui->BtNameListWidget->currentRow();
             BtNameIndex = BtNameIndex-BtPairList.size()/2;
             BtScanList.removeAt(BtNameIndex);
             database_w.insert_table2("bluetooth",Btname,BtAddress);
             ui->BtNameListWidget->clear();
             BtPairList = database_w.table_show("bluetooth");
             QString name,status;
             if(!BtPairList.isEmpty())
             {

                 for(int i = 0;i<BtPairList.size();i++)
                 {
                     name = BtPairList.at(i);
                     status = "saved";
                     i++;
                     bluetooth_listshow(name,status);
                 }
             }

             for(int i = 0; i < BtScanList.size(); i++)
             {
                 QString tmp = BtScanList.at(i);
                 QString tmp_address = tmp.trimmed().section("\t",0,0);
                 tmp = tmp.trimmed().section("\t",1,1);
                 if(BtPairList.size()<= 0)
                 {
                     bluetooth_listshow(tmp,"");
                 }
                 else
                 {
                     for(int j = 0; j < BtPairList.size(); j++)
                     {
                         if(tmp_address == QString(BtPairList.at(j+1)))
                         {
                             break;
                         }
                         else
                         {
                             j++;
                             if(j+1 == BtPairList.size())
                                 bluetooth_listshow(tmp,"");
                         }

                     }
                 }
             }
             timer_scan->start(3000);
        }
        else if(str == "failed")
        {
            pair_flag = 0;
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("pair failed!"),
                             0,this);
             mesg.addButton(tr("OK"), QMessageBox::ActionRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             else
             mesg.move(Width/3,Height/3);
             mesg.exec();
             timer_scan->start(3000);
        }
        ui->BTScanBtn->setEnabled(true);
        ui->BtNameListWidget->setEnabled(true);

        pMovie->stop();
        LoadLabel->hide();
    }
    else if(signal_type == Bt_connect_signal)
    {
        timer_scan->stop();
        if(str == "successful")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("connect success!"),
                             0,this);
             mesg.addButton(tr("OK"), QMessageBox::ActionRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             else
             mesg.move(Width/3,Height/3);
             mesg.exec();

            // int row = ui->BtNameListWidget->currentRow();
             BtPairList = database_w.table_show("bluetooth");
            // row=row*2;
            // Btcon = BtPairList.at(row);
             ui->BtNameListWidget->clear();
             ui->status_label->setText(tr("connect"));
             ui->bus_label->setText(Btname);
             Btcon=Btname;
             ui->stackedWidget->setCurrentIndex(1);
             ui->stackedWidget_2->setCurrentIndex(1);
             bluetooth_status();
             QString name,status;
             if(!BtPairList.isEmpty())
             {
                 QStringList pair_scanlist = BtPairList;
                 for(int i = 0;i<pair_scanlist.size();i++)
                 {
                     name = pair_scanlist.at(i);
                     if(!QString::compare(Btcon,name,Qt::CaseSensitive))
                     {
                         status = "connect";
                         bluetooth_listshow(name,status);
                         pair_scanlist.removeAt(i);
                         pair_scanlist.removeAt(i);
                         break;
                     }
                     i++;
                 }
                 for(int i = 0;i<pair_scanlist.size();i++)
                 {
                     name = pair_scanlist.at(i);
                     status = "saved";
                     i++;
                     bluetooth_listshow(name,status);
                 }
             }
             for(int i = 0; i < BtScanList.size(); i++)
             {
                 QString tmp = BtScanList.at(i);
                 QString tmp_address = tmp.trimmed().section("\t",0,0);
                 tmp = tmp.trimmed().section("\t",1,1);
                 if(BtPairList.size()<= 0)
                 {
                     bluetooth_listshow(tmp,"");
                     //ui->BtNameListWidget->addItem(tmp);
                 }
                 else
                 {
                     for(int j = 0; j < BtPairList.size(); j++)
                     {
                         if(tmp_address == QString(BtPairList.at(j+1)))
                         {
                             break;
                         }
                         else
                         {
                             j++;
                             if(j+1 == BtPairList.size())
                                 bluetooth_listshow(tmp,"");
                         }

                     }
                 }
             }
             connecting_flag = 0;
        }
        else if(str == "failed")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("connect failed!"),
                             0,this);
             mesg.addButton(tr("OK"), QMessageBox::ActionRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             else
             mesg.move(Width/3,Height/3);
             mesg.exec();

             ui->BtNameListWidget->clear();
             BtPairList = database_w.table_show("bluetooth");
             QString name,status;
             if(!BtPairList.isEmpty())
             {
                 for(int i = 0;i<BtPairList.size();i++)
                 {
                     name = BtPairList.at(i);
                     status = "saved";
                     i++;
                     bluetooth_listshow(name,status);
                 }
             }
             for(int i = 0; i < BtScanList.size(); i++)
             {
                 QString tmp = BtScanList.at(i);
                 QString tmp_address = tmp.trimmed().section("\t",0,0);
                 tmp = tmp.trimmed().section("\t",1,1);
                 if(BtPairList.size()<= 0)
                 {
                     bluetooth_listshow(tmp,"");
                 }
                 else
                 {
                     for(int j = 0; j < BtPairList.size(); j++)
                     {
                         if(tmp_address == QString(BtPairList.at(j+1)))
                         {
                             break;
                         }
                         else
                         {
                             j++;
                             if(j+1 == BtPairList.size())
                                 bluetooth_listshow(tmp,"");
                         }

                     }
                 }
             }
             connecting_flag = 0;
             timer_scan->start(3000);
        }
        else if(str == "connected")
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("please disconnect the \"%1\" device first!").arg(Btcon),
                             0,this);
             mesg.addButton(tr("OK"), QMessageBox::ActionRole);
             if(screen_flag == 1)
             mesg.move(Width*2/3,Height/3);
             else
             mesg.move(Width/3,Height/3);
             mesg.exec();
        }
        ui->BTScanBtn->setEnabled(true);
        ui->BtNameListWidget->setEnabled(true);

        pMovie->stop();
        LoadLabel->hide();
    }
}

void bluetooth::on_retBtn_clicked()
{
    scan_first = 0;
    show_flag = 0;
    emit Mysignal();
    timer->stop();
    emit rm_shell();
}

void bluetooth::on_BTScanBtn_clicked()
{
    timer->stop();
    if(open_flag == 0)
    {
        QMessageBox::information(this,"information",tr("Please open Bluetooth first!"));
        return;
    }
    int ind = ui->stackedWidget->currentIndex();
    if(ind == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
        timer_scan->start(3000);
        return;
    }

    if(!LoadLabel->isHidden())
    {
        ui->BtNameListWidget->setDisabled(false);
        LoadLabel->hide();
        pMovie->stop();
       // wifi_bt_t->executeLinuxCmd("kill");
        ui->BTScanBtn->setText(tr("scan"));
        scan_flag = 1;
        //emit bluetooth_scan_msg();
        return;
    }

    ui->BtNameListWidget->setDisabled(true);
    LoadLabel->show();
    //LoadLabel->move(this->width()/2,this->height()/2);
    pMovie->start();
    emit bluetooth_scan_msg();
    //scan_flag = 0;
    ui->BTScanBtn->setText(tr("stop"));
    scan_flag = 0;
    scan_first = 0;
}

//void bluetooth::on_BTConnectBtn_clicked()
//{
//    scan_first = 0;
//    if(open_flag == 0)
//    {
//        QMessageBox::information(this,"information",tr("Please open Bluetooth first!"));
//        return;
//    }
//    ui->stackedWidget->setCurrentIndex(1);
//    if(!Btcon.isEmpty())
//    {
//      ui->stackedWidget_2->setCurrentIndex(1);
//      timer->stop();
//    }
//    else
//    {
//       ui->stackedWidget_2->setCurrentIndex(0);
//       Btcon = "";
//       //bluetooth_sql();
//       timer->start(3000);
//    }
//    QString tmp;
//    QString strResult = wifi_bt_t->executeLinuxCmd("hciconfig");
//    QStringList list = strResult.split("\n");
//    list.removeAll("");
//    for(int i=0; i<list.size();i++)
//    {
//        QString str = list.at(i);
//        if(str.contains("Type",Qt::CaseInsensitive))
//        {
//            tmp = str.section(' ',1,1);
//            ui->type_label->setText(tmp);
//           // tmp = str.section(' ',4,4);
//           // ui->bus_label->setText(tmp);
//        }
//        else if(str.contains("Address",Qt::CaseInsensitive))
//        {
//            tmp = str.section(' ',2,2);
//            ui->address_label->setText(tmp);
//        }
//    }
//    if(!ui->status_label->text().contains(tr("connect"),Qt::CaseInsensitive))
//    {
//        ui->status_label->setText(tr("not connect"));
//    }

//}

void bluetooth::language_reload()
{
    ui->retranslateUi(this);
}

void  bluetooth::blue_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = Width / realX * 2.54;
    qreal realHeight = Height / realY *2.54;
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

    ui->BTScanBtn->setFont(font);
    ui->label_2->setFont(font);
    ui->pushButton->setFont(font);
    ui->BtDeviceNameLab->setFont(font);
    ui->btn_disconnect->setFont(font);
    ui->btn_remove->setFont(font);
    ui->label->setFont(font);
    ui->label_3->setFont(font);
    ui->label_4->setFont(font);
    ui->label_5->setFont(font);
    ui->address_label->setFont(font);
    ui->bus_label->setFont(font);
    ui->type_label->setFont(font);
    ui->status_label->setFont(font);
    ui->label_6->setFont(font);
}

void bluetooth::on_pushButton_clicked()
{
    if(open_flag == 0)
    {
        open_flag = 1;
        ui->pushButton->setText(tr("close"));
       // bluetooth_sql();
        ui->BtNameListWidget->setDisabled(true);
        LoadLabel->show();
        //LoadLabel->move(this->width()/2,this->height()/2);
        pMovie->start();
        emit bluetooth_scan_msg();
        ui->BTScanBtn->setText("stop");
        timer_scan->start(3000);
    }
   else
   {
        scan_first = 0;
        open_flag = 0;
        LoadLabel->hide();
        pMovie->stop();
        ui->pushButton->setText(tr("open"));
        ui->BtNameListWidget->clear();
        ui->stackedWidget->setCurrentIndex(0);
        timer->stop();
        if(connect_flag != "1")
        {
            wifi_bt_t->bluetooth_disconnect(BtAddress);
            ui->bus_label->setText(tr("no"));
            ui->stackedWidget_2->setCurrentIndex(0);
            Btcon = "";
            //bluetooth_sql();
            ui->status_label->setText(tr("not connect"));
        }
    }
}

//void bluetooth::bluetooth_sql()
//{
//    ui->Bt_pairedListwidget->clear();
//    BtPairList = database_w.table_show("bluetooth");

//    for(int i = 0;i<BtPairList.size();i++)
//    {
//        QString tmp = BtPairList.at(i);
//        QHBoxLayout *horLayout = new QHBoxLayout();
//        horLayout->setContentsMargins(10,0,0,0);
//        QWidget *widget=new QWidget(this);
//        QLabel *nameLabel = new QLabel(widget);
//        QLabel *connectLabel = new QLabel(widget);
//        QListWidgetItem *item = new QListWidgetItem();
//        nameLabel->setText(tmp);
//        if(!QString::compare(Btcon,tmp,Qt::CaseSensitive))
//        {
//            connectLabel->setText(tr("connect"));
//        }
//        else
//        {
//            connectLabel->setText(tr("not connect"));
//        }
//        horLayout->addWidget(nameLabel);
//        horLayout->addWidget(connectLabel);
//        widget->setLayout(horLayout);
//        horLayout->setStretchFactor(nameLabel,3);
//        horLayout->setStretchFactor(connectLabel,1);
//        horLayout->setSpacing(0);
//        ui->Bt_pairedListwidget->addItem(item);
//        ui->Bt_pairedListwidget->setItemWidget(item, widget);
//        i++;
//    }

//     ui->Bt_pairedListwidget->setCurrentRow(0);
//}

void bluetooth::bluetooth_listshow(QString name,QString status)
{
        QHBoxLayout *horLayout = new QHBoxLayout();
        horLayout->setContentsMargins(5,0,0,0);
        QWidget *widget=new QWidget(this);
        QLabel *nameLabel = new QLabel(widget);
        QLabel *connectLabel = new QLabel(widget);
        QListWidgetItem *item = new QListWidgetItem();
        nameLabel->setText(name);
        connectLabel->setText(tr(status.toUtf8()));
        horLayout->addWidget(nameLabel);
        horLayout->addWidget(connectLabel);
        widget->setLayout(horLayout);
        horLayout->setStretchFactor(nameLabel,3);
        horLayout->setStretchFactor(connectLabel,1);
        horLayout->setSpacing(0);
        ui->BtNameListWidget->addItem(item);
        ui->BtNameListWidget->setItemWidget(item, widget);
        ui->BtNameListWidget->setCurrentRow(0);
        ui->BtNameListWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:40px;}");
        ui->BtNameListWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{height:30px;}");
        ui->BtNameListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void bluetooth::showEvent(QShowEvent *event)
{
    ui->stackedWidget->setCurrentIndex(0);
    QString name = wifi_bt_t->get_bluetooth_name();
    ui->BtDeviceNameLab->setText(name);
    show_flag = 1;timer_scan->start(3000);
    QWidget::showEvent(event);
}

void bluetooth::on_BtNameListWidget_itemClicked(QListWidgetItem *item)
{
    QListWidgetItem *item1 = ui->BtNameListWidget->currentItem();
    QWidget* pwig = ui->BtNameListWidget->itemWidget(item1); // 获取里面的QWidget
    QList<QLabel*> labelList = pwig->findChildren<QLabel*>();  // 获取所有的Qlabel
    QString status = labelList.at(1)->text();
    QString name = labelList.at(0)->text();
    if(status == tr("connect"))
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(1);
        return;
    }
    BtPairList = database_w.table_show("bluetooth");
    int BtNameIndex = ui->BtNameListWidget->currentRow();
    if(!BtPairList.isEmpty())
    {
        if(BtPairList.size()/2>BtNameIndex)
        {
            connect_refresh();
            if(connect_flag != "1")
            {
                QString address = database_w.select_by_name("bluetooth",connect_flag);
                wifi_bt_t->bluetooth_disconnect(address);
                ui->status_label->setText(tr("not connect"));
                ui->bus_label->setText(tr("no"));
                ui->stackedWidget->setCurrentIndex(0);
                Btcon = "";
            }
            connecting_flag = 1;Btname = name;
            BtAddress = database_w.select_by_name("bluetooth",name);qDebug() << BtAddress;
            LoadLabel->show();
            pMovie->start();
            QString strCmd = QString("ps -x|grep pulseaudio | grep -v grep |wc -l");
            QString strResult = wifi_bt_t->executeLinuxCmd(strCmd);

            if(strResult == QString("0\n"))
            {
                strCmd = QString("pulseaudio --start");
                strResult = wifi_bt_t->executeLinuxCmd(strCmd);
            }
            emit bluetooth_connect_msg(BtAddress);

            ui->BTScanBtn->setDisabled(true);
            return;
        }
    }
    pair_flag = 1;
    BtNameIndex = BtNameIndex-BtPairList.size()/2;
    BtAddress = BtScanList.at(BtNameIndex);
    Btname = BtAddress.trimmed().section("\t",1,1);
    BtAddress = BtAddress.trimmed().section("\t",0,0);
    ui->BTScanBtn->setDisabled(true);
    LoadLabel->show();
    pMovie->start();

    emit bluetooth_pair_msg(BtAddress);
}

//void bluetooth::on_Bt_pairedListwidget_itemClicked(QListWidgetItem *item)
//{
//    if(!Btcon.isEmpty())
//    {
//        QMessageBox mesg(QMessageBox::Information,
//                         tr("QMessageBox::information()"),
//                         tr("please disconnect the \"%1\" device first!").arg(Btcon),
//                         0,this);
//        mesg.addButton(tr("OK"), QMessageBox::ActionRole);
//        if(screen_flag == 1)
//            mesg.move(Width*2/3,Height/3);
//        else
//            mesg.move(Width/3,Height/3);
//        mesg.exec();
//        return;
//    }
//    LoadLabel->show();
//    pMovie->start();
//    QString strCmd = QString("ps -x|grep pulseaudio | grep -v grep |wc -l");
//    QString strResult = wifi_bt_t->executeLinuxCmd(strCmd);

//    if(strResult == QString("0\n"))
//    {
//        strCmd = QString("pulseaudio --start");
//        strResult = wifi_bt_t->executeLinuxCmd(strCmd);
//    }
//    int BtNameIndex = ui->Bt_pairedListwidget->currentRow();
//    if(BtNameIndex != 0)
//    {
//        BtNameIndex = BtNameIndex*2;
//    }
//    Btname = BtPairList.at(BtNameIndex);
//    BtAddress = BtPairList.at(BtNameIndex+1);
//    // qDebug() << Btname; qDebug() << BtAddress;
//    emit bluetooth_connect_msg(BtAddress);

//    ui->BTScanBtn->setDisabled(true);
//    ui->BTConnectBtn->setDisabled(true);
//}

void bluetooth::connect_refresh()
{
    connect_flag = wifi_bt_t->bluetooth_connectflag();
    if(connect_flag == "1")
    {
        Btcon = "";
        ui->bus_label->setText(tr("no"));
        ui->status_label->setText(tr("not connect"));
    }
    else
    {
      // Btcon = connect_flag;qDebug() << 11;
    }
   // bluetooth_sql();
}

//void bluetooth::on_btn_status_clicked()
//{
//    ui->stackedWidget_2->setCurrentIndex(0);
//    connect_refresh();
//    timer->start(3000);
//}

void bluetooth::on_btn_disconnect_clicked()
{
    if(Btcon.isEmpty())
    {
        QMessageBox::information(this,"information",tr("No device is connected!"));
    }
    else
    {
        wifi_bt_t->bluetooth_disconnect(BtAddress);
        QMessageBox::information(this,"information",tr("Disconnect successful!"));
        ui->status_label->setText(tr("not connect"));
        ui->bus_label->setText(tr("no"));
        timer->stop();timer_scan->start(3000);
        ui->stackedWidget->setCurrentIndex(0);
        Btcon = "";
        QString name,status;
        ui->BtNameListWidget->clear();
        BtPairList = database_w.table_show("bluetooth");
        if(!BtPairList.isEmpty())
        {
            for(int i = 0;i<BtPairList.size();i++)
            {
                name = BtPairList.at(i);
                status = "saved";
                i++;
                bluetooth_listshow(name,status);
            }
        }
        for(int i = 0; i < BtScanList.size(); i++)
        {
            QString tmp = BtScanList.at(i);
            tmp = tmp.trimmed().section("\t",1,1);
            bluetooth_listshow(tmp,"");
        }
    }
}

void bluetooth::on_btn_remove_clicked()
{
    QString address = database_w.select_by_name("bluetooth",Btname);
    wifi_bt_t->bluetooth_remove(address);
    QMessageBox::information(this,"information",tr("Remove successful!"));
    database_w.delete_record_by_name("bluetooth",Btname);
    ui->status_label->setText(tr("not connect"));
    ui->bus_label->setText(tr("no"));


    ui->BtNameListWidget->clear();
    BtPairList = database_w.table_show("bluetooth");
    QString name,status;
    if(!BtPairList.isEmpty())
    {
        for(int i = 0;i<BtPairList.size();i++)
        {
            name = BtPairList.at(i);
            status = "saved";
            i++;
            bluetooth_listshow(name,status);
        }
    }
    for(int i = 0; i < BtScanList.size(); i++)
    {
        QString tmp = BtScanList.at(i);
        tmp = tmp.trimmed().section("\t",1,1);
        bluetooth_listshow(tmp,"");
    }


    timer->stop();timer_scan->start(3000);
    ui->stackedWidget->setCurrentIndex(0);
}

void bluetooth::scan_refresh()
{
    if(pair_flag == 1)
    {
        scan_first = 0;
         timer_scan->stop();
        return;
    }
    if(connecting_flag == 1)
    {
        scan_first = 0;
         timer_scan->stop();
        return;
    }
  if(ui->stackedWidget->currentIndex() == 0)
  {
      scan_first=1;
      emit bluetooth_scan_msg();
      timer_scan->stop();
  }
}

void bluetooth::bluetooth_status()
{
    QString tmp;
    QString strResult = wifi_bt_t->executeLinuxCmd_bluetooth_connect("hciconfig");
    QStringList list = strResult.split("\n");
    list.removeAll("");
    for(int i=0; i<list.size();i++)
    {
        QString str = list.at(i);
        if(str.contains("Type",Qt::CaseInsensitive))
        {
            tmp = str.section(' ',1,1);
            ui->type_label->setText(tmp);
        }
        else if(str.contains("Address",Qt::CaseInsensitive))
        {
            tmp = str.section(' ',2,2);
            ui->address_label->setText(tmp);
        }
    }
    if(!ui->status_label->text().contains(tr("connect"),Qt::CaseInsensitive))
    {
        ui->status_label->setText(tr("not connect"));
    }
}
