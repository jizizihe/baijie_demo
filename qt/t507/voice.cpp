#include "voice.h"
#include "ui_voice.h"

voice::voice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice)
{
    ui->setupUi(this);
}

voice::~voice()
{
    delete ui;
}

void voice::on_pushButton_clicked()
{
    QProcess *proc = new QProcess;
    static bool flag = true;

    if(flag)
    {
        ui->pushButton->setText("end");
        flag = false;
        qDebug() << "begin arecord";
        proc->start("arecord -r 16000 -c 1 -t wav /data/luyin.wav");

    }
    else
    {
        ui->pushButton->setText("begin");

        proc->start("kill -9");

        qDebug() << "end arecord" ;
        flag = true;
    }
}

void voice::on_play_clicked()
{
    QProcess *proc = new QProcess;

    qDebug() << "begin play" ;
    proc->start("aplay /data/luyin.wav");
}

void voice::on_return_2_clicked()
{
    emit Mysignal();
}
