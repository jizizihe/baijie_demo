#include "voice_savefile_name.h"
#include "ui_voice_savefile_name.h"
#include <QDebug>
#include <QScreen>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

static int g_screenWidth;
static int g_screenHeight;
static int g_screenFlag;
static QString g_beforeName;
static QString g_path;
static QScreen *g_screen;
static QGraphicsView *g_chooseFileView;
static int g_chooseFileFirstFlag;

voice_savefile_name::voice_savefile_name(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice_savefile_name)
{
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();

    if(g_screenWidth < g_screenHeight)
    {
        g_screenFlag = 1;
    }
    setSaveFileFont();
    ui->lbl_pathValue->setText("/data/");
    g_proPath.start("bash");
    g_proPath.write("mkdir /data");
    connect(&g_fileOprationWg,SIGNAL(file_rev_path_msg(QString)),this,SLOT(get_file_path(QString)));
    connect(&g_fileOprationWg,SIGNAL(file_hide_msg()),this,SLOT(choose_file_widget_hide()));
}

voice_savefile_name::~voice_savefile_name()
{
    delete ui;
}

void voice_savefile_name::on_btn_cancel_clicked()
{
    QString str = QString("rm %1 \n").arg(g_beforeName);
    QProcess *pro = new QProcess();
    pro->start(str);
    emit save_back_msg();
}

void voice_savefile_name::languageReload()
{
    ui->retranslateUi(this);
    g_fileOprationWg.languageReload();
}

void voice_savefile_name::getFileName(QString fileName)
{
    ui->lineEdit->setText(fileName);
    QString g_path = "/data";
    QString name = QString("%1").arg(ui->lineEdit->text());
    g_beforeName = QString("%1/%2").arg(g_path).arg(name);
}

void voice_savefile_name::saveFile()
{
    g_path = ui->lbl_pathValue->text();
    QString name = QString("%1").arg(ui->lineEdit->text());
    QString pathFile = QString("%1%2").arg(g_path).arg(name);

    if(pathFile.compare(g_beforeName) != 0)
    {
        QString str = QString("cp %1 %2").arg(g_beforeName).arg(pathFile);
        QString strr = QString("rm %1 \n").arg(g_beforeName);

        QProcess *pro = new QProcess();
        pro->start(str);
        pro->write(strr.toUtf8());
    }
    QStringList list = g_database.tableShow("voice");
    if(!list.isEmpty())
    {
        QString name = list.at(0);
        g_database.deleteTableName("voice",name);
    }
    g_database.insertTableTwo("voice",g_path,name);           //Save the path of the latest recording file to the database
}

void voice_savefile_name::on_btn_ok_clicked()
{
    saveFile();
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Save successfully!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    mesg.resize(g_screenWidth/3,g_screenHeight/3);
    int moveWidth = g_screenWidth*2/9+(g_screenWidth*7/18-mesg.width()/2);
    int moveHeight = g_screenHeight/6+(g_screenHeight*5/12-mesg.height()/2);
    if(g_screenFlag == 1)
        mesg.move(g_screenWidth*2/3,g_screenHeight/3);
    else
        mesg.move(moveWidth,moveHeight);
    mesg.exec();
    emit save_back_msg();
}

void voice_savefile_name::setSaveFileFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realY = g_screen->physicalDotsPerInchY();
    qreal realWidth = g_screenWidth / realX * 2.54;
    qreal realHeight = g_screenHeight / realY *2.54;
    QFont font;
    if(g_screenFlag)
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
        ui->btn_chooseFile->setIconSize(QSize(45,45));
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
        ui->btn_chooseFile->setIconSize(QSize(32,32));
    }
    ui->btn_ok->setFont(font);
    ui->btn_cancel->setFont(font);
    ui->lbl_pathValue->setFont(font);
    ui->lbl_path->setFont(font);
    ui->lbl_file->setFont(font);
    ui->lineEdit->setFont(font);
    ui->btn_chooseFile->setFont(font);
    ui->lbl_saveFile->setFont(font);
}

void voice_savefile_name::on_btn_chooseFile_clicked()
{
    g_fileOprationWg.filePathFlag = 1;
    chooseFileWidgetShow();
}

void voice_savefile_name::get_file_path(QString path)
{
    ui->lbl_pathValue->setText(path);
}

void voice_savefile_name::chooseFileWidgetShow()
{
    if(g_screenFlag == 1)
    {
        if(g_chooseFileFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&g_fileOprationWg);
            w->setRotation(90);

            g_chooseFileView = new QGraphicsView(scene);
            g_chooseFileView->setWindowFlags(Qt::FramelessWindowHint);
            g_chooseFileView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_chooseFileView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            g_chooseFileView->resize(g_fileOprationWg.height(),g_fileOprationWg.width());
            g_fileOprationWg.show();
            g_chooseFileView->show();
            int moveWidth = (g_screenWidth-g_screenWidth/6)/2-this->height()/2;
            int moveHeight = g_screenHeight*2/9+(g_screenHeight*7/9/2-this->width()/2);
            g_chooseFileView->move(moveWidth,moveHeight);
            g_chooseFileFirstFlag++;
        }
        else
        {
            g_fileOprationWg.show();
            g_chooseFileView->show();
            int moveWidth=(g_screenWidth-g_screenWidth/6)/2-this->height()/2;
            int moveHeight=g_screenHeight*2/9+(g_screenHeight*7/9/2-this->width()/2);
            g_chooseFileView->move(moveWidth,moveHeight);
        }
    }
    else
    {
        g_fileOprationWg.resize(g_screenWidth/2,g_screenHeight*2/3);
        g_fileOprationWg.show();
        int moveWidth = g_screenWidth*2/9+(g_screenWidth-g_screenWidth*2/9)/2-(g_fileOprationWg.width()/2);
        int moveHeight = g_screenHeight/6+((g_screenHeight*5/6)/2-g_fileOprationWg.height()/2);
        g_fileOprationWg.move(moveWidth,moveHeight);
    }
}

void voice_savefile_name::choose_file_widget_hide()
{
    if(g_screenFlag == 1)
    {
        g_fileOprationWg.hide();
        g_chooseFileView->hide();
        this->hide();this->show();
        this->activateWindow();this->setFocus();
    }
    else
    {
        g_fileOprationWg.hide();
    }
}
