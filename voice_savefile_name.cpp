#include "voice_savefile_name.h"
#include "ui_voice_savefile_name.h"
#include <QDebug>
#include <QScreen>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

static int s_width;
static int s_height;
static int screen_flag;
static QString name_start;
static QString path;
static QString filepath;
static QScreen *screen;
static QGraphicsView *file_vview;
static int file_flag;

voice_savefile_name::voice_savefile_name(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice_savefile_name)
{
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    screen = qApp->primaryScreen();
    s_width = screen->size().width();
    s_height = screen->size().height();

    if(s_width < s_height)
    {
        screen_flag = 1;
    }
    voice_savefile_font();
    ui->label_2->setText("/data/");
    pro_pathh.start("bash");
    pro_pathh.write("mkdir /data");
    connect(&File_oprationw,SIGNAL(file_rev2(QString)),this,SLOT(file_path(QString)));
    connect(&File_oprationw,SIGNAL(file_hide()),this,SLOT(file_hide()));
}

voice_savefile_name::~voice_savefile_name()
{
    delete ui;
}

void voice_savefile_name::on_pushButton_2_clicked()
{
    QString str = QString("rm %1 \n").arg(name_start);
    QProcess *pro = new QProcess();
    pro->start(str);
    emit save_back();
}

void voice_savefile_name::language_reload()
{
    ui->retranslateUi(this);
}

void voice_savefile_name::get_filename(QString file_pn)
{
    ui->lineEdit->setText(file_pn);
    QString path1 = "/data";
    QString name1 = QString("%1").arg(ui->lineEdit->text());
    name_start = QString("%1/%2").arg(path1).arg(name1);
}

void voice_savefile_name::save_as()
{
    path = ui->label_2->text();
    QString name = QString("%1").arg(ui->lineEdit->text());
    QString path_name = QString("%1%2").arg(path).arg(name);

    if(path_name.compare(name_start) != 0)
    {
    QString str = QString("cp %1 %2").arg(name_start).arg(path_name);
    QString strr = QString("rm %1 \n").arg(name_start);

    QProcess *pro = new QProcess();
    pro->start(str);
    pro->write(strr.toUtf8());
    }
    QStringList list = database_w.table_show("voice");
    if(!list.isEmpty())
    {
        QString name = list.at(0);
        database_w.delete_record_by_name("voice",name);
    }
    database_w.insert_table2("voice",path,name);
    emit save_path(path,name);
}

void voice_savefile_name::on_pushButton_clicked()
{
    save_as();
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Save successfully!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    if(screen_flag == 1)
    mesg.move(s_width*2/3,s_height/3);
    else
    mesg.move(s_width/3,s_height/3);
    mesg.exec();

    emit save_back();
}

void voice_savefile_name::voice_savefile_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = s_width / realX * 2.54;
    qreal realHeight = s_height / realY *2.54;
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
        ui->opefile_btn->setIconSize(QSize(45,45));
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
        ui->opefile_btn->setIconSize(QSize(32,32));
    }
    ui->pushButton->setFont(font);
    ui->pushButton_2->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->label->setFont(font);
    ui->lineEdit->setFont(font);
    ui->opefile_btn->setFont(font);
    ui->label_4->setFont(font);
}

void voice_savefile_name::on_opefile_btn_clicked()
{
    File_oprationw.filepath_flag = 1;
    file_choose_show();
}

void voice_savefile_name::file_path(QString path)
{
    ui->label_2->setText(path);
}

void voice_savefile_name::file_choose_show()
{
    if(screen_flag == 1)
    {
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
            file_vview->move(s_width/5,s_height/4);
            file_flag++;
        }
        else
        {
            File_oprationw.show();
            file_vview->show();
            file_vview->move(s_width/4,s_height/4);
        }
    }
    else
    {
        File_oprationw.resize(s_width/2,s_height*2/3);
        File_oprationw.move(s_width/2-File_oprationw.width()/2,s_height/2-File_oprationw.height()/2);
        File_oprationw.show();
    }
}

void voice_savefile_name::file_hide()
{
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
