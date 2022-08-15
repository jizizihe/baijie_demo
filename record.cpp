#include "record.h"
#include "ui_record.h"

#include <QMessageBox>
#include <QLabel>
#include <QDebug>

//#define Dbug
#ifdef Dbug
#define qdebug(format, ...)  qDebug("Function:%s,Line:%d " format,__func__,__LINE__, ##__VA_ARGS__);
#else
#define qdebug(format, ...)
#endif


static bool flag = true;
static bool refresh_flag = true;
//static int play = 0;
static QString s_time;
static QString e_time;
QString str;
QString str_name;

record::record(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::record)
{
    ui->setupUi(this);

    ui->pathname_2->setText("/data");


    refresh("/data");

    QTimer *show_time = new QTimer(this);
    //get_length = new QTimer(this);

    show_time->start(1000);
    connect(show_time,SIGNAL(timeout()),this,SLOT(show_time()));
   // connect(get_length,SIGNAL(timeout()),this,SLOT(get_nowlength()));
   // connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(get_musictime()));
   //   connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(get_audiotime()));

    //ui->label_9->setText(tr("begin"));
   // ui->begin->setIcon(QIcon(":/button_image/voice/begin.png"));

}

record::~record()
{
    delete ui;
}

void record::refresh(QString dir_path)
{
    QDir dir(dir_path);

    QStringList filters;

    filters << "*.wav" << "*.mp3" << "*.mp4";

    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();
    QStringList files;
    for(int i = 0; i != list.size(); i++)
    {
        files << list.at(i).fileName();
    }
    files.removeAll("\n");
     ui->combox->clear();
    ui->combox->addItems(files);
}

void record::show_time()
{
    ui->time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss"));
}


void record::on_return_2_clicked()
{

    emit Mysignal_r();
}

void record::on_choose_3_clicked()
{
    QString path = QFileDialog::getExistingDirectory(NULL,tr("choose"),"/data",QFileDialog::ShowDirsOnly);
    QDir dir1(path);
    QFileInfoList list1 = dir1.entryInfoList();

    ui->pathname_2->setText(list1.at(0).path());
    refresh(list1.at(0).path());

}


int record::calculatetime(QString startime,QString endtime)
{
  int start_s,start_m,start_h,start_d;
  int end_s,end_m,end_h,end_d;
  int time,ss,mm,hh,dd;
  start_s = startime.section(':',2,2).trimmed().toInt();
  start_m = startime.section(':',1,1).trimmed().toInt();
  start_h = startime.section(':',0,0).remove(0,11).trimmed().toInt();
  start_d = startime.section('-',2,2).remove(2,9).trimmed().toInt();

  end_s = endtime.section(':',2,2).trimmed().toInt();
  end_m = endtime.section(':',1,1).trimmed().toInt();
  end_h = endtime.section(':',0,0).remove(0,11).trimmed().toInt();
  end_d = endtime.section('-',2,2).remove(2,9).trimmed().toInt();

  if(start_s <= end_s)
      ss = end_s - start_s;
  else
      ss = end_s;
  if(start_m <= end_m)
      mm = end_m - start_m;
  else
      mm = end_m;
  if(start_h <= end_h)
      hh = end_h - start_h;
  else
      hh = end_h;

  time = ss + mm*60 + hh*60*60;
  return time;
}



void record::on_begin_clicked()
{

    QProcess *proc = new QProcess;
//    proc->start("killall aplay");
//    proc->waitForFinished(-1);

    QString files;
    int time;

    if(flag)
    {
        s_time = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
        str_name = QString("%1/%2.record.wav").arg(ui->pathname_2->text()).arg(s_time);
        qDebug() << str_name;
        str = QString("arecord -r 16000 -c 1 -t wav %1").arg(str_name);

        ui->label_9->setText(tr("stop"));
        ui->begin->setIcon(QIcon(":/button_image/voice/stop.png"));
        flag = false;
        refresh_flag = false;
        proc->start(str);

        ui->time->setText(s_time);

    }
    else
    {
        ui->label_9->setText(tr("begin"));
        ui->begin->setIcon(QIcon(":/button_image/voice/begin.png"));
        e_time = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
        time = calculatetime(s_time,e_time);

        files = QString("%1_record_%2.wav").arg(s_time).arg(time);
        file_name = files;

        QMessageBox::information(this, NULL, QString(tr("Successfully save the file %1")).arg(file_name));
        ui->combox->addItem(QString("%1").arg(file_name));

        QString filename = QString("%1/%2").arg(ui->pathname_2->text()).arg(file_name);
        flag = true;
        QString strCmd = QString("mv %1 %2").arg(str_name).arg(filename);
        qDebug() << filename;
        qDebug() << str_name;
        qDebug() << strCmd;
        chang_filename(strCmd);
        proc->start("killall arecord");
    }


}
