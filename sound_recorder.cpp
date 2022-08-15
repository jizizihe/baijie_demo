#include "sound_recorder.h"
#include "ui_sound_recorder.h"

sound_recorder::sound_recorder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sound_recorder)
{
    ui->setupUi(this);
    connect(&voice_w,SIGNAL(Mysignal_v()),this,SLOT(show_sound_record()));
    connect(&record_w,SIGNAL(Mysignal_r()),this,SLOT(show_sound_record()));
}

sound_recorder::~sound_recorder()
{
    delete ui;
}

void sound_recorder::on_pushButton_clicked()
{
    this->hide();
    record_w.show();
}

void sound_recorder::on_pushButton_2_clicked()
{
    this->hide();
    voice_w.show();
}

void sound_recorder::on_return_2_clicked()
{
    emit Mysignal();
}

void sound_recorder::language_reload()
{
     ui->retranslateUi(this);
}

void sound_recorder::show_sound_record()
{
    voice_w.hide();
    record_w.hide();
    this->show();
}
