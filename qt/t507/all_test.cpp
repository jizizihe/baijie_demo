#include "all_test.h"
#include "ui_all_test.h"

#include "all_test_interface.h"

QLabel *ll;
QMovie *w_movie;

all_test::all_test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::all_test)
{
    ui->setupUi(this);

    qRegisterMetaType<serial_config>("serial_config");
    serialConfig.count = 0;


    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();

    ll = new QLabel(this);
    //LoadLabel->resize(100,100);
    ll->move(screen_width/2 - 150,screen_height/2 -50);
    w_movie = new QMovie(":/t507_button_image/loading.webp");
    ll->setFixedSize(50, 50);
    ll->setScaledContents(true);
    ll->setMovie(w_movie);
    //pMovie->start();

    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:30px;}");
    serialStopTimer = new QTimer(this);
    connect(serialStopTimer,SIGNAL(timeout()),this,SLOT(serial_stop_deal()));
    serialDialog = new serialdialog(this);

    connect(serialDialog,SIGNAL(serial_config_msg(serial_config)),this,SLOT(serial_config_func(serial_config)));
    connect(serialDialog,SIGNAL(serial_test_msg()),this,SLOT(serial_test_func()));
    connect(serialDialog,SIGNAL(serial_dialog_stop_msg()),this,SLOT(serial_stop_deal()));
    connect(this,SIGNAL(serial_set_testBt_msg()),serialDialog,SLOT(serial_set_testBt_func()));


    testButtonGroup = new QButtonGroup(this);
    testButtonGroup->setExclusive(false);               //设置这个按钮组为非互斥模式

    testButtonGroup->addButton(ui->networkChk,0);
    testButtonGroup->addButton(ui->usbChk,1);
    testButtonGroup->addButton(ui->rtcChk,2);
    testButtonGroup->addButton(ui->sdcardChk,3);
    testButtonGroup->addButton(ui->keyChk,4);
    testButtonGroup->addButton(ui->serialChk,5);
    testButtonGroup->addButton(ui->cameraChk,6);
    testButtonGroup->addButton(ui->battaryChk,7);
    testButtonGroup->addButton(ui->simChk,8);
    testButtonGroup->addButton(ui->audioChk,9);
    testButtonGroup->addButton(ui->wifiChk,10);
    testButtonGroup->addButton(ui->bluetoothChk,11);
    CheckedBtnList = testButtonGroup->buttons();

    myThread = new QThread(this);
    allTestThread = new all_test_thread();
    allTestThread->moveToThread(myThread);

    myThread->start();

    btThread = new QThread(this);
    btTestThead = new all_test_thread();
    btTestThead->moveToThread(btThread);


    connect(this,SIGNAL(bluetooth_test_msg()),btTestThead,SLOT(bluetooth_test_thread()));

    connect(btTestThead,SIGNAL(send_test_msg(int ,QString )),this,SLOT(recv_test_msg(int, QString )));
    connect(allTestThread,SIGNAL(send_test_msg(int ,QString )),this,SLOT(recv_test_msg(int, QString )));
    connect(this,SIGNAL(network_test_msg()),allTestThread,SLOT(network_test_thread()));
    connect(this,SIGNAL(usb_test_msg(int)),allTestThread,SLOT(usb_test_thread(int)));
    connect(this,SIGNAL(rtc_test_msg()),allTestThread,SLOT(rtc_test_thread()));
    connect(this,SIGNAL(sdcard_test_msg()),allTestThread,SLOT(sdcard_test_thread()));
    connect(this,SIGNAL(camera_test_msg()),allTestThread,SLOT(camera_test_thread()));
    connect(this,SIGNAL(battary_test_msg()),allTestThread,SLOT(battary_test_thread()));
    connect(this,SIGNAL(sim_test_msg()),allTestThread,SLOT(sim_test_thread()));
    connect(this,SIGNAL(audio_test_msg()),allTestThread,SLOT(audio_test_thread()));
    connect(this,SIGNAL(wifi_test_msg()),allTestThread,SLOT(wifi_test_thread()));
//    connect(this,SIGNAL(bluetooth_test_msg()),allTestThread,SLOT(bluetooth_test_thread()));


}

all_test::~all_test()
{
    delete ui;

    myThread->quit();
    myThread->wait();
    delete allTestThread;
}

void all_test::language_reload()
{
    ui->retranslateUi(this);
}

