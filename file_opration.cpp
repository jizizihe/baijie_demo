#include "file_opration.h"
#include "ui_file_opration.h"
#include <QMessageBox>
#include <QDebug>
#include <QScreen>

static QString nextPath;
static QString lastPath;
static QString filePath;            //  0: Display file  1: Display folder
static int screenWidth;
static int screenHeight;
static int screenFlag;
static QScreen *screen;

File_opration::File_opration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::File_opration)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    if(screenWidth < screenHeight)
    {
        screenFlag = 1;
        this->setMinimumSize(screenHeight*2/3,screenWidth*3/4);
        this->setMaximumSize(screenHeight*2/3,screenWidth*3/4);
    }
    else
    {
        this->setMinimumSize(screenWidth*2/3,screenHeight*2/3);
        this->setMaximumSize(screenWidth*2/3,screenHeight*2/3);
    }
    connect(&proPath, SIGNAL(readyReadStandardOutput()), this, SLOT(read_bash_standard_output_info()));
    this->setWindowModality(Qt::ApplicationModal);
    proPath.start("bash");
}

File_opration::~File_opration()
{
    delete ui;
    proPath.close();
}

void File_opration::read_bash_standard_output_info()
{
    QString out = proPath.readAllStandardOutput();
    QStringList list = out.split("\n");
    list.removeAll("");

    ui->treeWidget->clear();
    if(filePathFlag == 1)
    {
        if(list.count() < 3)                //no file
        {

        }
        else
        {
            for(int i = 2;i<list.size();i++)
            {
                QTreeWidgetItem *g = new QTreeWidgetItem(ui->treeWidget);
                g->setText(0,list[i]);
            }
        }
    }
    else
    {
        for(int i = 2;i<list.size();i++)
        {
            QTreeWidgetItem *g = new QTreeWidgetItem(ui->treeWidget);
            g->setText(0,list[i]);
        }
    }
}

void File_opration::on_btn_back_clicked()
{
    filePath = ui->lbl_pathValue->text();
    if(QString::compare(filePath,QString("/"),Qt::CaseSensitive))
    {
        filePath.chop(1);
        filePath = filePath.mid(0,filePath.lastIndexOf("/")+1);

        QString s = "cd "+ filePath + " \n";
        proPath.write(s.toUtf8());
        if(filePathFlag == 0)
        {
            proPath.write("ls -ap \n");
        }
        else
        {
            proPath.write("ls -ap| grep / \n");
        }
        ui->lbl_pathValue->setText(filePath);
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("The current path is the most forward path!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
    }
}

void File_opration::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    nextPath = QString(item->text(column));
    lastPath = ui->lbl_pathValue->text();
    QString path;
    if(filePathFlag == 0)
    {
        if(nextPath.contains("/",Qt::CaseSensitive))
        {
            path = lastPath + nextPath;
            QString s = "cd "+ path + " \n";
            proPath.write(s.toUtf8());
            proPath.write("ls -ap \n");
            ui->lbl_pathValue->setText(path);
        }
    }
    else
    {
        path = lastPath + nextPath;
        QString s = "cd "+ path + " \n";
        proPath.write(s.toUtf8());
        proPath.write("ls -ap| grep / \n");
        ui->lbl_pathValue->setText(path);
    }

}

void File_opration::showEvent(QShowEvent *event)
{
    this->move(screenWidth/2-this->width()/2,screenHeight/2-this->height()/2);
    if(filePathFlag == 0)
    {
        ui->lbl_pathValue->setText("/");
        QString s = "cd / \n";
        proPath.write(s.toUtf8());
        proPath.write("ls -ap \n");
    }
    else
    {
        ui->lbl_pathValue->setText("/");;
        QString s = "cd / \n";
        proPath.write(s.toUtf8());
        proPath.write("ls -ap| grep / \n");
    }
    QWidget::showEvent(event);

}
void File_opration::on_btn_cancel_clicked()
{
    emit file_hide_msg();
}

void File_opration::on_btn_choose_clicked()
{
    if(filePathFlag == 1)                                     //Send the path signal
    {
        QString str = ui->lbl_pathValue->text();
        emit file_hide_msg();
        emit file_rev_path_msg(str);
    }
    else                                                      //Send the file signal
    {
        if(nextPath.contains("/",Qt::CaseSensitive)||(nextPath == "/"))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Please choose file!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
        }
        else
        {
            emit file_hide_msg();
            emit file_rev_file_msg(lastPath,nextPath);         //lastPath: path  nextPath: file
        }
    }
}

void File_opration::languageReload()
{
    ui->retranslateUi(this);
}

void File_opration::fileOprationFont()
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
            font.setPointSize(10);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(12);
        }
        else
        {
            font.setPointSize(14);
        }

    }
    else
    {
        if(realWidth < 15)
        {
            font.setPointSize(10);
        }
        else if (realWidth < 17)
        {
            font.setPointSize(12);
        }
        else
        {
            font.setPointSize(14);
        }
    }
    ui->btn_back->setFont(font);
    ui->btn_cancel->setFont(font);
    ui->btn_choose->setFont(font);
    ui->lbl_path->setFont(font);
    ui->lbl_pathValue->setFont(font);
    ui->treeWidget->setFont(font);
}
