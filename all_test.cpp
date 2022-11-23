#include "all_test.h"
#include "ui_all_test.h"
#include "all_test_interface.h"
#include <QDebug>

static int g_screenWidth;
static int g_screenHeight;

all_test::all_test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::all_test)
{
    ui->setupUi(this);
    qRegisterMetaType<serial_config>("serial_config");
    QScreen *screen = QGuiApplication:: primaryScreen();
    g_screenWidth = screen->size().width();
    g_screenHeight = screen->size().height();
    g_waitLbl = new QLabel(this);
    g_waitMovie = new QMovie(":/button_image/loading.webp");
    g_waitLbl->setFixedSize(50, 50);
    g_waitLbl->setScaledContents(true);
    g_waitLbl->setMovie(g_waitMovie);
    g_waitLbl->move(g_screenWidth/2,g_screenHeight/2 );
    g_stopSerialTimer = new QTimer(this);
    g_serialDialog = new serialdialog(this);
    g_serialConfig.count = 0;
    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:30px;}");
    setAllTestFont();

    connect(g_stopSerialTimer,SIGNAL(timeout()),this,SLOT(serial_stop_deal()));
    connect(g_serialDialog,SIGNAL(serial_config_msg(serial_config)),this,SLOT(serial_config_func(serial_config)));

    g_testButtonGroup = new QButtonGroup(this);
    g_testButtonGroup->setExclusive(false);               // Set to non-exclusive mode
    g_testButtonGroup->addButton(ui->networkChk,0);
    g_testButtonGroup->addButton(ui->usbChk,1);
    g_testButtonGroup->addButton(ui->rtcChk,2);
    g_testButtonGroup->addButton(ui->sdcardChk,3);
    g_testButtonGroup->addButton(ui->keyChk,4);
    g_testButtonGroup->addButton(ui->serialChk,5);
    g_testButtonGroup->addButton(ui->cameraChk,6);
    g_testButtonGroup->addButton(ui->battaryChk,7);
    g_testButtonGroup->addButton(ui->simChk,8);
    g_testButtonGroup->addButton(ui->audioChk,9);
    g_testButtonGroup->addButton(ui->wifiChk,10);
    g_testButtonGroup->addButton(ui->bluetoothChk,11);
    g_checkedBtnList = g_testButtonGroup->buttons();
}

all_test::~all_test()
{
    delete ui;
    delete g_allTestThread;
    delete g_btTestThead;
}

void all_test::languageReload()
{
    QString str = ui->btn_begin->text();
    if(str == "停止")
    {
        on_btn_begin_clicked();
    }
    if(str == "stop")
    {
        on_btn_begin_clicked();
    }
    ui->textEdit->clear();
    ui->retranslateUi(this);
    g_serialDialog->languageReload();
}

