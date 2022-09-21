#include "voice_rename.h"
#include "ui_voice_rename.h"
#include <QDebug>
#include <QScreen>

static int s_width;
static int s_height;
static int screen_flag;
static QString name_s;
static QString fpath;
static QScreen *screen;

voice_rename::voice_rename(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice_rename)
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
    voice_rename_font();
}

voice_rename::~voice_rename()
{
    delete ui;
}

void voice_rename::on_btn_ok_clicked()
{
    QString name = QString("%1/%2").arg(fpath).arg(ui->lineEdit->text());
    if(name.compare(name_s) != 0)
    {
        QString str = QString("mv %1 %2").arg(name_s).arg(name);

        QProcess *pro = new QProcess();
        pro->start(str);
        //pro->close();
    }
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Remove successfully!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    if(screen_flag == 1)
    mesg.move(s_width*2/3,s_height/3);
    else
    mesg.move(s_width/3,s_height/3);
    mesg.exec();
    emit rename_finish();
    emit rename_back();
}

void voice_rename::rename_chang(QString filename,QString filepath)
{
    fpath = filepath;
    ui->lineEdit->setText(filename);
    name_s = QString("%1/%2").arg(fpath).arg(filename);
}

void voice_rename::on_btn_cel_clicked()
{
    emit rename_back();
}
void voice_rename::language_reload()
{
    ui->retranslateUi(this);
}

void voice_rename::voice_rename_font()
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
    ui->btn_cel->setFont(font);
    ui->btn_ok->setFont(font);
    ui->label_2->setFont(font);
    ui->lineEdit->setFont(font);
}