void all_test::serial_config_func(serial_config config)
{
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__;

    serialConfig = config;
    if(serialConfig.count == 0)
    {
        serial_flag = false;
        ui->serialChk->setChecked(Qt::Unchecked);
    }
    else
    {
        serial_flag = true;
        ui->serialChk->setChecked(Qt::Checked);
    }
    //ui->serialChk->setChecked(Qt::Checked);
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "name:" << serialConfig.checkedName;
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "mode:" << serialConfig.mode;
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "num:" << serialConfig.count;

}

void all_test::serial_test_func()
{
    for(int i = 0; i < serialConfig.count;i++)
    {
        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__<< i;
        thread_id[i] = new QThread(this);
        serial_test_thread[i] = new all_test_thread();
        serial_test_thread[i]->moveToThread(thread_id[i]);

        if(i > 0)
        {
            disconnect(this,SIGNAL(serial_test_client_msg(serial_config)),serial_test_thread[i-1],SLOT(serial_test_thread_client(serial_config)));
        }
        connect(serial_test_thread[i],SIGNAL(send_test_msg(int ,QString )),this,SLOT(recv_test_msg(int, QString )));
        connect(this,SIGNAL(serial_test_client_msg(serial_config)),serial_test_thread[i],SLOT(serial_test_thread_client(serial_config)));

        serialConfig.index = i;

        if(thread_id[i]->isRunning() == true)
        {
            qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "------2525-----";
            return;
        }
        thread_id[i]->start();

        emit serial_test_client_msg(serialConfig);
    }

    if(serialConfig.mode == "client")
    {
//        sleep(2);
        serialStopTimer->start(2000);
    }
}

void all_test::serial_stop_deal()
{
    if(serialStopTimer->isActive() == true)
    {
        serialStopTimer->stop();
    }
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__;
    for(int i = 0; i < serialConfig.count; i++)
    {
        if(i > 0)
        {
            disconnect(this,SIGNAL(serial_test_stop_msg()),serial_test_thread[i-1],SLOT(serial_test_thread_stop()));
        }
        connect(this,SIGNAL(serial_test_stop_msg()),serial_test_thread[i],SLOT(serial_test_thread_stop()));

        if(thread_id[i]->isRunning() == true)
        {
            qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__;

            emit serial_test_stop_msg();
            thread_id[i]->quit();
            thread_id[i]->wait();
        }
//            delete serial_test_thread[i];
    }
    if(serialConfig.mode == "client")
    {
        emit serial_set_testBt_msg();
    }
}


void all_test::flagSet()
{
    //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__;

    int id = -1;
    QAbstractButton *checkBtn;
//    QList<QAbstractButton*> CheckedBtnList = testButtonGroup->buttons();

    for(int i =0 ;i<CheckedBtnList.length();i++)
    {
        checkBtn = CheckedBtnList.at(i);
        id = testButtonGroup->id(checkBtn);

        if(checkBtn->isChecked())
        {
            //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__ << checkBtn->text();

            switch (id) {
            case 0:network_flag = true; break;
            case 1:usb_flag = true;     break;
            case 2:rtc_flag = true;     break;
            case 3:sd_card_flag = true; break;
            case 4:key_flag = true;     break;
            case 5:serial_flag = true;  break;
            case 6:camera_flag = true;  break;
            case 7:battary_flag = true; break;
            case 8:sim_flag = true;     break;
            case 9:audio_flag = true;   break;
            case 10:wifi_flag = true;   break;
            case 11:bluetooth_flag = true;break;
            default:break;
            }
        }
        else
        {
            //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__ << checkBtn->text();
            switch (id) {
            case 0:network_flag = false; break;
            case 1:usb_flag = false;     break;
            case 2:rtc_flag = false;     break;
            case 3:sd_card_flag = false; break;
            case 4:key_flag = false;     break;
            case 5:serial_flag = false;  break;
            case 6:camera_flag = false;  break;
            case 7:battary_flag = false; break;
            case 8:sim_flag = false;     break;
            case 9:audio_flag = false;   break;
            case 10:wifi_flag = false;   break;
            case 11:bluetooth_flag = false;break;
            default:break;
            }
        }
    }
}

