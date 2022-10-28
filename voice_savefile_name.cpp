#include "voice_savefile_name.h"
#include "ui_voice_savefile_name.h"
#include <QDebug>
#include <QScreen>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

static int screenWidth;
static int screenHeight;
static int screenFlag;
static QString beforeName;
static QString path;
static QScreen *screen;
static QGraphicsView *chooseFileView;
static int chooseFileFirstFlag;

voice_savefile_name::voice_savefile_name(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice_savefile_name)
{
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;
    }
    saveFileFont();
    ui->lbl_pathValue->setText("/data/");
    proPath.start("bash");
    proPath.write("mkdir /data");
    connect(&File_oprationWg,SIGNAL(file_rev_path_msg(QString)),this,SLOT(get_file_path(QString)));
    connect(&File_oprationWg,SIGNAL(file_hide_msg()),this,SLOT(choose_file_widget_hide()));
}

voice_savefile_name::~voice_savefile_name()
{
    delete ui;
}

void voice_savefile_name::on_btn_cancel_clicked()
{
    QString str = QString("rm %1 \n").arg(beforeName);
    QProcess *pro = new QProcess();
    pro->start(str);
    emit save_back_msg();
}

void voice_savefile_name::languageReload()
{
    ui->retranslateUi(this);
    File_oprationWg.languageReload();
}

void voice_savefile_name::getFileName(QString fileName)
{
    ui->lineEdit->setText(fileName);
    QString path = "/data";
    QString name = QString("%1").arg(ui->lineEdit->text());
    beforeName = QString("%1/%2").arg(path).arg(name);
}

void voice_savefile_name::saveFile()
{
    path = ui->lbl_pathValue->text();
    QString name = QString("%1").arg(ui->lineEdit->text());
    QString pathFile = QString("%1%2").arg(path).arg(name);

    if(pathFile.compare(beforeName) != 0)
    {
        QString str = QString("cp %1 %2").arg(beforeName).arg(pathFile);
        QString strr = QString("rm %1 \n").arg(beforeName);

        QProcess *pro = new QProcess();
        pro->start(str);
        pro->write(strr.toUtf8());
    }
    QStringList list = databaseWg.tableShow("voice");
    if(!list.isEmpty())
    {
        QString name = list.at(0);
        databaseWg.deleteTableName("voice",name);
    }
    databaseWg.insertTableTwo("voice",path,name);           //Save the path of the latest recording file to the database
}

void voice_savefile_name::on_btn_ok_clicked()
{
    saveFile();
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Save successfully!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    mesg.resize(screenWidth/3,screenHeight/3);
    int w = screenWidth*2/9+(screenWidth*7/18-mesg.width()/2);
    int h = screenHeight/6+(screenHeight*5/12-mesg.height()/2);
    if(screenFlag == 1)
        mesg.move(screenWidth*2/3,screenHeight/3);
    else
        mesg.move(w,h);
    mesg.exec();
    emit save_back_msg();
}

void voice_savefile_name::saveFileFont()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = screenWidth / realX * 2.54;
    qreal realHeight = screenHeight / realY *2.54;
    QFont font;
    if(screenFlag)
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
    File_oprationWg.filePathFlag = 1;
    fileChooseWidgetShow();
}

void voice_savefile_name::get_file_path(QString path)
{
    ui->lbl_pathValue->setText(path);
}

void voice_savefile_name::fileChooseWidgetShow()
{
    if(screenFlag == 1)
    {
        if(chooseFileFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&File_oprationWg);
            w->setRotation(90);

            chooseFileView = new QGraphicsView(scene);
            chooseFileView->setWindowFlags(Qt::FramelessWindowHint);
            chooseFileView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            chooseFileView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            chooseFileView->resize(File_oprationWg.height(),File_oprationWg.width());
            File_oprationWg.show();
            chooseFileView->show();
            int moveWidth = (screenWidth-screenWidth/6)/2-this->height()/2;
            int moveHeight = screenHeight*2/9+(screenHeight*7/9/2-this->width()/2);
            chooseFileView->move(moveWidth,moveHeight);
            chooseFileFirstFlag++;
        }
        else
        {
            File_oprationWg.show();
            chooseFileView->show();
            int moveWidth=(screenWidth-screenWidth/6)/2-this->height()/2;
            int moveHeight=screenHeight*2/9+(screenHeight*7/9/2-this->width()/2);
            chooseFileView->move(moveWidth,moveHeight);
        }
    }
    else
    {
        File_oprationWg.resize(screenWidth/2,screenHeight*2/3);
        File_oprationWg.show();
        int moveWidth = screenWidth*2/9+(screenWidth-screenWidth*2/9)/2-(File_oprationWg.width()/2);
        int moveHeight = screenHeight/6+((screenHeight*5/6)/2-File_oprationWg.height()/2);
        File_oprationWg.move(moveWidth,moveHeight);
    }
}

void voice_savefile_name::choose_file_widget_hide()
{
    if(screenFlag == 1)
    {
        File_oprationWg.hide();
        chooseFileView->hide();
        this->hide();this->show();
        this->activateWindow();this->setFocus();
    }
    else
    {
        File_oprationWg.hide();
    }
}