void all_test::on_btn_begin_clicked()
{
    int i = 0;
    QAbstractButton *checkBtn;
    g_testItemsCount = 0;                        // Counting the number of checked button
    if(tr("begin") == ui->btn_begin->text())
    {
        for(i =0 ;i < g_checkedBtnList.length();i++)
        {
            checkBtn = g_checkedBtnList.at(i);
            if(checkBtn->isChecked())
            {
                g_testItemsCount++;
            }
        }
        if(g_testItemsCount == 0)
        {
            ui->textEdit->setText(tr("Please select a test item first"));
            return;
        }
        if(ui->serialChk->isChecked() == true)
        {
            g_testItemsCount = g_testItemsCount - 1 + g_serialConfig.count;
        }
        else if(ui->keyChk->isChecked() == true)        // Key tests are not counted in the number of events tested in addition
            g_testItemsCount = g_testItemsCount - 1;

        if(g_testItemsCount != 0)
        {
            g_waitLbl->show();
            g_waitMovie->start();
            ui->btn_testCheckAll->setEnabled(false);
            for(int i =0 ;i < g_checkedBtnList.length();i++)
            {
                checkBtn = g_checkedBtnList.at(i);
                checkBtn->setEnabled(false);
            }
        }
        else if(ui->keyChk->isChecked() == true)
        {
            ui->btn_testCheckAll->setEnabled(false);
            for(int i =0 ;i < g_checkedBtnList.length();i++)
            {
                checkBtn = g_checkedBtnList.at(i);
                checkBtn->setEnabled(false);
            }
        }
        QThread::usleep(10);
        ui->btn_begin->setText(tr("stop"));
        ui->textEdit->setText(tr("---test start:"));
        g_startTime = QTime::currentTime();

        if((g_testItemsCount != 0) || (g_testItemsCount == 0 && ui->keyChk->isChecked() == true))
        {
            g_mainTestThread = new QThread(this);
            g_allTestThread = new all_test_thread();
            g_allTestThread->moveToThread(g_mainTestThread);
            connect(this,SIGNAL(network_test_msg()),g_allTestThread,SLOT(network_test_thread()));
            connect(this,SIGNAL(usb_test_msg(int)),g_allTestThread,SLOT(usb_test_thread(int)));
            connect(this,SIGNAL(rtc_test_msg()),g_allTestThread,SLOT(rtc_test_thread()));
            connect(this,SIGNAL(sdcard_test_msg()),g_allTestThread,SLOT(sdcard_test_thread()));
            connect(this,SIGNAL(camera_test_msg()),g_allTestThread,SLOT(camera_test_thread()));
            connect(this,SIGNAL(battary_test_msg()),g_allTestThread,SLOT(battary_test_thread()));
            connect(this,SIGNAL(sim_test_msg()),g_allTestThread,SLOT(sim_test_thread()));
            connect(this,SIGNAL(audio_test_msg()),g_allTestThread,SLOT(audio_test_thread()));
            connect(this,SIGNAL(wifi_test_msg()),g_allTestThread,SLOT(wifi_test_thread()));
            connect(this,SIGNAL(key_test_msg()),g_allTestThread,SLOT(key_test_thread()));
            connect(g_allTestThread,SIGNAL(send_test_msg(int ,QString )),this,SLOT(recv_test_msg(int, QString )));

            if(g_mainTestThread->isRunning() == false)
            {
                g_mainTestThread->start();
            }
        }

        if(ui->bluetoothChk->isChecked() == true)
        {
            g_bluetoothThread = new QThread(this);
            g_btTestThead = new all_test_thread();
            g_btTestThead->moveToThread(g_bluetoothThread);
            connect(this,SIGNAL(bluetooth_test_msg()),g_btTestThead,SLOT(bluetooth_test_thread()));
            connect(g_btTestThead,SIGNAL(send_test_msg(int ,QString )),this,SLOT(recv_test_msg(int, QString )));

            if(g_bluetoothThread->isRunning() == false)
            {
                g_bluetoothThread->start();
            }
            emit bluetooth_test_msg();
        }
        if(ui->audioChk->isChecked() == true)
        {
            emit audio_test_msg();
            QProcess pro;
            pro.start("bash", QStringList() <<"-c" << "aplay /usr/helperboard/test.wav");
            pro.waitForFinished(-1);
            pro.close();
        }
        if(true == ui->networkChk->isChecked()) emit network_test_msg();
        if(true == ui->usbChk->isChecked())     emit usb_test_msg(g_usbAddNum);
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
        ui->btn_begin->setEnabled(false);
        if(true == g_mainTestThread->isRunning())
        {
            g_mainTestThread->terminate();                // Terminate thread
            delete g_allTestThread;
        }
        if(ui->bluetoothChk->isChecked() == true)
        {
            if(true == g_bluetoothThread->isRunning())
            {
                g_bluetoothThread->terminate();
                delete g_btTestThead;
            }
        }
        for(i = 0;i < g_serialConfig.count;i++)
        {
            if(true == g_threadId[i]->isRunning())
            {
                if(g_stopSerialTimer->isActive() == true)
                {
                    g_stopSerialTimer->stop();
                }
                g_threadId[i]->terminate();
            }
        }
        ui->btn_begin->setText(tr("begin"));
        ui->btn_testCheckAll->setEnabled(true);
        ui->btn_begin->setEnabled(true);
        g_waitMovie->stop();
        g_waitLbl->close();
        for(i =0 ;i < g_checkedBtnList.length();i++)
        {
            checkBtn = g_checkedBtnList.at(i);
            checkBtn->setEnabled(true);
        }      
    }
}

bool all_test::event(QEvent *event)           // Key test
{
    if(event->type() == QEvent::KeyPress)
    {
        if(ui->keyChk->isChecked() == true && ui->btn_begin->text() == tr("stop"))
        {
            ui->textEdit->append(tr("---key pressed!"));
            return true;
        }
    }
    return QWidget::event(event);
}

void all_test::on_btn_testCheckAll_clicked()
{
    QAbstractButton *checkBtn;
    if(ui->btn_testCheckAll->text() == tr("check all"))
    {
        for(int i =0 ;i<g_checkedBtnList.length();i++)
        {
            checkBtn = g_checkedBtnList.at(i);
            checkBtn->setChecked(true);
        }
        ui->btn_testCheckAll->setText(tr("check none"));
    }
    else if(ui->btn_testCheckAll->text() == tr("check none"))
    {
        for(int i =0 ;i<g_checkedBtnList.length();i++)
        {
            checkBtn = g_checkedBtnList.at(i);
            checkBtn->setChecked(false);
        }
        ui->btn_testCheckAll->setText(tr("check all"));
    }
}

