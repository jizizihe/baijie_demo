#include "all_test.h"
#include "ui_all_test.h"
#include "all_test_interface.h"
#include <QDebug>

static QScreen *screen;
static int screen_flag;
static int Width;
static int Height;
//static int camera_flag;

all_test::all_test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::all_test)
{
    ui->setupUi(this);

    qRegisterMetaType<serial_config>("serial_config");
    serialConfig.count = 0;

    screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    int screen_width = mm.width();
    int screen_height = mm.height();

    Width = screen->size().width();			//屏幕宽
    Height = screen->size().height();
    if(Width < Height)
    {
        screen_flag = 1;
    }

    all_font();
    waitLbl = new QLabel(this);
    waitLbl->move(screen_width/2 - 150,screen_height/2 -50);
    waitMovie = new QMovie(":/button_image/loading.webp");
    waitLbl->setFixedSize(50, 50);
    waitLbl->setScaledContents(true);
    waitLbl->setMovie(waitMovie);

    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:30px;}");
    serialStopTimer = new QTimer(this);
    connect(serialStopTimer,SIGNAL(timeout()),this,SLOT(serial_stop_deal()));
    serialDialog = new serialdialog(this);
    connect(serialDialog,SIGNAL(serial_config_msg(serial_config)),this,SLOT(serial_config_func(serial_config)));

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
}

all_test::~all_test()
{
    delete ui;

    delete allTestThread;
    delete btTestThead;
}

void all_test::language_reload()
{
    ui->retranslateUi(this);
    serialDialog->language_reload();
}

