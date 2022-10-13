#include "voice.h"
#include "ui_voice.h"

#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <iostream>
#include <fstream>
using namespace std;

//#define Dbug
#ifdef Dbug
#define qdebug(format, ...)  qDebug("Function:%s,Line:%d " format,__func__,__LINE__, ##__VA_ARGS__);
#else
#define qdebug(format, ...)
#endif

static int screen_flag;
static int Width;  //屏幕宽
static int Height;

static QGraphicsView *rename_view;
static QGraphicsView *save_view;
static QGraphicsView *file_vview;

static int rename_flag = 0;
static int save_flag = 0;
static int file_flag;

static bool flag = true;
static bool refresh_flag = true;
static int view_show;

static QString s_time;
static int kk;
static int length;
static int record_length;
static int flag_record = 0;
static int flag_stop_play = 0;

//static int slider_flag = 0;
static int volume_flag;
static QString file_path;
static QScreen *screenn;
static int show_num;
static QString savepath;
static QString savefile;
QTimer *get_length;

voice::voice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice)
{
    ui->setupUi(this);
    screenn = qApp->primaryScreen();
    Width = screenn->size().width();
    Height = screenn->size().height();

    if(Width < Height)
    {
        screen_flag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    voice_font();
    volume_flag = 1;

    ui->stackedWidget->setCurrentIndex(0);
    ui->Slider_volume->setRange(0,30);
    ui->Slider_volume->setValue(0);
    ui->label_timestart->setText("00:00:00");
    ui->label_timeend->setText("00:00:00");

    pro_path.start("bash");
    QTimer *show_time = new QTimer(this);
    get_length = new QTimer(this);

    show_time->start(1000);
    connect(show_time,SIGNAL(timeout()),this,SLOT(show_time()));
    connect(get_length,SIGNAL(timeout()),this,SLOT(get_nowlength()));
    connect(&rename_w,SIGNAL(rename_finish()),this,SLOT(update_file()));
    connect(&rename_w,SIGNAL(rename_back()),this,SLOT(renamew_hide()));
    connect(&changname,SIGNAL(save_back()),this,SLOT(savew_hide()));
    connect(&File_oprationw,SIGNAL(file_rev2(QString)),this,SLOT(file_path(QString)));
    connect(&File_oprationw,SIGNAL(file_hide()),this,SLOT(refile_hide()));
    connect(this,SIGNAL(sliderchange(int)),this,SLOT(slider_change(int)));
    //connect(&changname,SIGNAL(save_path(QString,QString)),this,SLOT(save_path(QString,QString)));

    ui->btn_rec->setText(tr("start"));
    ui->begin->setText("record");
    ui->play->setText("paly");
    ui->combox->view()->parentWidget()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    ui->combox->view()->parentWidget()->setAttribute(Qt::WA_TranslucentBackground);

}

voice::~voice()
{
    delete ui;
}

void voice::refresh(QString dir_path)
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
    if(!savefile.isEmpty())
    {
        for(int i = 0; i < ui->combox->count(); i++)
        {
            ui->combox->setCurrentIndex(i);
            if(ui->combox->currentText() == savefile)
            {
                break;
            }
        }
        savefile = "";
    }
}

void voice::set_volume(int value)
{
    QProcess *proc = new QProcess();
    //qdebug("sound_value:%d",value);
    int sound = 31 - value;
    proc->start(QString("amixer cset numid=6 %1").arg(sound));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);
    //slider_flag = 0;
}

void voice::show_time()
{
    if(flag_record == 1)
    {
        record_length++;
        QString length = record_timelength(record_length);
        ui->record_timelength->setText(length);
    }
//    if(slider_flag == 1)
//    {
//       int value = ui->Slider_volume->value();
//       set_volume(value);
//    }
}

//void voice::on_horizontalSlider_valueChanged(int value)
//{
//    //qDebug() << value;
//    QProcess *proc = new QProcess();
//    qdebug("sound_value:%d",value);
//    int sound = 31 - value;
//    proc->start(QString("amixer cset numid=6 %1").arg(sound));
//    proc->waitForStarted(-1);
//    proc->waitForFinished(-1);
//    qdebug("******************************************");
//    proc->close();
//}

