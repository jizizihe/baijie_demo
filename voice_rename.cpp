#include "voice_rename.h"
#include "ui_voice_rename.h"
#include <QDebug>
#include <QScreen>

static int g_screenWidth;
static int g_screenHeight;
static QString g_beforeName;
static QString g_filePath;
static QScreen *g_screen;

voice_rename::voice_rename(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice_rename)
{
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();
    setVoiceRenameFont();
}

voice_rename::~voice_rename()
{
    delete ui;
}

void voice_rename::on_btn_ok_clicked()
{
    QString newName = QString("%1%2").arg(g_filePath).arg(ui->lineEdit->text());
    rename(newName,g_beforeName);
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Remove successfully!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    mesg.move(g_screenWidth/3,g_screenHeight/3);
    mesg.exec();
    emit rename_finish_msg(ui->lineEdit->text());
    emit rename_back_msg();
}

void voice_rename::rename(QString newName,QString beforeName)
{

    if(newName.compare(beforeName) != 0)
    {
        QString str = QString("mv %1 %2").arg(beforeName).arg(newName);
        QProcess *pro = new QProcess();
        pro->start(str);
        pro->waitForFinished(-1);
        pro->close();
    }  
}

void voice_rename::getBeforeFile(QString filename,QString filepath)
{
    g_filePath = filepath;
    ui->lineEdit->setText(filename);
    g_beforeName = QString("%1%2").arg(g_filePath).arg(filename);
}

void voice_rename::on_btn_cel_clicked()
{
    emit rename_back_msg();
}
void voice_rename::languageReload()
{
    ui->retranslateUi(this);
}

void voice_rename::setVoiceRenameFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;
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
    ui->btn_cel->setFont(font);
    ui->btn_ok->setFont(font);
    ui->lbl_rename->setFont(font);
    ui->lineEdit->setFont(font);
}
