#include "all_test.h"
#include "ui_all_test.h"
#include "all_test_interface.h"
#include <QDebug>

static QScreen *screen;
static int screenFlag;
static int screenWidth;
static int screenHeight;

all_test::all_test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::all_test)
{
    ui->setupUi(this);
    qRegisterMetaType<serial_config>("serial_config");
    serialConfig.count = 0;
    screen = QGuiApplication::primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    waitLbl = new QLabel(this);
    waitMovie = new QMovie(":/button_image/loading.webp");
    waitLbl->setFixedSize(50, 50);
    waitLbl->setScaledContents(true);
    waitLbl->setMovie(waitMovie);

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line_2->setStyleSheet("background-color: rgb(186, 189, 182);");
        waitLbl->move(screenHeight/2,screenWidth/2);
    }
    else
    {
        waitLbl->move(screenWidth/2,screenHeight/2 );
    }

    allFont();
    ui->textEdit->verticalScrollBar()->setStyleSheet("QScrollBar{width:30px;}");
    serialStopTimer = new QTimer(this);
    connect(serialStopTimer,SIGNAL(timeout()),this,SLOT(serial_stop_deal()));
    serialDialog = new serialdialog(this);
    connect(serialDialog,SIGNAL(serial_config_msg(serial_config)),this,SLOT(serial_config_func(serial_config)));

    testButtonGroup = new QButtonGroup(this);
    testButtonGroup->setExclusive(false);               //Set to non-exclusive mode
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
    checkedBtnList = testButtonGroup->buttons();
}

all_test::~all_test()
{
    delete ui;
    delete allTestThread;
    delete btTestThead;
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
    serialDialog->languageReload();
}

void all_test::on_btn_begin_clicked()
{
    int i = 0;
    QAbstractButton *checkBtn;
    testItemsCount = 0;                        //Counting the number of checked button
    if(tr("begin") == ui->btn_begin->text())
    {
        for(i =0 ;i < checkedBtnList.length();i++)
        {
            checkBtn = checkedBtnList.at(i);
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
        else if(ui->keyChk->isChecked() == true)        //Key tests are not counted in the number of events tested in addition
            testItemsCount = testItemsCount - 1;

        if(testItemsCount != 0)
        {
            waitLbl->show();
            waitMovie->start();
            ui->btn_testCheckAll->setEnabled(false);
            for(int i =0 ;i < checkedBtnList.length();i++)
            {
                checkBtn = checkedBtnList.at(i);
                checkBtn->setEnabled(false);
            }
        }
        else if(ui->keyChk->isChecked() == true)
        {
            ui->btn_testCheckAll->setEnabled(false);
            for(int i =0 ;i < checkedBtnList.length();i++)
            {
                checkBtn = checkedBtnList.at(i);
                checkBtn->setEnabled(false);
            }
        }
        ui->btn_begin->setText(tr("stop"));
        QThread::usleep(10);
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
        ui->btn_begin->setEnabled(false);
        if(true == mainTestThread->isRunning())
        {
            mainTestThread->terminate();                //terminate thread
            delete allTestThread;
        }
        if(ui->bluetoothChk->isChecked() == true)
        {
            if(true == btThread->isRunning())
            {
                btThread->terminate();
                delete btTestThead;
            }
        }
        for(i = 0;i < serialConfig.count;i++)
        {

            if(true == threadId[i]->isRunning())
            {
                if(serialStopTimer->isActive() == true)
                {
                    serialStopTimer->stop();
                }
                threadId[i]->terminate();
            }
        }
        ui->btn_begin->setText(tr("begin"));
        ui->btn_testCheckAll->setEnabled(true);
        waitMovie->stop();
        waitLbl->close();
        for(i =0 ;i < checkedBtnList.length();i++)
        {
            checkBtn = checkedBtnList.at(i);
            checkBtn->setEnabled(true);
        }
        ui->btn_begin->setEnabled(true);
    }
}

bool all_test::event(QEvent *event)           // key test
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
        for(int i =0 ;i<checkedBtnList.length();i++)
        {
            checkBtn = checkedBtnList.at(i);
            checkBtn->setChecked(true);
        }
        ui->btn_testCheckAll->setText(tr("check none"));
    }
    else if(ui->btn_testCheckAll->text() == tr("check none"))
    {
        for(int i =0 ;i<checkedBtnList.length();i++)
        {
            checkBtn = checkedBtnList.at(i);
            checkBtn->setChecked(false);
        }
        ui->btn_testCheckAll->setText(tr("check all"));
    }
}