void all_test::on_begin_clicked()
{
    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"Thread构造函数ID:"<<QThread::currentThreadId();
    QAbstractButton *checkBtn;
    testItemsCount = 0;

    for(int i =0 ;i < CheckedBtnList.length();i++)
    {
        checkBtn = CheckedBtnList.at(i);
//        checkBtn->setEnabled(false);
        if(checkBtn->isChecked())
        {
            testItemsCount++;
            qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "testItemsCount:" << testItemsCount;
        }
    }

    if(ui->serialChk->isChecked() == true)
    {
        testItemsCount = testItemsCount - 1;
    }
    if(ui->keyChk->isChecked() == true)
        testItemsCount = testItemsCount - 1;
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "testItemsCount:" << testItemsCount;

    if(testItemsCount != 0)
    {
        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "******************************";
        ll->show();
        w_movie->start();
        ui->testCheckAllBtn->setEnabled(false);
        ui->begin->setEnabled(false);
        for(int i =0 ;i < CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setEnabled(false);
        }
    }

    ui->image->clear();
    ui->textEdit->setText("***********test start***********");
    startTime = QTime::currentTime();
    if(myThread->isRunning() == false)
    {
        myThread->start();
    }
    if(btThread->isRunning() == false)
    {
//        qDebug() << "------778------" << __LINE__;
        btThread->start();
    }

    if(bluetooth_flag)
    {
        emit bluetooth_test_msg();
        qDebug() << "blue_flag" << __LINE__;
    }
    if(audio_flag)
    {
        emit audio_test_msg();
        qDebug() << "audio_flag" << __LINE__;

        QProcess p;
        p.start("bash", QStringList() <<"-c" << "aplay /usr/test.wav");
        p.waitForFinished(-1);
//        QString strResult = p.readAllStandardOutput();
        p.close();
    }

    if(network_flag)
    {
        qDebug() << "network_flag" << __LINE__;
        emit network_test_msg();
    }
    if(ui->usbChk->isChecked() == true)
    {
        emit usb_test_msg(usbAddNum);
        qDebug() << "usb_flag" << __LINE__;
    }
    if(rtc_flag)
    {
        emit rtc_test_msg();
        qDebug() << "rtc_flag" << __LINE__;
    }
    if(sd_card_flag)
    {
        emit sdcard_test_msg();
        qDebug() << "sd_card_flag" << __LINE__;
    }
    if(key_flag)
    {
        qDebug() << "key_flag" << __LINE__;
        ui->textEdit->append("---please press the key to test!");
    }
    if(camera_flag)
    {
        camera_test_msg();
        qDebug() << "camera_flag" << __LINE__;
    }
    if(battary_flag)
    {
        emit battary_test_msg();
        qDebug() << "battary_flag" << __LINE__;
    }

    if(sim_flag)
    {
        emit sim_test_msg();
        qDebug() << "4G_flag" << __LINE__;
    }
    if(wifi_flag)
    {
        emit wifi_test_msg();
        qDebug() << "wifi_flag" << __LINE__;
    }

    if(serial_flag)
    {
        if(serialConfig.mode == "client")
        {
            serial_test_func();
        }
        qDebug() << "serial_flag" << __LINE__;
    }
}


bool all_test::event(QEvent *event)
{
    if(event->type() == QEvent::KeyPress)  //键盘按下处理,其他事件让事件处理器自己处理,不能返回false
    {
        if(key_flag == true)
        {
            qDebug()<<"-key pressed!";
            ui->textEdit->append(tr("-  key pressed!"));
            return true;
        }
        else
        {
            return QWidget::event(event);
        }
    }
    else
    {
        return QWidget::event(event);
    }
}


void all_test::on_testCheckAllBtn_clicked()
{
    QAbstractButton *checkBtn;

    if(ui->testCheckAllBtn->text() == "check all")
    {
        for(int i =0 ;i<CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setChecked(true);
        }
        flagSet();
        ui->testCheckAllBtn->setText("check none");
    }
    else if(ui->testCheckAllBtn->text() == "check none")
    {
        for(int i =0 ;i<CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setChecked(false);
        }
        flagSet();
        ui->testCheckAllBtn->setText("check all");
    }

}

void all_test::on_retBtn_clicked()
{
//    startTime = QTime::currentTime();
    serialDialog->close();
    emit Mysignal();
}

void all_test::checkbox_disenable_func(int itemsCount)
{
    QAbstractButton *checkBtn;
    ll->show();
    w_movie->start();
    ui->image->clear();
    ui->textEdit->clear();
    testItemsCount = itemsCount;
    ui->begin->setEnabled(false);
    ui->testCheckAllBtn->setEnabled(false);
    for(int i =0 ;i < CheckedBtnList.length();i++)
    {
        checkBtn = CheckedBtnList.at(i);
        checkBtn->setEnabled(false);
    }
    startTime = QTime::currentTime();
}


