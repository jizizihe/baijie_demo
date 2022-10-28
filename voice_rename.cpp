#include "voice_rename.h"
#include "ui_voice_rename.h"
#include <QDebug>
#include <QScreen>

static int screenWidth;
static int screenHeight;
static int screenFlag;
static QString beforeName;
static QString filePath;
static QScreen *screen;

voice_rename::voice_rename(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice_rename)
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
    voiceRenameFont();
}

voice_rename::~voice_rename()
{
    delete ui;
}

void voice_rename::on_btn_ok_clicked()
{
    QString newName = QString("%1%2").arg(filePath).arg(ui->lineEdit->text());
    if(newName.compare(beforeName) != 0)
    {
        QString str = QString("mv %1 %2").arg(beforeName).arg(newName);
        QProcess *pro = new QProcess();
        pro->start(str);
        pro->waitForFinished(-1);
        pro->close();
    }
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Remove successfully!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    if(screenFlag == 1)
        mesg.move(screenWidth*2/3,screenHeight/3);
    else
        mesg.move(screenWidth/3,screenHeight/3);
    mesg.exec();
    emit rename_finish_msg(ui->lineEdit->text());
    emit rename_back_msg();
}

void voice_rename::renameChang(QString filename,QString filepath)
{
    filePath = filepath;
    ui->lineEdit->setText(filename);
    beforeName = QString("%1%2").arg(filePath).arg(filename);
}

void voice_rename::on_btn_cel_clicked()
{
    emit rename_back_msg();
}
void voice_rename::languageReload()
{
    ui->retranslateUi(this);
}

void voice_rename::voiceRenameFont()
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
    ui->lbl_rename->setFont(font);
    ui->lineEdit->setFont(font);
}
