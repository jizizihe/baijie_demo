#include "about_board.h"
#include "ui_about_board.h"
#include <QScreen>
#include "about_board_interface.h"

static int g_screenWidth;
static int g_screenHeight;
static QTimer *g_timer;

about_board::about_board(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::about_board)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground,true);
    ui->pb_batteryLevel->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    QScreen *screen = qApp->primaryScreen();
    g_screenWidth = screen->size().width();
    g_screenHeight = screen->size().height();

    setAboutBoardFont();
    board_name_update();
    kernel_name_update();
    battery_update();
    cpu_temp_update();
    resolution_update();
    qt_version_update();
    os_name_update();

    g_timer = new QTimer(this);
    connect(g_timer, SIGNAL(timeout()), this, SLOT(battery_update()));
    connect(g_timer,SIGNAL(timeout()), this, SLOT(cpu_temp_update()));
}

about_board::~about_board()
{
    delete ui;
}

void about_board::showEvent(QShowEvent *event)
{
    g_timer->start(1000);
}

void about_board::battery_update()
{
    tr("Unknown");tr("Uncharged");tr("charge");tr("Full");
    int batteryLevel = getBatteryLevel();
    ui->pb_batteryLevel->setValue(batteryLevel);
    char *batteryStatus = getBatteryStatus();
    ui->lbl_batteryStatusVale->setText(QString(tr("%1")).arg(batteryStatus));
}

void about_board::cpu_temp_update()
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

void about_board::os_name_update()
{
    char *osName = getOSName();
    ui->lbl_OSValue->setText(QString(tr("%1")).arg(osName));
}

void about_board::resolution_update()
{
    int x,y;
    getResolution(&x,&y);
    ui->lbl_ResolutionValue->setText(QString(tr("%1 * %2")).arg(x).arg(y));
}

void about_board::qt_version_update()
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
    cpu_temp_update();
    resolution_update();
    os_name_update();
    qt_version_update();
}

void about_board::setAboutBoardFont()
{
    QScreen *screen;
    screen = qApp->primaryScreen();
    qreal realX = screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;

    if(realWidth < 15)
    {
        font.setPointSize(10);
        ui->lbl_telValue->setFont(font);
        ui->lbl_customization->setFont(font);
        ui->lbl_BusinessEmailValue->setFont(font);
        font.setPointSize(12);
    }
    else if (realWidth < 17)
    {
        font.setPointSize(12);
        ui->lbl_telValue->setFont(font);
        ui->lbl_customization->setFont(font);
        ui->lbl_BusinessEmailValue->setFont(font);
        font.setPointSize(14);
    }
    else
    {
        font.setPointSize(15);
        ui->lbl_telValue->setFont(font);
        ui->lbl_customization->setFont(font);
        ui->lbl_BusinessEmailValue->setFont(font);
        font.setPointSize(17);
    }

    ui->lbl_boardName->setFont(font);
    ui->lbl_batteryLevel->setFont(font);
    ui->lbl_batteryStatusVale->setFont(font);
    ui->lbl_CPUTemp->setFont(font);
    ui->lbl_Resolution->setFont(font);
    ui->lbl_OS->setFont(font);
    ui->lbl_kernel->setFont(font);
    ui->lbl_QTVersionValue->setFont(font);
    ui->lbl_boardNameValue->setFont(font);
    ui->lbl_batteryStatusVale->setFont(font);
    ui->lbl_CPUTempValue->setFont(font);
    ui->lbl_ResolutionValue->setFont(font);
    ui->lbl_OSValue->setFont(font);
    ui->lbl_kernelValue->setFont(font);
    ui->lbl_QTVersion->setFont(font);
    ui->lbl_aboutBoard->setFont(font);
    ui->lbl_companyWebsite->setFont(font);
    ui->lbl_batteryStatus->setFont(font);
    ui->lbl_companyWebsiteValue->setFont(font);
}

void about_board::on_btn_ret_clicked()
{
    g_timer->stop();
    emit about_board_back_msg();
}