void all_test::on_networkChk_clicked()
{
    if(!network_flag)
    {
        network_flag = true;
        ui->networkChk->setChecked(Qt::Checked);
        checkbox_disenable_func(1);
        emit network_test_msg();
    }
    else
    {
        network_flag = false;
        ui->networkChk->setChecked(Qt::Unchecked);
    }
}

void all_test::on_usbChk_clicked()
{
    QStringList items; //ComboBox 列表的内容
    for(int i = 0;i < 50;i++)
    {
        items << QString::number(i);
    }
    QString dlgTitle="条目选择对话框";
    QString txtLabel="请选择添加个数";
    int     curIndex=0; //初始选择项
    bool    editable=false; //ComboBox是否可编辑
    bool    ok=false;
    QString num = 0;

    if(true == ui->usbChk->isChecked())
    {
        ui->usbChk->setChecked(Qt::Checked);

        num = QInputDialog::getItem(this, dlgTitle,txtLabel,items,curIndex,editable,&ok);
        if (ok && !num.isEmpty())
        {
            usbAddNum = num.toInt();
            qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__<< usbAddNum;
            checkbox_disenable_func(1);
            emit usb_test_msg(usbAddNum);
        }
        else
        {
            ui->usbChk->setChecked(Qt::Unchecked);
        }
    }
    else
    {
        ui->usbChk->setChecked(Qt::Unchecked);
    }

#if 0
    if(!usb_flag)
    {
//        usb_flag = true;
//        ui->usbChk->setChecked(Qt::Checked);

        checkbox_disenable_func(1);
        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__;
        emit usb_test_msg(usbAddNum);
    }
    else
    {
        usb_flag = false;
        ui->usbChk->setChecked(Qt::Unchecked);
    }
#endif
}

void all_test::on_rtcChk_clicked()
{
    if(!rtc_flag)
    {
        rtc_flag = true;
        ui->rtcChk->setChecked(Qt::Checked);

        checkbox_disenable_func(1);
        emit rtc_test_msg();
    }
    else
    {
        rtc_flag = false;
        ui->rtcChk->setChecked(Qt::Unchecked);
    }
}

void all_test::on_sdcardChk_clicked()
{
    if(!sd_card_flag)
    {
        sd_card_flag = true;
        ui->sdcardChk->setChecked(Qt::Checked);

        checkbox_disenable_func(1);
        emit sdcard_test_msg();
    }
    else
    {
        sd_card_flag = false;
        ui->sdcardChk->setChecked(Qt::Unchecked);
    }
}

void all_test::on_keyChk_clicked()
{
    if(!key_flag)
    {
        key_flag = true;
        ui->keyChk->setChecked(Qt::Checked);
    }
    else
    {
        key_flag = false;
        ui->keyChk->setChecked(Qt::Unchecked);
    }
}

void all_test::on_serialChk_clicked()
{
    if(!serial_flag)
    {
//        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__;
        serialDialog->exec();
        if(serialConfig.count == 0)
        {
            serial_flag = false;
            ui->serialChk->setChecked(Qt::Unchecked);
        }
    }
    else
    {
        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__;
        serial_flag = false;
        ui->serialChk->setChecked(Qt::Unchecked);
    }
    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__<<serial_flag;
}


void all_test::on_cameraChk_clicked()
{
    if(!camera_flag)
    {
        camera_flag = true;
        ui->cameraChk->setChecked(Qt::Checked);

        checkbox_disenable_func(1);
        emit camera_test_msg();
    }
    else
    {
        camera_flag = false;
        ui->cameraChk->setChecked(Qt::Unchecked);
    }
}

void all_test::on_battaryChk_clicked()
{
    if(!battary_flag)
    {
        battary_flag = true;
        ui->battaryChk->setChecked(Qt::Checked);

        checkbox_disenable_func(1);
        emit battary_test_msg();
    }
    else
    {
        battary_flag = false;
        ui->battaryChk->setChecked(Qt::Unchecked);
    }
}

void all_test::on_simChk_clicked()
{
    startTime = QTime::currentTime();

    if(!sim_flag)
    {
        sim_flag = true;
//        ui->simChk->setChecked(Qt::Checked);
        checkbox_disenable_func(1);
        emit sim_test_msg();
    }
    else
    {
        sim_flag = false;
//        ui->simChk->setChecked(Qt::Unchecked);
    }
}

void all_test::on_audioChk_clicked()
{
    if(!audio_flag)
    {
        audio_flag = true;
        ui->audioChk->setChecked(Qt::Checked);
        checkbox_disenable_func(1);

        emit audio_test_msg();

        QProcess p;
        p.start("bash", QStringList() <<"-c" << "aplay /usr/test.wav");
        p.waitForFinished();
//        QString strResult = p.readAllStandardOutput();
        p.close();
    }
    else
    {
        audio_flag = false;
        ui->audioChk->setChecked(Qt::Unchecked);
    }
}