void voice::on_return_2_clicked()
{
    QProcess proc;
    proc.start("killall aplay");
    proc.waitForFinished(-1);
    emit Mysignal();
    proc.close();
    if(screen_flag == 1)
    {
        if(view_show == 1)
        {
            File_oprationw.hide();
            file_vview->hide();
        }
        if(view_show == 2)
        {
            changname.hide();
            save_view->hide();
        }
        if(view_show == 3)
        {
            rename_view->hide();
            rename_w.hide();
        }
    }
    else
    {
        File_oprationw.hide();rename_w.hide();changname.hide();
    }
}
//void voice::on_choose_3_clicked()
//{
//    QString path = QFileDialog::getExistingDirectory(NULL,tr("choose"),"/data",QFileDialog::ShowDirsOnly);
//    QDir dir1(path);
//    QFileInfoList list1 = dir1.entryInfoList();

//    ui->pathname_2->setText(list1.at(0).path());
//    refresh(list1.at(0).path());
//}

void voice::on_begin_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->record_timelength->setText("00:00:00");
    ui->begin->setText(tr("record"));
}

QString voice::record_timelength(int length)
{
    int s,m,h;
    QString ss,mm,hh;
    if(length < 60)
    {
        hh = QString("00");
        mm = QString("00");
        if(length < 10)
        ss = QString("0%1").arg(length);
        else
        ss = QString("%1").arg(length);
    }
    else if(length < 360)
    {
      hh = QString("00");
      m = length/60;
      s = (length%60);
     if(m < 10)
       mm = QString("0%1").arg(m);
     else
       mm = QString("%1").arg(m);
     if(s < 10)
       ss = QString("0%1").arg(s);
     else
       ss = QString("%1").arg(s);
    }
    else
    {
        h = length/360;
        m = (length%360)/60;
        s = (length%360)%60;
        if(h < 10)
            hh = QString("0%1").arg(h);
        else
            hh = QString("%1").arg(h);
        if(m < 10)
            mm = QString("0%1").arg(m);
        else
            mm = QString("%1").arg(m);
        if(s < 10)
            ss = QString("0%1").arg(s);
        else
            ss = QString("%1").arg(s);
    }

     QString str_length = QString("%1:%2:%3").arg(hh).arg(mm).arg(ss);
     return str_length;
}

void voice::on_play_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->play->setText(tr("play"));
    QStringList list = database_w.table_show("voice");
    if(!list.isEmpty())
    {
        savepath = list.at(0);
        savefile = list.at(1);
    }
    if(savepath.isEmpty())
    {
        ui->pathname_2->setText("/data");
    }
    else
    {
       ui->pathname_2->setText(savepath);
    }
    QString p = ui->pathname_2->text();
    refresh(p);
    savepath = "";
}

void voice::language_reload()
{
    ui->retranslateUi(this);
    rename_w.language_reload();
    changname.language_reload();
    File_oprationw.language_reload();
}

void voice::get_nowlength()
{
   if(flag_stop_play == 1)
   {
     kk+=1;
     ui->Slider_voicelength->setValue(kk);

     QString time_s = record_timelength(kk);
     ui->label_timestart->setText(time_s);

     if(kk >= length)
     {
         get_length->stop();
         ui->btn_paly->setIcon(QIcon(":/button_image/voice/start.png"));
         ui->btn_paly->setText("play");
         flag_stop_play = 0;
         ui->label_timeend->setText("00:00:00");
         ui->label_timestart->setText("00:00:00");
     }
    }
}

void voice::on_btn_paly_clicked()
{
    if(ui->combox->currentText() == NULL)
    {
        return;
    }

    QProcess *proc = new QProcess;
    proc->start("killall aplay");
    proc->waitForFinished(-1);

    if((length > kk)&&(kk != 0))
    {
        ui->btn_paly->setText("play");
        ui->btn_paly->setIcon(QIcon(":/button_image/voice/start.png"));
        kk=0;
        flag_stop_play = 0;
        ui->Slider_voicelength->setValue(kk);
        ui->label_timeend->setText("00:00:00");
        ui->label_timestart->setText("00:00:00");
    }
    else
    {
        flag_stop_play = 1;

        //ui->label_12->setText(tr("stop"));
        ui->btn_paly->setText("stop");
        ui->btn_paly->setIcon(QIcon(":/button_image/voice/stop.png"));

        QString path = QString("%1").arg(ui->pathname_2->text());
//        QDir dir(path);
//        QStringList filters;
//        filters << "*.wav";
//        dir.setNameFilters(filters);
//        QFileInfoList list = dir.entryInfoList();

//        QStringList files;

//        if(!refresh_flag)
//        {
//            refresh_flag = true;
//            for(int i = 0; i < list.size(); i++)
//            {
//                files << list.at(i).fileName();
//            }
//            files.removeAll("");
//            ui->combox->clear();
//            ui->combox->addItems(files);
//            ui->combox->setCurrentText(file_name);
//        }

        QString name = ui->combox->currentText();
        QString str = QString("aplay %1%2").arg(path).arg(name);

        proc->start(str);

        get_length->start(1000);
        kk = 0;
        QString pname = QString("%1%2").arg(path).arg(name);

        float len = wav_time(pname);
        QString time_len = QString("%1").arg(len);
        length = time_length(time_len);
        ui->Slider_voicelength->setRange(0, length);
    }
}


