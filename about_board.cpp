#include "about_board.h"
#include "ui_about_board.h"
#include <QScreen>
#include "boardinfo_interface.h"

static QScreen *screen;
static int screenFlag;
static int screenWidth;
static int screenHeight;
static QTimer *timer;

about_board::about_board(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::about_board)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground,true);
    ui->pb_batteryLevel->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    aboutFont();
    board_name_update();
    kernel_name_update();
    battery_update();
    CPU_temp_update();
    resolution_update();
    QT_version_update();
    OS_name_update();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(battery_update()));
    connect(timer,SIGNAL(timeout()), this, SLOT(CPU_temp_update()));
}

about_board::~about_board()
{
    delete ui;
}

void about_board::showEvent(QShowEvent *event)
{
    timer->start(1000);
}

void about_board::battery_update()
{
    tr("Unknown");tr("Uncharged");tr("charge");tr("Full");
    int batteryLevel = getBatteryLevel();
    ui->pb_batteryLevel->setValue(batteryLevel);
    char *batteryStatus = getBatteryStatus();
    ui->lbl_batteryStatusVale->setText(QString(tr("%1")).arg(batteryStatus));
}

void about_board::CPU_temp_update()
{
    float temp = getCPUTemp();
    ui->lbl_CPUTempValue->setText(QString(tr("%1")).arg(temp));
}

void about_board::board_name_update()
{
    char *boardName = getBoardName();
    ui->lbl_boardNameValue->setText(QString(tr("%1")).arg(boardName));
}

void about_board::kernel_name_update()
{
    char *kernel;
    kernel = getKernel();
    ui->lbl_kernelValue->setText(QString(tr("%1")).arg(kernel));
}

void about_board::OS_name_update()
{
    char *OSName = getOSName();
    ui->lbl_OSValue->setText(QString(tr("%1")).arg(OSName));
}

void about_board::resolution_update()
{
    int x,y;
    getResolution(&x,&y);
    ui->lbl_ResolutionValue->setText(QString(tr("%1 * %2")).arg(x).arg(y));
}

void about_board::QT_version_update()
{
    char vQT[64];
    if(getQTVersion("/usr/helperboard/qt/lib/","libQt5Core",vQT) == 0)
    {
        if(getQTVersion("/usr/helperboard/qt/lib/","libQt4Core",vQT) == 0)
        {
            ui->lbl_QTVersionValue->setText(QString(tr("Unknow")));
        }
    }
    else
    {
        ui->lbl_QTVersionValue->setText(QString(tr("%1")).arg(vQT));
    }
}

void about_board::languageReload()
{
    ui->retranslateUi(this);
    board_name_update();
    kernel_name_update();
    battery_update();
    CPU_temp_update();
    resolution_update();
    OS_name_update();
    QT_version_update();
}

void about_board::aboutFont()
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
    ui->lbl_boardName->setFont(font);
    ui->lbl_batteryLevel->setFont(font);
    ui->lbl_batteryStatusVale->setFont(font);
    ui->lbl_CPUTemp->setFont(font);
    ui->lbl_Resolution->setFont(font);
    ui->lbl_OS->setFont(font);
    ui->lbl_BusinessEmail->setFont(font);
    ui->lbl_kernel->setFont(font);
    ui->lbl_QTVersionValue->setFont(font);
    ui->lbl_boardNameValue->setFont(font);
    ui->lbl_batteryStatusVale->setFont(font);
    ui->lbl_CPUTempValue->setFont(font);
    ui->lbl_ResolutionValue->setFont(font);
    ui->lbl_OSValue->setFont(font);
    ui->lbl_kernelValue->setFont(font);
    ui->lbl_QTVersion->setFont(font);
    ui->lbl_companyWebsiteValue->setFont(font);
    ui->lbl_aboutBoard->setFont(font);
    ui->lbl_companyWebsite->setFont(font);
    ui->lbl_BusinessEmailValue->setFont(font);
}

void about_board::on_ret_btn_clicked()
{
    timer->stop();
    emit about_board_back_msg();
}
