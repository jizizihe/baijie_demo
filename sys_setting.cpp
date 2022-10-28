#include "sys_setting.h"
#include "ui_sys_setting.h"
#include "mainwindow.h"

static int screenFlag = 0;
static int screenWidth;
static int screenHeight;
static int backlightFirstFlag = 0;
static int timeSetFirstFlag = 0;
static int userFirstFlag = 0;
static int boardFirstFlag = 0;
static int showFirstFlag;
static QGraphicsView *backlightView;
static QGraphicsView *timeView;
static QGraphicsView *userView;
static QGraphicsView *boardView;
static QScreen *screen;

sys_setting::sys_setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sys_setting)
{
    ui->setupUi(this);

    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    sysFont();
    catOTGStatus();
    this->setAttribute(Qt::WA_StyledBackground,true);
    connect(&backlightWg,SIGNAL(backlight_back_msg()),this,SLOT(backlight_back()));
    connect(&timesetWg,SIGNAL(time_set_back_msg()),this,SLOT(time_back()));
    connect(&userWg,SIGNAL(user_manual_back_msg()),this,SLOT(user_back()));
    connect(&boardWg,SIGNAL(about_board_back_msg()),this,SLOT(board_back()));
}

sys_setting::~sys_setting()
{
    delete ui;
}

void sys_setting::on_btn_ret_clicked()
{
    emit sys_back_msg();
}

void sys_setting::on_btn_backlight_clicked()
{
    this->hide();
    if(screenFlag == 0)
    {
        backlightWg.resize(screenWidth,screenHeight);
        backlightWg.show();
    }
    else
    {
        if(backlightFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&backlightWg);
            w->setRotation(90);

            backlightView = new QGraphicsView(scene);
            backlightView->setWindowFlags(Qt::FramelessWindowHint);
            backlightView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            backlightView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            backlightView->resize(screenWidth,screenHeight);
            backlightWg.resize(screenHeight,screenWidth);
            backlightWg.show();
            backlightView->show();
            backlightFirstFlag++;
        }
        else
        {
            backlightWg.resize(screenHeight,screenWidth);
            backlightWg.show();
            backlightView->resize(screenWidth,screenHeight);
            backlightView->show();
        }
    }
}

void sys_setting::on_btn_RTC_clicked()
{
    this->hide();
    if(screenFlag == 0)
    {
        timesetWg.resize(screenWidth,screenHeight);
        timesetWg.show();
        timesetWg.activateWindow();timesetWg.setFocus();
    }
    else
    {
        if(timeSetFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&timesetWg);
            w->setRotation(90);

            timeView = new QGraphicsView(scene);
            timeView->setWindowFlags(Qt::FramelessWindowHint);
            timeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            timeView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            timeView->resize(screenWidth,screenHeight);
            timesetWg.resize(screenHeight,screenWidth);
            timesetWg.show();
            timeView->show();
            timeView->activateWindow();timeView->setFocus();
            timesetWg.activateWindow();timesetWg.setFocus();
            timeSetFirstFlag++;
        }
        else
        {
            timesetWg.resize(screenHeight,screenWidth);
            timesetWg.show();
            timeView->resize(screenWidth,screenHeight);
            timeView->show();
            timeView->activateWindow();timeView->setFocus();
            timesetWg.activateWindow();timesetWg.setFocus();
        }
    }
}

void sys_setting::on_btn_cn_clicked()
{
    static bool flag = 0;
    if(flag)
    {
        delete transl;
        transl = NULL;
    }
    else
    {
        transl = new QTranslator();
        transl->load(":/chinese.qm");
    }
    flag = !flag;
    emit main_cn_msg();                      //Send a signal,Translate the contents of the main interface
    ui->retranslateUi(this);
    timesetWg.languageReload();
    backlightWg.languageReload();
    boardWg.languageReload();
    userWg.languageReload();
    QString str = ui->lbl_cn->text();
    if(str == "Chinese/English")
    {
        ui->widget_7->layout()->setContentsMargins(28,0,28,0);
        ui->gridLayout_4->layout()->setContentsMargins(14,2,14,2);
        ui->gridLayout_5->layout()->setContentsMargins(33,2,33,2);
    }
    else
    {
        ui->widget_7->layout()->setContentsMargins(0,0,0,0);
        ui->gridLayout_4->layout()->setContentsMargins(0,0,0,0);
        ui->gridLayout_5->layout()->setContentsMargins(0,0,0,0);
    }
}