void all_test::on_btn_ret_clicked()
{
    g_serialDialog->close();
    QAbstractButton *checkBtn;
    for(int i =0 ;i<g_checkedBtnList.length();i++)
    {
        checkBtn = g_checkedBtnList.at(i);
        checkBtn->setChecked(false);
    }
    emit all_test_back_msg();
}

void all_test::on_usbChk_clicked()
{
    QStringList items;
    for(int i = 0;i < 50;i++)
    {
        items << QString::number(i);
    }
    QString dlgTitle = tr("Item selection dialog box");
    QString txtLabel = tr("Select the number to add");
    int     curIndex = g_usbAddNum;
    bool    editable = false;
    bool    ok = false;
    QString num = QString::number(g_usbAddNum);

    if(true == ui->usbChk->isChecked())
    {
        ui->usbChk->setChecked(Qt::Checked);
        num = QInputDialog::getItem(this, dlgTitle,txtLabel,items,curIndex,editable,&ok);
        if (ok && !num.isEmpty())
        {
            g_usbAddNum = num.toInt();
        }
    }
}

void all_test::on_serialChk_clicked()
{
    g_serialDialog->resize(g_screenWidth/2,g_screenHeight*3/4);
    g_serialDialog->exec();
}

void all_test::testMsgDisplay(QString type,QString str,int time)
{
    QString dispStr;
    if(str == "OK")
    {
        dispStr = type + QString(tr("OK! %1s")).arg(time);
        ui->textEdit->append(dispStr);
    }
    else if((str == "failed")||(str == "Failed"))
    {
        dispStr = type + QString(tr("failed--- %1s")).arg(time);
        ui->textEdit->append(dispStr);
    }
    else
    {
        dispStr = QString("%1%2--- %3s").arg(type).arg(str).arg(time);
        ui->textEdit->append(dispStr);
    }
}

void all_test::recv_test_msg(int testSignalType, QString str)
{
    QAbstractButton *checkBtn;
    QString serialBuf;

    if(testSignalType != EnumSerialSignal)
    {
        g_testItemsCount--;
    }
    if(g_testItemsCount == 0)
    {
        if(ui->keyChk->isChecked() == false)
        {
            ui->btn_testCheckAll->setEnabled(true);
            ui->btn_begin->setEnabled(true);

            for(int i =0 ;i < g_checkedBtnList.length();i++)
            {
                checkBtn = g_checkedBtnList.at(i);
                checkBtn->setEnabled(true);
            }
        }

        g_mainTestThread->quit();
        g_mainTestThread->wait();

        if(ui->bluetoothChk->isChecked() == true)
        {

        }
        else if((ui->serialChk->isChecked() == true)&&(g_serialConfig.mode == "server"))
        {

        }
        else
        {
            g_waitMovie->stop();
            g_waitLbl->close();

            if(ui->keyChk->isChecked() == false)
            {
                ui->btn_begin->setText(tr("begin"));
            }
        }
    }
    switch (testSignalType) {
    case EnumNetworkSignal:
        g_stopTime = QTime::currentTime();
        g_elapsed = g_startTime.secsTo(g_stopTime);
        testMsgDisplay(tr("---network test:  "),str,g_elapsed);
        break;
    case EnumUsbSignal:
        g_stopTime = QTime::currentTime();
        g_elapsed = g_startTime.secsTo(g_stopTime);
        testMsgDisplay(tr("---usb test:  "),str,g_elapsed);
        break;
    case EnumRtcSignal:
        g_stopTime = QTime::currentTime();
        g_elapsed = g_startTime.secsTo(g_stopTime);
        testMsgDisplay(tr("---RTC test:  "),str,g_elapsed);
        break;
    case EnumSdCardSignal:
        g_stopTime = QTime::currentTime();
        g_elapsed = g_startTime.secsTo(g_stopTime);
        testMsgDisplay(tr("---sdcard test:  "),str,g_elapsed);
        break;
    case EnumKeySignal:
        ui->textEdit->append(str);
        break;
    case EnumSerialSignal:
        serialBuf = QString(tr("---serial read:  %1")).arg(str);
        ui->textEdit->append(serialBuf);
        break;
    case EnumCameraSignal:
        g_stopTime = QTime::currentTime();
        g_elapsed = g_startTime.secsTo(g_stopTime);
        str = QString(tr("---camera test:  Please check the picture display? %1s")).arg(g_elapsed);
        ui->textEdit->append(str);
        imageShow();
        break;
    case EnumBattarySignal:
        g_stopTime = QTime::currentTime();
        g_elapsed = g_startTime.secsTo(g_stopTime);
        testMsgDisplay(tr("---battary test:  "),str,g_elapsed);
        break;
    case EnumSimSignal:
        g_stopTime = QTime::currentTime();
        g_elapsed = g_startTime.secsTo(g_stopTime);
        testMsgDisplay(tr("---4G test:  "),str,g_elapsed);
        break;
    case EnumAudioSignal:
        g_stopTime = QTime::currentTime();
        g_elapsed = g_startTime.secsTo(g_stopTime);
        str = QString(tr("---audio test:  Please sure sound play test yes or no? %1s")).arg(g_elapsed);
        ui->textEdit->append(str);
        break;
    case EnumWifiSignal:
        g_stopTime = QTime::currentTime();
        g_elapsed = g_startTime.secsTo(g_stopTime);
        testMsgDisplay(tr("---WiFi test:  "),str,g_elapsed);
        break;
    case EnumBluetoothSignal:
        g_stopTime = QTime::currentTime();
        g_elapsed = g_startTime.secsTo(g_stopTime);
        testMsgDisplay(tr("---bluetooth test:  "),str,g_elapsed);
        g_bluetoothThread->quit();
        g_bluetoothThread->wait();
        if(ui->btn_begin->text() == tr("stop"))
        {
            g_waitMovie->stop();
            g_waitLbl->close();
            if(ui->keyChk->isChecked() == false)
            {
                ui->btn_begin->setText(tr("begin"));
            }
        }
        delete g_btTestThead;
        break;
    default:
        break;
    }
}