void all_test::on_beginBtn_clicked()
{
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"time:"<<QTime::currentTime();
//    qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__<<"Thread构造函数ID:"<<QThread::currentThreadId();
    int i = 0;
    QAbstractButton *checkBtn;
    testItemsCount = 0;

    if(tr("begin") == ui->beginBtn->text())
    {
        for(i =0 ;i < CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            if(checkBtn->isChecked())
            {
                testItemsCount++;
            }
        }
        if(testItemsCount == 0)
        {
            ui->textEdit->setText(tr("Please select a test item first"));
            return;
        }
        if(ui->serialChk->isChecked() == true)
        {
            testItemsCount = testItemsCount - 1 + serialConfig.count;
        }
        else if(ui->keyChk->isChecked() == true)
            testItemsCount = testItemsCount - 1;
        //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "testItemsCount:" << testItemsCount;

        if(testItemsCount != 0)
        {
            waitLbl->show();
            waitMovie->start();
            ui->testCheckAllBtn->setEnabled(false);
            for(int i =0 ;i < CheckedBtnList.length();i++)
            {
                checkBtn = CheckedBtnList.at(i);
                checkBtn->setEnabled(false);
            }
        }
        ui->beginBtn->setText(tr("stop"));
        QThread::usleep(10);
        //myLabel->clear();
        ui->textEdit->setText(tr("---test start:"));
        startTime = QTime::currentTime();

        if((testItemsCount != 0) || (testItemsCount == 0 && ui->keyChk->isChecked() == true))
        {
            mainTestThread = new QThread(this);
            allTestThread = new all_test_thread();
            allTestThread->moveToThread(mainTestThread);
            connect(this,SIGNAL(network_test_msg()),allTestThread,SLOT(network_test_thread()));
            connect(this,SIGNAL(usb_test_msg(int)),allTestThread,SLOT(usb_test_thread(int)));
            connect(this,SIGNAL(rtc_test_msg()),allTestThread,SLOT(rtc_test_thread()));
            connect(this,SIGNAL(sdcard_test_msg()),allTestThread,SLOT(sdcard_test_thread()));
            connect(this,SIGNAL(camera_test_msg()),allTestThread,SLOT(camera_test_thread()));
            connect(this,SIGNAL(battary_test_msg()),allTestThread,SLOT(battary_test_thread()));
            connect(this,SIGNAL(sim_test_msg()),allTestThread,SLOT(sim_test_thread()));
            connect(this,SIGNAL(audio_test_msg()),allTestThread,SLOT(audio_test_thread()));
            connect(this,SIGNAL(wifi_test_msg()),allTestThread,SLOT(wifi_test_thread()));
            connect(this,SIGNAL(key_test_msg()),allTestThread,SLOT(key_test_thread()));
            connect(allTestThread,SIGNAL(send_test_msg(int ,QString )),this,SLOT(recv_test_msg(int, QString )));

            if(mainTestThread->isRunning() == false)
            {
                mainTestThread->start();
            }
        }

        if(ui->bluetoothChk->isChecked() == true)
        {
            btThread = new QThread(this);
            btTestThead = new all_test_thread();
            btTestThead->moveToThread(btThread);
            connect(this,SIGNAL(bluetooth_test_msg()),btTestThead,SLOT(bluetooth_test_thread()));
            connect(btTestThead,SIGNAL(send_test_msg(int ,QString )),this,SLOT(recv_test_msg(int, QString )));

            if(btThread->isRunning() == false)
            {
                btThread->start();
            }
            emit bluetooth_test_msg();
        }
        if(ui->audioChk->isChecked() == true)
        {
            emit audio_test_msg();
//            qDebug() << "audio_flag" << __LINE__;
            QProcess p;
            p.start("bash", QStringList() <<"-c" << "aplay /usr/helperboard/test.wav");
            p.waitForFinished(-1);
            p.close();
        }
        if(true == ui->networkChk->isChecked()) emit network_test_msg();
        if(true == ui->usbChk->isChecked())     emit usb_test_msg(usbAddNum);
        if(true == ui->rtcChk->isChecked())     emit rtc_test_msg();
        if(true == ui->sdcardChk->isChecked())  emit sdcard_test_msg();
        if(true == ui->cameraChk->isChecked())  emit camera_test_msg();
        if(true == ui->battaryChk->isChecked()) emit battary_test_msg();
        if(true == ui->simChk->isChecked())     emit sim_test_msg();
        if(true == ui->wifiChk->isChecked())    emit wifi_test_msg();
        if(true == ui->keyChk->isChecked())     emit key_test_msg();
        if(true == ui->serialChk->isChecked())  serial_test_func();
    }
    else
    {
        if(true == mainTestThread->isRunning())
        {
            //mainTestThread->terminate();
            //delete allTestThread;
            mainTestThread->quit();
            mainTestThread->wait();
        }
        if(ui->bluetoothChk->isChecked() == true)
        {
            if(true == btThread->isRunning())
            {
               // btThread->terminate();
               // delete btTestThead;
                btThread->quit();
                btThread->wait();
            }
        }
        for(i = 0;i < serialConfig.count;i++)
        {

            if(true == thread_id[i]->isRunning())
            {
                if(serialStopTimer->isActive() == true)
                {
                    serialStopTimer->stop();
                }
                //thread_id[i]->terminate();
                thread_id[i]->quit();
                thread_id[i]->wait();
            }
        }

            ui->beginBtn->setText(tr("begin"));
            ui->testCheckAllBtn->setEnabled(true);
            waitMovie->stop();
            waitLbl->close();
            for(i =0 ;i < CheckedBtnList.length();i++)
            {
                checkBtn = CheckedBtnList.at(i);
                checkBtn->setEnabled(true);
            }

    }
}

