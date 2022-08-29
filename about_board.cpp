#include "about_board.h"
#include "ui_about_board.h"

#include <QScreen>
#include "boardinfo_interface.h"

static QScreen *screen;
static int screen_flag;
static int Width;
static int Height;

about_board::about_board(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::about_board)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground,true);
    ui->progressBar->setAlignment(Qt::AlignLeft | Qt::AlignCenter);

    screen = qApp->primaryScreen();
    Width = screen->size().width();			//屏幕宽
    Height = screen->size().height();
    if(Width < Height)
    {
        screen_flag = 1;
    }
    about_font();
    boardname_update();
    kernelname_update();
    battery_update();
    CPU_temp_update();
    resolution_update();
    QTversion_update();
    OSname_update();

    QTimer *timer = new QTimer(this);
    timer->start(1000);

    connect(timer, SIGNAL(timeout()), this, SLOT(battery_update()));
    connect(timer,SIGNAL(timeout()), this, SLOT(CPU_temp_update()));
}

about_board::~about_board()
{
    delete ui;
}

void about_board::on_pushButton_clicked()
{
    emit Mysignal();
}

void about_board::battery_update()
{
    tr("Unknown");tr("Uncharged");tr("charge");tr("Full");
    int battery_level = get_battery_level();
   // ui->batterylevel_label->setText(QString(tr("battery level:")));
    ui->progressBar->setValue(battery_level);
    char *battery_status = get_battery_status();
    ui->batterystatus_label->setText(QString(tr(" %1")).arg(battery_status));
}

void about_board::CPU_temp_update()
{
    float temp = get_CPU_temp();
    ui->CPU_temp_label->setText(QString(tr(" %1")).arg(temp));
}

void about_board::boardname_update()
{
    char *boardname = get_boardname();
    ui->name_label->setText(QString(tr(" %1")).arg(boardname));
}

void about_board::kernelname_update()
{
    char *kernel;
    kernel = get_kernel();
    ui->kernel_label->setText(QString(tr(" %1")).arg(kernel));
}

void about_board::OSname_update()
{
    char *OSname = get_OSname();
    ui->OS_label->setText(QString(tr(" %1")).arg(OSname));
}

void about_board::resolution_update()
{
    int x,y;
    get_resolution(&x,&y);
    ui->resolution_label->setText(QString(tr(" %1 * %2")).arg(x).arg(y));
}

void about_board::QTversion_update()
{
    char vQT[64];
    if(get_QTversion("/usr/helperboard/qt/lib/","libQt5Core",vQT) == 0)
    {
         if(get_QTversion("/usr/helperboard/qt/lib/","libQt4Core",vQT) == 0)
         {
             ui->QTversion->setText(QString(tr(" Unknow")));
         }
    }
    else
    {
        ui->QTversion->setText(QString(tr(" %1")).arg(vQT));
    }
}

void about_board::language_reload()
{
    ui->retranslateUi(this);
    boardname_update();
    kernelname_update();
    battery_update();
    CPU_temp_update();
    resolution_update();
    OSname_update();
    QTversion_update();

}

void about_board::about_font()
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
    ui->label_3->setFont(font);
    ui->batterylevel_label->setFont(font);
    ui->label_5->setFont(font);
    ui->label_6->setFont(font);
    ui->label_7->setFont(font);
    ui->label_9->setFont(font);
    ui->label_8->setFont(font);
    ui->label_2->setFont(font);
    ui->label->setFont(font);
    ui->name_label->setFont(font);
    ui->batterystatus_label->setFont(font);
    ui->CPU_temp_label->setFont(font);
    ui->resolution_label->setFont(font);
    ui->OS_label->setFont(font);
    ui->kernel_label->setFont(font);
    ui->QTversion->setFont(font);
    ui->label_4->setFont(font);
}