float voice::wav_time(QString s)
{
    double len;
    char * filename = s.toLatin1().data();
    cout << filename << endl;
    if (filename != NULL)
    {
        FILE* fp;
        fp = fopen(filename, "rb");
        if (fp != NULL)
        {
            int i;
            int j;
            fseek(fp, 28, SEEK_SET);
            fread(&i, sizeof(4), 1, fp);
            fseek(fp ,0 ,SEEK_END);
            j = ftell(fp) - 44;

            fclose(fp);
            fp = NULL;

            len = (double)j/(double)i;
            int aa = len * 10;
            if((aa % 10) > 4) aa = (aa / 10) + 1;
            else aa /= 10;
            int zhengshu = aa / 60;
            int xiaoshu = aa % 60;
            aa = (zhengshu * 100) + xiaoshu;
            return (float)aa / 100.0;
        }
        else
        {
            std::cout << "open error!!" << std::endl;
        }
    }
    std::cout << "filename error!!" << std::endl;
    return 0;
}

int voice::time_length(QString length)   //处理计算的时间
{
    int mm = length.section('.',0,0).toInt();
    int s = length.section('.',1,1).toInt();
    int h,m,len;
    QString str_h,str_m,str_s;

    if(mm>60)
    {
        h = mm/60;
        m = mm%60;
        //s=s-1;
    }
    else
    {
        h = 0;
        m = mm;
         //s=s-1;
    }
    len = h*60*60+m*60+s;

    if(h>=10)
    str_h = QString("%1").arg(h);
    else
    str_h = QString("0%1").arg(h);

    if(m>=10)
    str_h = QString("%1").arg(m);
    else
    str_m = QString("0%1").arg(m);

    if(s>=10)
    str_s = QString("%1").arg(s);
    else
    str_s = QString("0%1").arg(s);

    QString timeend = QString("%1:%2:%3").arg(str_h).arg(str_m).arg(str_s);
    ui->label_timeend->setText(timeend);
    return len;
}

void voice::record_save(QString filen)   //record save file
{
    changname.get_filename(filen);
    save_fileshow();
}