void sys_setting::on_btn_user_clicked()
{
    this->hide();
    if(screenFlag == 0)
    {
        userWg.resize(screenWidth,screenHeight);
        userWg.show();
    }
    else
    {
        if(userFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&userWg);
            w->setRotation(90);

            userView = new QGraphicsView(scene);
            userView->setWindowFlags(Qt::FramelessWindowHint);
            userView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            userView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            userView->resize(screenWidth,screenHeight);
            userWg.resize(screenHeight,screenWidth);
            userWg.show();
            userView->show();
            userFirstFlag++;
        }
        else
        {
            userWg.resize(screenHeight,screenWidth);
            userWg.show();
            userView->resize(screenWidth,screenHeight);
            userView->show();
        }
    }
}

void sys_setting::on_btn_about_clicked()
{
    this->hide();
    if(screenFlag == 0)
    {
        boardWg.resize(screenWidth,screenHeight);
        boardWg.show();
    }
    else
    {
        if(boardFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&boardWg);
            w->setRotation(90);

            boardView = new QGraphicsView(scene);
            boardView->setWindowFlags(Qt::FramelessWindowHint);
            boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            boardView->resize(screenWidth,screenHeight);
            boardWg.resize(screenHeight,screenWidth);
            boardWg.show();
            boardView->show();
            boardFirstFlag++;
        }
        else
        {
            boardWg.resize(screenHeight,screenWidth);
            boardWg.show();
            boardView->resize(screenWidth,screenHeight);
            boardView->show();
        }
    }
}

void sys_setting::backlight_back()
{
    if(screenFlag == 1)
    {
        backlightView->hide();
    }
    backlightWg.hide();
    this->show();
}

void sys_setting::time_back()
{
    if(screenFlag == 1)
    {
        timeView->hide();
    }
    timesetWg.hide();
    this->show();
    this->activateWindow();this->setFocus();
}

void sys_setting::board_back()
{
    if(screenFlag == 1)
    {
        boardView->hide();
    }
    boardWg.hide();
    this->show();
}

void sys_setting::user_back()
{
    if(screenFlag == 1)
    {
        userView->hide();
    }
    userWg.hide();
    this->show();
}

void sys_setting::sysFont()
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
    ui->lbl_cn->setFont(font);
    ui->lbl_timeSet->setFont(font);
    ui->lbl_backlight->setFont(font);
    ui->lbl_aboutBoard->setFont(font);
    ui->lbl_userManual->setFont(font);
    ui->lbl_system->setFont(font);
    ui->lbl_OGT->setFont(font);
}

void sys_setting::closeEvent(QCloseEvent *event)
{
    if(showFirstFlag == 0)
    {
        on_btn_RTC_clicked();
        time_back();
        showFirstFlag++;
    }
    QWidget::closeEvent(event);
}

void sys_setting::on_btn_OTGHost_clicked()
{
    QString strOTGStatus = ui->lbl_OGT->text();
    if(strOTGStatus == tr("OTG Slave"))
    {
        QString strCmd = "cat /sys/bus/platform/drivers/otg\" \"manager/usbc0/usb_host";
        QProcess pro;
        pro.start("bash", QStringList() <<"-c" << strCmd);
        pro.waitForFinished(-1);
        QString strResult = pro.readAllStandardOutput();
        pro.close();
        if(!strResult.isEmpty())
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("set successful!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
            ui->lbl_OGT->setText(tr("OTG Host"));
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("set failed!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
        }
    }
    else
    {
        QString strCmd = "cat /sys/bus/platform/drivers/otg\" \"manager/usbc0/usb_device";
        QProcess pro;
        pro.start("bash", QStringList() <<"-c" << strCmd);
        pro.waitForFinished(-1);
        QString strResult = pro.readAllStandardOutput();
        pro.close();
        if(!strResult.isEmpty())
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("set successful!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
            ui->lbl_OGT->setText(tr("OTG Slave"));
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("set failed!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
        }
    }
}

void sys_setting::catOTGStatus()
{
    QString strCmd = "cat /sys/bus/platform/drivers/otg\" \"manager/usbc0/otg_role";
    QProcess pro;
    pro.start("bash", QStringList() <<"-c" << strCmd);
    pro.waitForFinished(-1);
    QString strResult = pro.readAllStandardOutput();
    pro.close();
    if(strResult.contains("device",Qt::CaseInsensitive))
    {
        ui->lbl_OGT->setText(tr("OTG Slave"));
    }
    else
    {
        ui->lbl_OGT->setText(tr("OTG Host"));
    }
}