void all_test::on_btn_ret_clicked()
{
    serialDialog->close();
    QAbstractButton *checkBtn;
    for(int i =0 ;i<checkedBtnList.length();i++)
    {
        checkBtn = checkedBtnList.at(i);
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
    QString dlgTitle=tr("Item selection dialog box");
    QString txtLabel=tr("Select the number to add");
    int     curIndex=usbAddNum;
    bool    editable=false;
    bool    ok=false;
    QString num = QString::number(usbAddNum);

    if(true == ui->usbChk->isChecked())
    {
        ui->usbChk->setChecked(Qt::Checked);
        num = QInputDialog::getItem(this, dlgTitle,txtLabel,items,curIndex,editable,&ok);
        if (ok && !num.isEmpty())
        {
            usbAddNum = num.toInt();
        }
    }
}

void all_test::on_serialChk_clicked()
{
    if(screenFlag == 1)
    {
        serialDialog->resize(screenHeight*2/3,screenWidth*2/3);
        serialDialog->move(screenWidth-((screenWidth-serialDialog->height())/2),screenHeight/2-serialDialog->width()/2);
    }
    else
    {
        serialDialog->resize(screenWidth/2,screenHeight*3/4);
    }
    serialDialog->exec();
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

    if(testSignalType != serialSignal)
    {
        testItemsCount--;
    }
    if(testItemsCount == 0)
    {
        if(ui->keyChk->isChecked() == false)
        {
            ui->btn_testCheckAll->setEnabled(true);
            ui->btn_begin->setEnabled(true);

            for(int i =0 ;i < checkedBtnList.length();i++)
            {
                checkBtn = checkedBtnList.at(i);
                checkBtn->setEnabled(true);
            }
        }

        mainTestThread->quit();
        mainTestThread->wait();

        if(ui->bluetoothChk->isChecked() == true)
        {

        }
        else if(ui->serialChk->isChecked() == true)
        {
            if(serialConfig.mode == "server")
            {

            }
            else
            {
                ui->btn_begin->setText(tr("begin"));
            }
        }
        else
        {
            waitMovie->stop();
            waitLbl->close();

            if(ui->keyChk->isChecked() == false)
            {
                ui->btn_begin->setText(tr("begin"));
            }
        }
    }
    switch (testSignalType) {
    case networkSignal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        testMsgDisplay(tr("---network test:  "),str,elapsed);
        break;
    case usbSignal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        testMsgDisplay(tr("---usb test:  "),str,elapsed);
        break;
    case rtcSignal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        testMsgDisplay(tr("---RTC test:  "),str,elapsed);
        break;
    case sdCardSignal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        testMsgDisplay(tr("---sdcard test:  "),str,elapsed);
        break;
    case keySignal:
        ui->textEdit->append(str);
        break;
    case serialSignal:
        serialBuf = QString(tr("---serial read:  %1")).arg(str);
        ui->textEdit->append(serialBuf);
        break;
    case cameraSignal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        str = QString(tr("---camera test:  Please check the picture display? %1s")).arg(elapsed);
        ui->textEdit->append(str);
        imageShow();
        break;
    case battarySignal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        testMsgDisplay(tr("---battary test:  "),str,elapsed);
        break;
    case simSignal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        testMsgDisplay(tr("---4G test:  "),str,elapsed);
        break;
    case audioSignal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        str = QString(tr("---audio test:  Please sure sound play test yes or no? %1s")).arg(elapsed);
        ui->textEdit->append(str);
        break;
    case wifiSignal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        testMsgDisplay(tr("---WiFi test:  "),str,elapsed);
        break;
    case bluetoothSignal:
        stopTime = QTime::currentTime();
        elapsed = startTime.secsTo(stopTime);
        testMsgDisplay(tr("---bluetooth test:  "),str,elapsed);
        btThread->quit();
        btThread->wait();
        if(ui->btn_begin->text() == tr("stop"))
        {
            waitMovie->stop();
            waitLbl->close();
            if(ui->keyChk->isChecked() == false)
            {
                ui->btn_begin->setText(tr("begin"));
            }
        }
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
        threadId[i] = new QThread(this);
        serialTestThread[i] = new all_test_thread();
        serialTestThread[i]->moveToThread(threadId[i]);

        if(i > 0)
        {
            disconnect(this,SIGNAL(serial_test_client_msg(serial_config)),serialTestThread[i-1],SLOT(serial_test_thread_client(serial_config)));
        }
        connect(serialTestThread[i],SIGNAL(send_test_msg(int,QString)),this,SLOT(recv_test_msg(int, QString )));
        connect(this,SIGNAL(serial_test_client_msg(serial_config)),serialTestThread[i],SLOT(serial_test_thread_client(serial_config)));

        serialConfig.index = i;
        if(threadId[i]->isRunning() == false)
        {
            threadId[i]->start();
        }
        emit serial_test_client_msg(serialConfig);
    }

    if(serialConfig.mode == tr("client"))
    {
        serialStopTimer->start(1500);
    }
    else if(serialConfig.mode == tr("server"))
    {
        ui->textEdit->append(QString(tr("---serial test: waiting to read data as server")));
    }
}

void all_test::serial_stop_deal()
{
    QAbstractButton *checkBtn;

    if(serialStopTimer->isActive() == true)
    {
        ui->textEdit->append(QString(tr("---serial test: OK!")));
        serialStopTimer->stop();
    }

    for(int i = 0; i < serialConfig.count; i++)
    {
        testItemsCount--;
        if(testItemsCount == 0)
        {
            ui->btn_begin->setText(tr("begin"));
            ui->btn_testCheckAll->setEnabled(true);
            ui->btn_begin->setEnabled(true);
            waitMovie->stop();
            waitLbl->close();
            for(int i =0 ;i < checkedBtnList.length();i++)
            {
                checkBtn = checkedBtnList.at(i);
                checkBtn->setEnabled(true);
            }
            mainTestThread->quit();
            mainTestThread->wait();
        }

        if(i > 0)
        {
            disconnect(this,SIGNAL(serial_test_stop_msg()),serialTestThread[i-1],SLOT(serial_test_thread_stop()));
        }
        connect(this,SIGNAL(serial_test_stop_msg()),serialTestThread[i],SLOT(serial_test_thread_stop()));

        if(threadId[i]->isRunning() == true)
        {
            emit serial_test_stop_msg();
            threadId[i]->quit();
            threadId[i]->wait();
        }
        //            delete serial_test_thread[i];
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

void all_test::allFont()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = screenWidth / realX * 2.54;
    qreal realHeight = screenHeight / realY *2.54;
    QFont font;
    if(screenFlag)
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