void voice::on_btn_paly_2_clicked()  //delete
{
    if(!ui->combox->currentText().isEmpty())
    {
        QString str = ui->combox->currentText();
        QString strr = QString(tr("Do you want to delete %1?")).arg(str);
        QMessageBox mesg(QMessageBox::Question,
                         tr("QMessageBox::question()"),
                         tr(strr.toUtf8()),
                         0,this);

        QPushButton *yesButton = mesg.addButton(tr("Yes"), QMessageBox::ActionRole);
        QPushButton *noButton = mesg.addButton(tr("No"),QMessageBox::ActionRole);
        if(screen_flag == 1)
            mesg.move(Width*2/3,Height/3);
        else
            mesg.move(Width/3,Height/3);
        mesg.exec();

        if (mesg.clickedButton() == yesButton) {
            QProcess *pro = new QProcess();
            QString name = ui->combox->currentText();
            QString path = QString("%1").arg(ui->pathname_2->text());
            QString str = QString("rm %1%2").arg(path).arg(name);
            pro->start(str);
           // QMessageBox::information(this,"information",tr("Remove successfully!"));
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("delete successfully!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screen_flag == 1)
                mesg.move(Width*2/3,Height/3);
            else
                mesg.move(Width/3,Height/3);
            mesg.exec();
            refresh(path);
            pro->close();
        }
        else if (mesg.clickedButton() == noButton)
        {
            return;
        }
    }
    else
    {
        //QMessageBox::information(this,"information",tr("No file to delete!"));
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No file to delete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
            mesg.move(Height/3,Width*2/3);
        else
            mesg.move(Width/3,Height/3);
        mesg.exec();
    }
}

void voice::on_btn_rename_clicked()  //rename
{   
    if(!ui->combox->currentText().isEmpty())
    {
        rename_fileshow();
        QString name = ui->combox->currentText();
        QString path = QString("%1").arg(ui->pathname_2->text());
        rename_w.rename_chang(name,path);
    }
    else
    {
       // QMessageBox::information(this,"information",tr("No file to rename!"));
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No file to rename!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
            mesg.move(Height/3,Width*2/3);
        else
            mesg.move(Width/3,Height/3);
        mesg.exec();
    }
}
void voice::update_file()     //updata combox file
{
     QString path = QString("%1").arg(ui->pathname_2->text());
     refresh(path);
}

void voice::on_pushButton_clicked()
{
    if(volume_flag == 1)
    {
      set_volume(0);
      volume_flag = 0;
      ui->pushButton->setIcon(QIcon(":/button_image/voice/sound_cross.svg"));
    }
    else
    {
       int value = ui->Slider_volume->value();
       set_volume(value);
       volume_flag = 1;
       ui->pushButton->setIcon(QIcon(":/button_image/voice/sound_open.svg"));
    }
}

void voice::on_btn_rec_clicked()
{
    QProcess *proc = new QProcess;
    proc->start("killall aplay");
    proc->waitForFinished(-1);

    if(flag)
    {
        s_time = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
        QString path = "/data";
        QString str_name = QString("%1/%2_record.wav").arg(path).arg(s_time);

        QString str = QString("arecord -r 16000 -c 1 -t wav %1").arg(str_name);
        proc->start(str);
        ui->btn_rec->setText(tr("stop"));
        ui->btn_rec->setIcon(QIcon(":/button_image/voice/stop.png"));
        flag = false;
        refresh_flag = false;
        flag_record = 1;

    }
    else
    {
        ui->btn_rec->setText(tr("start"));
        ui->record_timelength->setText("00:00:00");
        flag_record = 0;
        record_length = 0;
        ui->btn_rec->setIcon(QIcon(":/button_image/voice/start.png"));

        file_name = QString("%1_record.wav").arg(s_time);
        flag = true;
        proc->start("killall arecord");
        record_save(file_name);
    }
}


void voice::on_Slider_volume_valueChanged(int value)
{
    //slider_flag = 1;
    emit sliderchange(value);
}

void voice::save_fileshow()
{
    if(screen_flag == 0)
    {
        int w_fun = ui->function->width();
        int w_sta = ui->stackedWidget->width();
        int w = w_fun+(w_sta/2-Width*31/50/2);
        QPoint p = ui->btn_rec->mapToGlobal(QPoint(0, 0));
        int h_btn = p.y()+ui->btn_rec->height();
        int h = Height-(Height - h_btn)-Height*2/3;
        changname.resize(Width*31/50,Height*2/3);
        w = Width*2/9+((Width-Width*2/9)/2-changname.width()/2);
        h = Height/6+((Height-Height/6)/2-changname.height()/2);
        changname.move(w,h);

        changname.show();
        changname.activateWindow();changname.setFocus();
    }
    else
    {
        view_show = 2;
        if(save_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&changname);
            w->setRotation(90);

            save_view = new QGraphicsView(scene);

            save_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            save_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            save_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            changname.resize(Height*2/3,Width*2/3);
            save_view->resize(Width*2/3,Height*2/3);
            changname.show();
            save_view->show();
            save_view->activateWindow();save_view->setFocus();
            changname.activateWindow();changname.setFocus();
            int w1=(Width-Width/6)/2-changname.height()/2;
            int h=Height*2/9+(Height*7/9/2-changname.width()/2);
            save_view->move(w1,h);
            save_flag++;
        }
        else
        {
            changname.show();
            save_view->show();
            save_view->activateWindow();save_view->setFocus();
            changname.activateWindow();changname.setFocus();
            int w1=(Width-Width/6)/2-changname.height()/2;
            int h=Height*2/9+(Height*7/9/2-changname.width()/2);
            save_view->move(w1,h);
        }
    }
}
void voice::rename_fileshow()
{
    if(screen_flag == 0)
    {
        int w_fun = ui->function->width();
        int w_sta = ui->stackedWidget->width();
        int w = w_fun+(w_sta/2-Width*31/50/2);
        QPoint p = ui->stackedWidget->mapToGlobal(QPoint(0, 0));
        int h = p.y();
        rename_w.move(w,h);
        rename_w.show();
        rename_w.activateWindow();rename_w.setFocus();
        rename_w.resize(Width*3/5,ui->stackedWidget->height()*3/4);
    }
    else
    {
        if(rename_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&rename_w);
            w->setRotation(90);

            rename_view = new QGraphicsView(scene);

            rename_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            rename_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            rename_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            rename_w.resize(Height*2/3,Width*2/3);
            rename_view->resize(Width*2/3,Height*2/3);
            rename_w.show();
            rename_view->show();
            int w1=(Width-Width/6)/2-rename_w.height()/2;
            int h=Height-Height*7/9+(Height*7/9/2-rename_w.width()/2);
            rename_view->move(w1,h);
            rename_view->activateWindow();rename_view->setFocus();
            rename_w.activateWindow();rename_w.setFocus();
            rename_flag++;
        }
        else
        {
            view_show = 3;
            rename_w.show();
            rename_view->show();
            rename_view->activateWindow();rename_view->setFocus();
            rename_w.activateWindow();rename_w.setFocus();
            int w1=(Width-Width/6)/2-rename_w.height()/2;
            int h=Height*2/9+(Height*7/9/2-rename_w.width()/2);
            rename_view->move(w1,h);
        }
    }
}