void all_test::serial_config_func(serial_config config)
{
    g_serialConfig = config;
    if(g_serialConfig.count == 0)
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
    for(int i = 0; i < g_serialConfig.count;i++)
    {
        g_threadId[i] = new QThread(this);
        g_serialTestThread[i] = new all_test_thread();
        g_serialTestThread[i]->moveToThread(g_threadId[i]);

        if(i > 0)
        {
            disconnect(this,SIGNAL(serial_test_start_msg(serial_config)),g_serialTestThread[i-1],SLOT(serial_test_thread_start(serial_config)));
        }
        connect(g_serialTestThread[i],SIGNAL(send_test_msg(int,QString)),this,SLOT(recv_test_msg(int, QString )));
        connect(this,SIGNAL(serial_test_start_msg(serial_config)),g_serialTestThread[i],SLOT(serial_test_thread_start(serial_config)));

        g_serialConfig.index = i;
        if(g_threadId[i]->isRunning() == false)
        {
            g_threadId[i]->start();
        }
        emit serial_test_start_msg(g_serialConfig);
    }

    if(g_serialConfig.mode == tr("client"))
    {
        g_stopSerialTimer->start(1500);
    }
    else if(g_serialConfig.mode == tr("server"))
    {
        ui->textEdit->append(QString(tr("---serial test: Waiting to read data as server")));
    }
}

void all_test::serial_stop_deal()
{
    QAbstractButton *checkBtn;

    if(g_stopSerialTimer->isActive() == true)
    {
        ui->textEdit->append(QString(tr("---serial test: OK!")));
        g_stopSerialTimer->stop();
    }

    for(int i = 0; i < g_serialConfig.count; i++)
    {
        g_testItemsCount--;
        if(g_testItemsCount == 0)
        {
            ui->btn_begin->setText(tr("begin"));
            ui->btn_testCheckAll->setEnabled(true);
            ui->btn_begin->setEnabled(true);
            g_waitMovie->stop();
            g_waitLbl->close();
            for(int i =0 ;i < g_checkedBtnList.length();i++)
            {
                checkBtn = g_checkedBtnList.at(i);
                checkBtn->setEnabled(true);
            }
            g_mainTestThread->quit();
            g_mainTestThread->wait();
        }

        if(i > 0)
        {
            disconnect(this,SIGNAL(serial_test_stop_msg()),g_serialTestThread[i-1],SLOT(serial_test_thread_stop()));
        }
        connect(this,SIGNAL(serial_test_stop_msg()),g_serialTestThread[i],SLOT(serial_test_thread_stop()));

        if(g_threadId[i]->isRunning() == true)
        {
            emit serial_test_stop_msg();
            g_threadId[i]->quit();
            g_threadId[i]->wait();
        }
    }
}
void all_test::imageShow()
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

void all_test::setAllTestFont()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    qreal realX = screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;

    if(realWidth < 15)
    {
        font.setPointSize(12);
    }
    else if (realWidth < 18)
    {
        font.setPointSize(14);
    }
    else
    {
        font.setPointSize(17);
    }

    ui->groupBox->setFont(font);
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
    ui->btn_testCheckAll->setFont(font);
    ui->btn_begin->setFont(font);
    ui->textEdit->setFont(font);
    ui->title_AllInterfaceTests->setFont(font);
}