void all_test::on_wifiChk_clicked()
{
    startTime = QTime::currentTime();

    if(!wifi_flag)
    {
        wifi_flag = true;
        ui->wifiChk->setChecked(Qt::Checked);
        checkbox_disenable_func(1);

        emit wifi_test_msg();
    }
    else
    {
        wifi_flag = false;
        ui->wifiChk->setChecked(Qt::Unchecked);
    }
}

void all_test::on_bluetoothChk_clicked()
{
    startTime = QTime::currentTime();

    if(!bluetooth_flag)
    {
        bluetooth_flag = true;
        ui->bluetoothChk->setChecked(Qt::Checked);

        checkbox_disenable_func(1);
        btThread->start();
        emit bluetooth_test_msg();
    }
    else
    {
        bluetooth_flag = false;
        ui->bluetoothChk->setChecked(Qt::Unchecked);
    }
}

void all_test::testMsgDisplay(QString type,QString str,int time)
{
    QString dispStr;

    if(str == "OK")
    {
        dispStr = type + QString("OK! %1s").arg(time);
        ui->textEdit->append(dispStr);
    }
    else
    {
        dispStr = QString("%1%2--- %3s").arg(type).arg(str).arg(time);
        ui->textEdit->append(dispStr);
    }
}

void all_test::recv_test_msg(int test_signal_type, QString str)
{
    QImage image;
    QAbstractButton *checkBtn;
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__<< str;

    if(test_signal_type != serial_signal)
    {
        testItemsCount--;
        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__<< "count:"<< testItemsCount;
    }
    if(testItemsCount == 0)
    {
        ui->testCheckAllBtn->setEnabled(true);
        ui->begin->setEnabled(true);
        w_movie->stop();
        ll->close();
        for(int i =0 ;i < CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setEnabled(true);
        }
    }

    switch (test_signal_type) {
    case network_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime.netwotk ="<<elapsed<<"s";
        testMsgDisplay("---network test:",str,elapsed);

        break;
    case usb_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime.rtc ="<<elapsed<<"s";
        testMsgDisplay("---usb test:    ",str,elapsed);
        break;
    case rtc_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime.rtc ="<<elapsed<<"s";
        testMsgDisplay("---rtc test:    ",str,elapsed);

        break;
    case sd_card_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime.sdcard ="<<elapsed<<"s";
        testMsgDisplay("---sdcard test: ",str,elapsed);
        break;
    case key_signal:
        break;
    case serial_signal:
//        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime.Serial";
//        testMsgDisplay("---serial test:",str,0);

        str = QString(tr("---serial test: %1")).arg(str);
        ui->textEdit->append(str);
        break;
    case camera_signal:
        image = QImage("/data/yuv.jpg");
        ui->image->setScaledContents(true);
        ui->image->setPixmap(QPixmap::fromImage(image));
        ui->image->show();

        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:camera ="<<elapsed<<"s";

        str = QString(tr("---camera test: Please check the picture display? %1s")).arg(elapsed);
        ui->textEdit->append(str);

        break;
    case battary_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:battary ="<<elapsed<<"s";
        testMsgDisplay("---battary test:",str,elapsed);
        break;
    case sim_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:sim ="<<elapsed<<"s";
        testMsgDisplay("---4G test:     ",str,elapsed);
        break;
    case audio_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:audio ="<<elapsed<<"s";

        str = QString(tr("---audio test:  please sure sound play test yes or no? %1s")).arg(elapsed);
        ui->textEdit->append(str);
//        myThread->quit();
//        myThread->wait();

        break;
    case wifi_signal:
        stopTime = QTime::currentTime();

        elapsed = startTime.secsTo(stopTime);
        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:wifi ="<<elapsed<<"s";
        testMsgDisplay("---wifi test:   ",str,elapsed);

        break;
    case bluetooth_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:bluetooth ="<<elapsed<<"s";
        testMsgDisplay("---bluetooth test:",str,elapsed);
//        btThread->quit();
//        btThread->wait();
//        w_movie->stop();
//        ll->close();
//        ui->begin->setEnabled(true);

        break;
    default:
        break;
    }

//    if(btThread->isFinished() && myThread->isFinished())
//    {
//        w_movie->stop();
////        ll->clear();

//        ui->begin->setEnabled(true);
//        myThread->start();
//    }

}



