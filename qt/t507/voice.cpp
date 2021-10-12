#include "voice.h"
#include "ui_voice.h"

#include <QMessageBox>

static bool flag = true;
static bool refresh_flag = true;
QString str[100];

QString file_name;

int len;
voice::voice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice)
{
    ui->setupUi(this);
    ui->pathname_2->setText("/data");

    QDir dir("/data");

    QStringList filters;

    filters << "*.wav" << "*.mp3" << "*.mp4";

    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();

    for(int i = 0; i != list.size(); i++)
    {
        QString name = list.at(i).fileName();
        qDebug() << "file name:" << name;
        qDebug() << "fileName="<<list.at(i).fileName() ;
        qDebug() << "absoluteFilePath=" << list.at(i).absoluteFilePath() ;

        str[i] = name;
        len = i;
        qDebug() << "str[" << i <<"]" << str[i] ;
    }
    ui->combox->clear();
    for(int i = 0; i <= len; i++)
    {
        if("." == str[i] || ".." == str[i])
        {
            continue;
        }
        qDebug() << "IN FOR str[" << i <<"]" << str[i] ;
        ui->combox->addItem(str[i]);
    }
    len = -1;

}

voice::~voice()
{
    delete ui;
}


void voice::on_return_2_clicked()
{
    emit Mysignal();
}

void voice::on_choose_3_clicked()
{
    QString path = QFileDialog::getExistingDirectory(NULL,"choose","/data",QFileDialog::ShowDirsOnly);
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
        qDebug() << "file name:" << name;
        qDebug() << "fileName="<<list.at(i).fileName() ;
        qDebug() << "absoluteFilePath=" << list.at(i).absoluteFilePath() ;

        str[i] = name;
        len = i;
        qDebug() << "str[" << i <<"]" << str[i] ;
    }
    ui->combox->clear();
    for(int i = 0; i <= len; i++)
    {
        if("." == str[i] || ".." == str[i])
        {
            continue;
        }
        qDebug() << "IN FOR str[" << i <<"]" << str[i] ;
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

    qDebug() << "time name:" << s_time;


    if(flag)
    {
        s_time = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
        str = QString("arecord -r 16000 -c 1 -t wav %1/%2_record.wav").arg(ui->pathname_2->text()).arg(s_time);

        files = QString("%1_record.wav").arg(s_time);
        file_name = files;

        qDebug() << "str name:" << str;
        qDebug() << "file_name:" << file_name;
        ui->begin->setText("end");
        flag = false;
        refresh_flag = false;
        qDebug() << "begin arecord";
        proc->start(str);

        ui->time->setText(s_time);
        while(1)
        {
            QString s;
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            s= QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
            ui->time->setText(s);
        }

        ui->time->setText(s_time);
        qDebug() << "in begin s_time:" << s_time;
    }
    else
    {
        ui->begin->setText("begin");
        qDebug() << "In end s_time:" << file_name;

        QMessageBox::StandardButton reply;

        reply = QMessageBox::information(this, file_name, file_name);

        ui->combox->addItem(QString("%1").arg(file_name));

        proc->start("killall arecord");

        qDebug() << "end arecord" ;
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
    int len;

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
        qDebug() <<"in paly file_name" << file_name;
        qDebug() << "befor choose text:" << ui->combox->currentText();
        ui->combox->setCurrentText(file_name);
        qDebug() << "now choose text:" << ui->combox->currentText();
    }

    qDebug() << "begin play";
    QString name = ui->combox->currentText();
    qDebug() << "name:" << name;
    str[0] = QString("aplay %1/%2").arg(ui->pathname_2->text()).arg(name);
    qDebug() << "name:" << str[0];
    proc->start(str[0]);
}
