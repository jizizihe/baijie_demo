#include "file_opration.h"
#include "ui_file_opration.h"
#include <QMessageBox>
#include <QDebug>
#include <QScreen>

static QString g_nextPath;
static QString g_lastPath;
static QString g_filePath;            //  0: Display file  1: Display folder
static int g_screenWidth;
static int g_screenHeight;
static QScreen *g_screen;

File_opration::File_opration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::File_opration)
{
    ui->setupUi(this);
    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();
    this->setMinimumSize(g_screenWidth*2/3,g_screenHeight*2/3);
    this->setMaximumSize(g_screenWidth*2/3,g_screenHeight*2/3);
    setFileOprationFont();
    connect(&g_proPath, SIGNAL(readyReadStandardOutput()), this, SLOT(read_bash_standard_output_info()));
    this->setWindowModality(Qt::ApplicationModal);
    g_proPath.start("bash");
}

File_opration::~File_opration()
{
    delete ui;
    g_proPath.close();
}

void File_opration::read_bash_standard_output_info()
{
    QString out = g_proPath.readAllStandardOutput();
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
    g_filePath = ui->lbl_pathValue->text();
    if(QString::compare(g_filePath,QString("/"),Qt::CaseSensitive))
    {
        g_filePath.chop(1);
        g_filePath = g_filePath.mid(0,g_filePath.lastIndexOf("/")+1);

        QString s = "cd "+ g_filePath + " \n";
        g_proPath.write(s.toUtf8());
        if(filePathFlag == 0)
        {
            g_proPath.write("ls -ap \n");
        }
        else
        {
            g_proPath.write("ls -ap| grep / \n");
        }
        ui->lbl_pathValue->setText(g_filePath);
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("The current path is the most forward path!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
    }
}

void File_opration::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    g_nextPath = QString(item->text(column));
    g_lastPath = ui->lbl_pathValue->text();
    QString path;
    if(filePathFlag == 0)
    {
        if(g_nextPath.contains("/",Qt::CaseSensitive))
        {
            path = g_lastPath + g_nextPath;
            QString s = "cd "+ path + " \n";
            g_proPath.write(s.toUtf8());
            g_proPath.write("ls -ap \n");
            ui->lbl_pathValue->setText(path);
        }
    }
    else
    {
        path = g_lastPath + g_nextPath;
        QString s = "cd "+ path + " \n";
        g_proPath.write(s.toUtf8());
        g_proPath.write("ls -ap| grep / \n");
        ui->lbl_pathValue->setText(path);
    }

}

void File_opration::showEvent(QShowEvent *event)
{
    if(filePathFlag == 0)
    {
        ui->lbl_pathValue->setText("/");
        QString s = "cd / \n";
        g_proPath.write(s.toUtf8());
        g_proPath.write("ls -ap \n");
    }
    else
    {
        ui->lbl_pathValue->setText("/");;
        QString s = "cd / \n";
        g_proPath.write(s.toUtf8());
        g_proPath.write("ls -ap| grep / \n");
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
        if(g_nextPath.contains("/",Qt::CaseSensitive)||(g_nextPath == "/"))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Please choose file!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
        }
        else
        {
            emit file_hide_msg();
            emit file_rev_file_msg(g_lastPath,g_nextPath);         //g_lastPath: path  g_nextPath: file
        }
    }
}

void File_opration::languageReload()
{
    ui->retranslateUi(this);
}

void File_opration::setFileOprationFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;
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
    ui->btn_back->setFont(font);
    ui->btn_cancel->setFont(font);
    ui->btn_choose->setFont(font);
    ui->lbl_path->setFont(font);
    ui->lbl_pathValue->setFont(font);
    ui->treeWidget->setFont(font);
}