void voice::savew_hide()
{
    view_show = 0;
    if(screen_flag == 1)
    {
      changname.hide();
      save_view->hide();
      this->hide();
      this->show();
    }
    else
    changname.hide();
    this->activateWindow();this->setFocus();
}

void voice::renamew_hide()
{
    view_show = 0;
    if(screen_flag == 1)
    {
      rename_w.hide();
      rename_view->hide();
      this->hide();
      this->show();
    }
    else
    rename_w.hide();
    this->activateWindow();this->setFocus();
}

void voice::voice_font()
{
    qreal realX = screenn->physicalDotsPerInchX();
    qreal realY = screenn->physicalDotsPerInchY();
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
        ui->choose_btn->setIconSize(QSize(40,40));
        ui->pushButton->setIconSize(QSize(40,40));
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
        ui->choose_btn->setIconSize(QSize(32,32));
        ui->pushButton->setIconSize(QSize(32,32));
    }
  ui->play->setFont(font);
  ui->begin->setFont(font);
  ui->pushButton->setFont(font);
  ui->btn_paly->setFont(font);
  ui->btn_paly_2->setFont(font);
  ui->btn_rename->setFont(font);
  ui->label->setFont(font);
  ui->label_2->setFont(font);
  ui->label_timeend->setFont(font);
  ui->label_timestart->setFont(font);
  ui->combox->setFont(font);
  ui->btn_rec->setFont(font);
  ui->record_timelength->setFont(font);
  ui->label_3->setFont(font);
  ui->pathname_2->setFont(font);
}

void voice::closeEvent(QCloseEvent *event)
{
    if(show_num == 0)
    {
        save_fileshow();
        savew_hide();
        rename_fileshow();
        renamew_hide();
        show_num++;
    }
    QWidget::closeEvent(event);
}

void voice::on_choose_btn_clicked()
{
    File_oprationw.filepath_flag = 1;
    file_choose_show();
}

void voice::file_path(QString fpath)
{
    ui->pathname_2->setText(fpath);
    refresh(fpath);
}

void voice::file_choose_show()
{
    if(screen_flag == 1)
    {
        view_show=1;
        if(file_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&File_oprationw);
            w->setRotation(90);

            file_vview = new QGraphicsView(scene);
            file_vview->setWindowFlags(Qt::FramelessWindowHint);//无边框
            file_vview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            file_vview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            file_vview->resize(File_oprationw.height(),File_oprationw.width());
            File_oprationw.show();
            file_vview->show();
            int w1=Width*5/6-File_oprationw.height();
            int h=Height*2/9+(Height*7/9/2-File_oprationw.width()/2);
            file_vview->move(w1,h);
            file_flag++;
        }
        else
        {
            File_oprationw.show();
            file_vview->show();
            int w1=Width*5/6-File_oprationw.height();
            int h=Height*2/9+(Height*7/9/2-File_oprationw.width()/2);
            file_vview->move(w1,h);
        }
    }
    else
    {
        int w = Width*2/9+((Width-Width*2/9)/2-File_oprationw.width()/2);
        QPoint p = ui->stackedWidget->mapToGlobal(QPoint(0, 0));
        int h = p.y();
        File_oprationw.show();File_oprationw.move(w,h);
    }
}

void voice::refile_hide()
{
    view_show = 0;
    if(screen_flag == 1)
    {
        File_oprationw.hide();
        file_vview->hide();
        this->hide();this->show();
        this->activateWindow();this->setFocus();
    }
    else
    {
        File_oprationw.hide();
    }
}

void voice::slider_change(int value)
{
    if(volume_flag == 0)
    {
        volume_flag = 1;
        ui->pushButton->setIcon(QIcon(":/button_image/voice/sound_open.svg"));
    }
    set_volume(value);
}
