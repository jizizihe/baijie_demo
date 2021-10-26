#include "about_board.h"
#include "ui_about_board.h"

//extern "C"
//{
    #include "boardinfo_interface.h"
//}

about_board::about_board(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::about_board)
{
    ui->setupUi(this);

//    QScrollArea *m_pScroll = new QScrollArea(ui->widget);
//    m_pScroll->setWidget(ui->widget_2);
//    m_pScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->widget_2->setMinimumSize(1500,1000);
//    QHBoxLayout *pLayout = new QHBoxLayout;
//    pLayout->addWidget(m_pScroll);
//    pLayout->setMargin(0);
//    pLayout->setSpacing(0);
//    ui->widget->setLayout(pLayout);

    QTversion = new QLabel(ui->groupBox_3);
    QTversion->move(20,140);
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
    ui->batterylevel_label->setText(QString(tr("battery level: %1")).arg(battery_level));
    char *battery_status = get_battery_status();
    ui->batterystatus_label->setText(QString(tr("battery status: %1")).arg(battery_status));
}

void about_board::CPU_temp_update()
{
    float temp = get_CPU_temp();
    ui->CPU_temp_label->setText(QString(tr("CPU temp: %1")).arg(temp));
}

void about_board::boardname_update()
{
    char *boardname = get_boardname();
    ui->name_label->setText(QString(tr("Board name: %1")).arg(boardname));
}

void about_board::kernelname_update()
{
    char *kernel;
    kernel = get_kernel();
    ui->kernel_label->setText(QString(tr("kernel: %1")).arg(kernel));

}

void about_board::OSname_update()
{
    char *OSname = get_OSname();
    ui->OS_label->setText(QString(tr("OS: %1")).arg(OSname));
}

void about_board::resolution_update()
{
    int x,y;
    get_resolution(&x,&y);
    ui->resolution_label->setText(QString(tr("resolution: %1 * %2")).arg(x).arg(y));
}

void about_board::QTversion_update()
{
    char *vQT;
    vQT = get_QTversion();
    QTversion->setText(QString(tr("QT version: %1")).arg(vQT));
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
