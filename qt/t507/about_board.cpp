#include "about_board.h"
#include "ui_about_board.h"

#include <QScrollArea>
#include <QHBoxLayout>
#include <QProcess>
#include <QDebug>
extern "C"
{
    #include "battery_interface.h"
}

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

    QProcess *process = new QProcess(this);
    process->start("uname -m");
    if(!process->waitForFinished())
    {
        ui->kernel_label->setText(QString("kernel : Not detected"));
    }
    QString hardware_name = QString::fromLocal8Bit(process->readAllStandardOutput());
    hardware_name = hardware_name.simplified();

    process->start("uname -s");
    if(!process->waitForFinished())
    {
        ui->kernel_label->setText(QString("kernel : Not detected"));
    }
    QString kernel_name = QString::fromLocal8Bit(process->readAllStandardOutput());
    kernel_name = kernel_name.simplified();

    process->start("uname -r");
    if(!process->waitForFinished())
    {
        ui->kernel_label->setText(QString("kernel : Not detected"));
    }
    QString kernel_release = QString::fromLocal8Bit(process->readAllStandardOutput());
    kernel_release = kernel_release.simplified();

    ui->kernel_label->setText(QString("kernel: %1 %2 %3").arg(hardware_name).arg(kernel_name).arg(kernel_release));

    char *OSname = get_OSname();
    ui->OS_label->setText(QString("OS: %1").arg(OSname));

    char *boardname = get_boardname();
    ui->name_label->setText(QString("Board name: %1").arg(boardname));

    int x,y;
    get_resolution(&x,&y);
    ui->resolution_label->setText(QString("resolution: %1 * %2").arg(x).arg(y));

    QLabel *QTversion = new QLabel("QTversion",ui->groupBox_3);
    QTversion->move(20,140);
    QProcess *QT = new QProcess(this);
    QT->start("qmake -v | cut -d ' ' -f 4 | sed -n '2p' ");
    if(!QT->waitForFinished())
    {
        QTversion->setText(QString("QT version : Unknown"));
    }
    else
    {
        QTversion->setText(QString("QT version: %1").arg(QString::fromLocal8Bit(QT->readAllStandardOutput())));
    }

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
    int battery_level = get_battery_level();
    ui->batterylevel_label->setText(QString(tr("battery level: %1")).arg(battery_level));
    char *battery_status = get_battery_status();
    ui->batterystatus_label->setText(QString(tr("battery status: %1")).arg(battery_status));
}

void about_board::CPU_temp_update()
{
    float temp = get_CPU_temp();
    ui->CPU_temp_label->setText(QString("CPU temp: %1").arg(temp));
}