bool all_test::event(QEvent *event)
{
    if(event->type() == QEvent::KeyPress)  //键盘按下处理,其他事件让事件处理器自己处理,不能返回false
    {
        if(ui->keyChk->isChecked() == true && ui->beginBtn->text() == tr("stop"))
        {
            //qDebug()<<"-key pressed!";
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

    if(ui->testCheckAllBtn->text() == tr("check all"))
    {
        for(int i =0 ;i<CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setChecked(true);
        }
        ui->testCheckAllBtn->setText(tr("check none"));
    }
    else if(ui->testCheckAllBtn->text() == tr("check none"))
    {
        for(int i =0 ;i<CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setChecked(false);
        }
        ui->testCheckAllBtn->setText(tr("check all"));
    }
}

void all_test::on_retBtn_clicked()
{
    serialDialog->close();
    QAbstractButton *checkBtn;
    for(int i =0 ;i<CheckedBtnList.length();i++)
    {
        checkBtn = CheckedBtnList.at(i);
        checkBtn->setChecked(false);
    }
    emit Mysignal();
}


void all_test::on_usbChk_clicked()
{
    QStringList items; //ComboBox 列表的内容
    for(int i = 0;i < 50;i++)
    {
        items << QString::number(i);
    }
    QString dlgTitle=tr("Item selection dialog box");
    QString txtLabel=tr("Select the number to add");
    int     curIndex=usbAddNum; //初始选择项
    bool    editable=false; //ComboBox是否可编辑
    bool    ok=false;
    QString num = QString::number(usbAddNum);

    if(true == ui->usbChk->isChecked())
    {
        ui->usbChk->setChecked(Qt::Checked);

        num = QInputDialog::getItem(this, dlgTitle,txtLabel,items,curIndex,editable,&ok);
        if (ok && !num.isEmpty())
        {
            usbAddNum = num.toInt();
            qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__<< usbAddNum;
        }
    }
}


void all_test::on_serialChk_clicked()
{
    if(screen_flag == 1)
    {
      serialDialog->resize(Height*2/3,Width*2/3);
      serialDialog->move(Width-((Width-serialDialog->height())/2),Height/2-serialDialog->width()/2);
    }
    else
    {
      serialDialog->resize(Width/2,Height*3/4);
    }
    serialDialog->exec();

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
    QAbstractButton *checkBtn;
     QString serial_buf;
//    qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__<< str;

   if(test_signal_type != serial_signal)
   {
        testItemsCount--;
//        qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__<< "count:"<< testItemsCount;
    }
    if(testItemsCount == 0)
    {

        ui->testCheckAllBtn->setEnabled(true);
        ui->beginBtn->setEnabled(true);
        waitMovie->stop();
        waitLbl->close();
        for(int i =0 ;i < CheckedBtnList.length();i++)
        {
            checkBtn = CheckedBtnList.at(i);
            checkBtn->setEnabled(true);
        }
        mainTestThread->quit();
        mainTestThread->wait();
    }
    if(ui->keyChk->isChecked() == true)
    {

    }
    else if(ui->serialChk->isChecked() == true)
    {
        if(serialConfig.mode == "server")
        {

        }
        else
        {
            ui->beginBtn->setText(tr("begin"));
        }
    }
    else
    {
        ui->beginBtn->setText(tr("begin"));
    }

    switch (test_signal_type) {
    case network_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        //qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime.netwotk ="<<elapsed<<"s";
        testMsgDisplay(tr("---network test:"),str,elapsed);
        break;
    case usb_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        //qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime.rtc ="<<elapsed<<"s";
        testMsgDisplay(tr("---usb test:    "),str,elapsed);
        break;
    case rtc_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        //qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime.rtc ="<<elapsed<<"s";
        testMsgDisplay(tr("---rtc test:    "),str,elapsed);

        break;
    case sd_card_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        //qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime.sdcard ="<<elapsed<<"s";
        testMsgDisplay(tr("---sdcard test: "),str,elapsed);
        break;
    case key_signal:
        ui->textEdit->append(str);
        break;
    case serial_signal:
        serial_buf = QString("---serial read: %1").arg(str);
        ui->textEdit->append(serial_buf);
        break;
    case camera_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        //qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:camera ="<<elapsed<<"s";

        str = QString(tr("---camera test: Please check the picture display? %1s")).arg(elapsed);
        ui->textEdit->append(str);
        image_show();
        break;
    case battary_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        //qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:battary ="<<elapsed<<"s";
        testMsgDisplay(tr("---battary test:"),str,elapsed);

        break;
    case sim_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        //qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:sim ="<<elapsed<<"s";
        testMsgDisplay(tr("---4G test:     "),str,elapsed);
        break;
    case audio_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        //qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:audio ="<<elapsed<<"s";
        str = QString(tr("---audio test:  please sure sound play test yes or no? %1s")).arg(elapsed);
        ui->textEdit->append(str);
        break;
    case wifi_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        //qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:wifi ="<<elapsed<<"s";
        testMsgDisplay(tr("---wifi test:   "),str,elapsed);
        break;
    case bluetooth_signal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        //qDebug()<< "Line:" << __LINE__<< "FILE:" << __FILE__ <<"QTime:bluetooth ="<<elapsed<<"s";
        testMsgDisplay(tr("---bluetooth test:"),str,elapsed);
        btThread->quit();
        btThread->wait();
        delete btTestThead;
        break;
    default:
        break;
    }
}

void all_test::serial_config_func(serial_config config)
{
    serialConfig = config;
    if(serialConfig.count == 0)
    {
        ui->serialChk->setChecked(Qt::Unchecked);
    }
    else
    {
        ui->serialChk->setChecked(Qt::Checked);
    }
}

void all_test::serial_test_func()
{
    for(int i = 0; i < serialConfig.count;i++)
    {
        //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__<< i;
        thread_id[i] = new QThread(this);
        serial_test_thread[i] = new all_test_thread();
        serial_test_thread[i]->moveToThread(thread_id[i]);

        if(i > 0)
        {
            disconnect(this,SIGNAL(serial_test_client_msg(serial_config)),serial_test_thread[i-1],SLOT(serial_test_thread_client(serial_config)));
        }
        connect(serial_test_thread[i],SIGNAL(send_test_msg(int,QString)),this,SLOT(recv_test_msg(int, QString )));
        connect(this,SIGNAL(serial_test_client_msg(serial_config)),serial_test_thread[i],SLOT(serial_test_thread_client(serial_config)));

        serialConfig.index = i;
        if(thread_id[i]->isRunning() == false)
        {
            thread_id[i]->start();
        }
        emit serial_test_client_msg(serialConfig);
    }
    if(serialConfig.mode == "client")
    {
        serialStopTimer->start(1500);
    }
}

void all_test::serial_stop_deal()
{
    QAbstractButton *checkBtn;

    if(serialStopTimer->isActive() == true)
    {
        //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---!!!!!!!!!!!!!!!!!!!---:";
        ui->textEdit->append(QString(tr("serial test end")));
//        serial_test_recv_func(QString(tr("serial test end")));
        serialStopTimer->stop();
    }

    for(int i = 0; i < serialConfig.count; i++)
    {
        testItemsCount--;
        if(testItemsCount == 0)
        {
            ui->beginBtn->setText(tr("begin"));
            ui->testCheckAllBtn->setEnabled(true);
            ui->beginBtn->setEnabled(true);
            waitMovie->stop();
            waitLbl->close();
            for(int i =0 ;i < CheckedBtnList.length();i++)
            {
                checkBtn = CheckedBtnList.at(i);
                checkBtn->setEnabled(true);
            }
            mainTestThread->quit();
            mainTestThread->wait();
        }

        if(i > 0)
        {
            disconnect(this,SIGNAL(serial_test_stop_msg()),serial_test_thread[i-1],SLOT(serial_test_thread_stop()));
        }
        connect(this,SIGNAL(serial_test_stop_msg()),serial_test_thread[i],SLOT(serial_test_thread_stop()));

        if(thread_id[i]->isRunning() == true)
        {
            //qDebug() << "Line:" << __LINE__<< "FILE:" << __FILE__ << "---FUNC---:" << __FUNCTION__;

            emit serial_test_stop_msg();
            thread_id[i]->quit();
            thread_id[i]->wait();
        }
//            delete serial_test_thread[i];
    }
}
void all_test::image_show()
{
    ui->textEdit->append("\n");
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    QTextImageFormat imageFormat;
    imageFormat.setName("/data/yuv.jpg");
    imageFormat.setWidth(ui->textEdit->width()/2);
    imageFormat.setHeight(ui->textEdit->height()/2);
    cursor.insertImage(imageFormat);
}

void all_test::all_font()
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

    ui->groupBox_2->setFont(font);
    ui->networkChk->setFont(font);
    ui->bluetoothChk->setFont(font);
    ui->sdcardChk->setFont(font);
    ui->battaryChk->setFont(font);
    ui->cameraChk->setFont(font);
    ui->audioChk->setFont(font);
    ui->serialChk->setFont(font);
    ui->keyChk->setFont(font);
    ui->wifiChk->setFont(font);
    ui->simChk->setFont(font);
    ui->usbChk->setFont(font);
    ui->rtcChk->setFont(font);
    ui->testCheckAllBtn->setFont(font);
    ui->beginBtn->setFont(font);
    ui->textEdit->setFont(font);
}
