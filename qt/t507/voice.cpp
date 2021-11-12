#include "voice.h"
#include "ui_voice.h"

#include <QMessageBox>
#include <QLabel>

//#define Dbug
#ifdef Dbug
#define qdebug(format, ...)  qDebug("Function:%s,Line:%d " format,__func__,__LINE__, ##__VA_ARGS__);
#else
#define qdebug(format, ...)
#endif


static bool flag = true;
static bool refresh_flag = true;
int len;

QString str[100];
QString file_name;
QProcess *proc;



voice::voice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice)
{
    ui->setupUi(this);
    ui->pathname_2->setText("/data");

    ui->horizontalSlider->setRange(0,31);
    ui->horizontalSlider->setValue(15);

    refresh();

    QTimer *show_time = new QTimer(this);
    show_time->start(1000);
    connect(show_time,SIGNAL(timeout()),this,SLOT(show_time()));

    ui->label_9->setText(tr("begin"));
    ui->begin->setIcon(QIcon(":/t507_button_image/voice/begin.png"));

}

voice::~voice()
{
    delete ui;
}

void voice::refresh()
{
    QDir dir("/data");

    QStringList filters;

    filters << "*.wav" << "*.mp3" << "*.mp4";

    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();

    for(int i = 0; i != list.size(); i++)
    {
        QString name = list.at(i).fileName();
        str[i] = name;
        len = i;
    }
    ui->combox->clear();
    for(int i = 0; i <= len; i++)
    {
        if("." == str[i] || ".." == str[i] || "" == str[i])
        {
            continue;
        }
        ui->combox->addItem(str[i]);
    }
    len = -1;
}

void voice::show_time()
{
    ui->time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss"));
}

void voice::on_horizontalSlider_valueChanged(int value)
{
    proc = new QProcess();
    qdebug("sound_value:%d",value);
    int sound = 31 - value;
    proc->start(QString("amixer cset numid=6 %1").arg(sound));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);
    qdebug("******************************************");

}

void voice::on_return_2_clicked()
{
    emit Mysignal();
}

void voice::on_choose_3_clicked()
{
    QString path = QFileDialog::getExistingDirectory(NULL,tr("choose"),"/data",QFileDialog::ShowDirsOnly);
    QDir dir1(path);
    QFileInfoList list1 = dir1.entryInfoList();

    ui->pathname_2->setText(list1.at(0).path());
    QDir dir(list1.at(0).path());
    QStringList filters;
    filters << "*.wav";
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();

    for(int i = 0; i != list.size(); i++)
    {
        QString name = list.at(i).fileName();
        str[i] = name;
        len = i;
    }
    ui->combox->clear();
    for(int i = 0; i <= len; i++)
    {
        if("." == str[i] || ".." == str[i])
        {
            continue;
        }
        ui->combox->addItem(str[i]);
    }
    len = -1;
}

void voice::on_begin_clicked()
{
    QProcess *proc = new QProcess;

    QString files;
    QString str ;
    QString s_time;
    if(flag)
    {
        s_time = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
        str = QString("arecord -r 16000 -c 1 -t wav %1/%2_record.wav").arg(ui->pathname_2->text()).arg(s_time);

        files = QString("%1_record.wav").arg(s_time);
        file_name = files;
        ui->label_9->setText(tr("stop"));
        ui->begin->setIcon(QIcon(":/t507_button_image/voice/stop.png"));
        flag = false;
        refresh_flag = false;
        proc->start(str);

        ui->time->setText(s_time);
    }
    else
    {
        ui->label_9->setText(tr("begin"));
        ui->begin->setIcon(QIcon(":/t507_button_image/voice/begin.png"));

        QMessageBox::information(this, NULL, QString(tr("Successfully save the file %1")).arg(file_name));

        ui->combox->addItem(QString("%1").arg(file_name));

        proc->start("killall arecord");

        flag = true;
    }
}

void voice::on_play_clicked()
{
    QProcess *proc = new QProcess;
    QString path = QString("%1").arg(ui->pathname_2->text());
    QDir dir(path);
    QStringList filters;
    filters << "*.wav";
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    QString str[100];
    int len = 0;

    if(!refresh_flag)
    {
        refresh_flag = true;
        for(int i = 0; i < list.size(); i++)
        {
           str[i] = list.at(i).fileName();
           len = i;
        }
        ui->combox->clear();

        for(int i = 0; i <= len; i++)
        {
            if("." == str[i] || ".." == str[i])
            {
                continue;
            }
            ui->combox->addItem(str[i]);
        }
        len = -1;
        ui->combox->setCurrentText(file_name);
    }

    QString name = ui->combox->currentText();
    str[0] = QString("aplay %1/%2").arg(ui->pathname_2->text()).arg(name);
    proc->start(str[0]);
}

void voice::language_reload()
{
    ui->retranslateUi(this);
    ui->pathname_2->setText("/data");

}


