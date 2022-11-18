#include "udev.h"
#include "ui_udev.h"
#include <QDebug>
#include <QThread>
#include <QTimer>

static int g_screenWidth;
static int g_screenHeight;
static int g_cpCtFlag;     // 0:null  1:copy  2:cut
static QScreen *g_screen;
static QString g_cpFile;
static QString g_ctFile;
static QStringList g_listFileCheck;
static QTimer *g_timer;

udev::udev(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::udev)
{
    ui->setupUi(this);
    g_proc = new QProcess();

    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();
    setUdevFont();
    g_timer = new QTimer(this);
    connect(g_timer,SIGNAL(timeout()),this,SLOT(find_mount_file()));
    connect(&g_proPath, SIGNAL(readyReadStandardOutput()), this, SLOT(readBashStandardOutputInfo()));
    g_proPath.start("bash");
    find_mount_file();
}

udev::~udev()
{
    delete ui;
}

void udev::on_btn_safeUnplug_clicked()
{
    safeUnplug();
}

void udev::safeUnplug()
{
    g_proPath.write("sync\n");
    g_proPath.write("sync\n");
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("safe unplug successful!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    mesg.move(g_screenWidth/3,g_screenHeight/3);
    mesg.exec();
    g_listFileCheck.clear();
}

void udev::on_btn_cp_clicked()
{
    if(g_cpCtFlag == 2)
    {
        g_ctFile.clear();
    }
    if(g_listFileCheck.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please select the file that you want to copy!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        return;
    }
    copy();
}

void udev::copy()
{
    int isFolderFlag;                   // Whether to include folders
    g_cpCtFlag = 1;
    g_cpFile.clear();
    QString path = ui->lbl_pathValue->text();
    for(int i = 0; i< g_listFileCheck.size();++i)
    {
        QString str = g_listFileCheck.at(i);
        g_cpFile = QString("%1%2%3 ").arg(g_cpFile).arg(path).arg(str);
        str = str.remove(0,str.length()-1);
        if(str == "/")
        {
            isFolderFlag = 1;
        }
    }

    if(isFolderFlag  == 1)
    {
        g_cpFile = QString("cp -r %2 ").arg(g_cpFile);
    }
    else
    {
        g_cpFile = QString("cp %2 ").arg(g_cpFile);
    }

    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Copy complete!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    mesg.move(g_screenWidth/3,g_screenHeight/3);
    mesg.exec();
}

void udev::on_btn_cut_clicked()
{
    cut();
}

void udev::cut()
{
    if(g_cpCtFlag == 1)
    {
        g_cpFile.clear();
    }
    if(g_listFileCheck.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please select the file that you want to cut!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        return;
    }
    g_cpCtFlag = 2; g_ctFile.clear();
    QString path = ui->lbl_pathValue->text();
    for(int i = 0; i< g_listFileCheck.size();++i)
    {
        QString str = g_listFileCheck.at(i);
        g_ctFile = QString("%1%2%3 ").arg(g_ctFile).arg(path).arg(str);
    }

    g_ctFile = QString("mv %1 ").arg(g_ctFile);
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Enter the cut state!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    mesg.move(g_screenWidth/3,g_screenHeight/3);
    mesg.exec();
    g_listFileCheck.clear();
}

void udev::on_btn_delete_clicked()
{
    if(g_listFileCheck.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please select the file that you want to delete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        return;
    }
     remove();
}

void udev::remove()
{
    QString strr = QString(tr("Do you want to delete the file?"));
    QMessageBox mesg(QMessageBox::Question,
                     tr("QMessageBox::question()"),
                     tr(strr.toUtf8()),
                     0,this);

    QPushButton *yesButton = mesg.addButton(tr("Yes"), QMessageBox::ActionRole);
    QPushButton *noButton = mesg.addButton(tr("No"),QMessageBox::ActionRole);
    mesg.move(g_screenWidth/3,g_screenHeight/3);
    mesg.exec();

    if(mesg.clickedButton() == yesButton) {
        int flag;
        QString deleteFile;
        QString path = ui->lbl_pathValue->text();
        for(int i = 0; i< g_listFileCheck.size();++i)
        {
            QString str = g_listFileCheck.at(i);
            deleteFile = QString("%1%2%3 ").arg(deleteFile).arg(path).arg(str);
            str = str.remove(0,str.length()-1);
            if(str == "/")
            {
                flag = 1;
            }
        }
        if(flag == 1)
        {
            deleteFile = QString("rm -r %1 \n").arg(deleteFile);
        }
        else
        {
            deleteFile = QString("rm %1 \n").arg(deleteFile);
        }
        g_proPath.write(deleteFile.toUtf8());
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Delete complete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        g_listFileCheck.clear();
        fileRefresh(path);
    }
    else if (mesg.clickedButton() == noButton)
    {
        return;
    }
}

void udev::on_btn_ret_clicked()
{
    emit udev_back_msg();
    g_timer->stop();
}

void udev::languageReload()
{
    ui->retranslateUi(this);
    ui->lbl_pathValue->setText(g_filePath);
}

void udev::setUdevFont()
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

    ui->btn_cp->setFont(font);
    ui->btn_cut->setFont(font);
    ui->btn_delete->setFont(font);
    ui->lbl_pathValue->setFont(font);
    ui->lbl_path->setFont(font);
    ui->btn_safeUnplug->setFont(font);
    ui->btn_paste->setFont(font);
    ui->btn_mount->setFont(font);
    ui->lbl_storage->setFont(font);
    ui->btn_back->setFont(font);
}

void udev::readBashStandardOutputInfo()
{
    QString out = g_proPath.readAllStandardOutput();
    QStringList list = out.split("\n");
    QString str;
    list.removeAll("");
    ui->treeWidget->clear();
    for(int i = 2;i<list.size();i++)
    {
        QTreeWidgetItem *g = new QTreeWidgetItem(ui->treeWidget);
        g->setText(0,list[i]);
        g->setCheckState(0,Qt::Unchecked);
        str = list[i];
        if(ui->lbl_pathValue->text() == "/media/udisk/")
        {
            str = str.remove(0,str.length()-1);
            if(str == "/")
                g->setIcon(0,QIcon(":/button_image/udisk.svg"));
        }
        else if(ui->lbl_pathValue->text() == "/media/sdcard/")
        {
            str = str.remove(0,str.length()-1);
            if(str == "/")
                g->setIcon(0,QIcon(":/button_image/sdcard.svg"));
        }
        else if((ui->lbl_pathValue->text() == "/media/")&&(str == "udisk/"))
        {
            g->setIcon(0,QIcon(":/button_image/udisk.svg"));
        }
        else if((ui->lbl_pathValue->text() == "/media/")&&(str == "sdcard/"))
        {
            g->setIcon(0,QIcon(":/button_image/sdcard.svg"));
        }
        else
        {
            str = str.remove(0,str.length()-1);
            if(str == "/")
                g->setIcon(0,QIcon(":/button_image/folder.svg"));
            else
                g->setIcon(0,QIcon(":/button_image/file.svg"));
        }
    }
}

void udev::showEvent(QShowEvent *event)
{
    fileRefresh("/");
    g_timer->start(4000);
    QWidget::showEvent(event);
}

void udev::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(Qt::Checked==item->checkState(0))
    {
        if(!g_listFileCheck.isEmpty())
        {
            for(int i = 0;i<g_listFileCheck.size();i++)
            {
                if(item->text(column)==g_listFileCheck.at(i))
                {
                    return;
                }
            }
            g_listFileCheck << item->text(column);
        }

        else
            g_listFileCheck << item->text(column);
    }
    else
    {
        for(int i = 0;i<g_listFileCheck.size();i++)
        {
            if(item->text(column)==g_listFileCheck.at(i))
            {
                g_listFileCheck.removeAt(i);break;
            }
        }
    }
}

void udev::on_btn_back_clicked()
{
    QString filePath = ui->lbl_pathValue->text();
    if(QString::compare(filePath,QString("/"),Qt::CaseSensitive))
    {
        filePath.chop(1);
        filePath = filePath.mid(0,filePath.lastIndexOf("/")+1);
        QString s = "cd "+ filePath + " \n";
        g_proPath.write(s.toUtf8());
        g_proPath.write("ls -ap \n");
        ui->lbl_pathValue->setText(filePath);
        g_listFileCheck.clear();
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

void udev::on_btn_paste_clicked()
{
    paste();
}

void udev::paste()
{
    QString path = ui->lbl_pathValue->text();
    if(!g_cpFile.isEmpty())
    {
        QString str = QString("%1%2 \n").arg(g_cpFile).arg(path);
        g_proPath.write(str.toUtf8());
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Paste complete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        g_cpFile.clear();
        fileRefresh(path);
        g_listFileCheck.clear();
    }
    else if(!g_ctFile.isEmpty())
    {
        QString str = QString("%1%2 \n").arg(g_ctFile).arg(path);
        g_proPath.write(str.toUtf8());
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Paste complete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        g_ctFile.clear();
        fileRefresh(path);
        g_listFileCheck.clear();
    }
    g_cpCtFlag = 0;
}

void udev::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
    QString str = QString(item->text(column));
    QString pathNew = str;
    if(str.remove(0,pathNew.length()-1) == "/")
    {
        QString pathCurrent = ui->lbl_pathValue->text();
        QString path= pathCurrent + pathNew;
        QString s = "cd "+ path + " \n";
        g_proPath.write(s.toUtf8());
        g_proPath.write("ls -ap \n");
        ui->lbl_pathValue->setText(path);
        g_listFileCheck.clear();
    }
    else
    {
        if(Qt::Checked==item->checkState(0))
        {
            item->setCheckState(column,Qt::Unchecked);
        }
        else
        {
            item->setCheckState(column,Qt::Checked);
        }
    }
}

void udev::fileRefresh(QString path)
{
    ui->lbl_pathValue->setText(path);
    QString str = QString("cd %1 \n").arg(path);
    g_proPath.write(str.toUtf8());
    g_proPath.write("ls -ap \n");
}

void udev::on_btn_mount_clicked()
{
    QRegExp reg("/dev/sd[a-z]1");
    QString str = QString("df -h | awk '{print $1}'");
    g_proc->start("bash",QStringList() << "-c" << str);
    g_proc->waitForFinished(-1);
    QString strResult = g_proc->readAllStandardOutput().data();
    QStringList data = strResult.split("\n");

    g_mountDevice.clear();
    int globallIndex = 0;
    for(int x = 0; x < data.size();x++)   //Find the mounted devices
    {
        if(reg.exactMatch(data.at(x)) || data.at(x) == "/dev/mmcblk1p1")
        {
            g_mountDevice << data.at(x);
            g_globall[globallIndex++] = data.at(x);
        }
    }
    if(g_mountDevice.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No device!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
        fileRefresh(ui->lbl_pathValue->text());
        return;
    }
    else
    {
        QString path;
        QDir dir;
        dir.setPath("/media/udisk");
        dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        QFileInfoList list = dir.entryInfoList();
        if (list.count() > 0)
        {
            path = "/media/udisk/";
        }
        dir.setPath("/media/sdcard");
        dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        list = dir.entryInfoList();
        if (list.count() > 0)
        {
            if(path.isEmpty())
            {
                path = "/media/sdcard/";
            }
            else
                path = "/media/";
        }
        g_filePath = QString("%1").arg(path);
        ui->lbl_pathValue->setText(g_filePath);
        fileRefresh(g_filePath);
    }
}

void udev::find_mount_file()
{
    QRegExp reg("/dev/sd[a-z]1");
    QString str_text = QString("df -h | awk '{print $1}'");
    g_proc->start("bash",QStringList() << "-c" << str_text);
    g_proc->waitForFinished(-1);
    QString ss = g_proc->readAllStandardOutput().data();
    QStringList data = ss.split("\n");

    g_mountDevice.clear();
    int globallIndex = 0;
    for(int x = 0; x < data.size();x++)   //Find the mounted devices
    {
        if(reg.exactMatch(data.at(x)) || data.at(x) == "/dev/mmcblk1p1")
        {
            g_mountDevice << data.at(x);
            g_globall[globallIndex++] = data.at(x);
        }
    }
    if(g_mountDevice.isEmpty())
    {
        ui->btn_safeUnplug->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                                          "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                                          "padding: 6px;outline: none; ");
        ui->btn_safeUnplug->setEnabled(false);
        if((ui->lbl_pathValue->text() == "/media/sdcard/")||(ui->lbl_pathValue->text() == "/media/udisk/"))
        {
            fileRefresh(ui->lbl_pathValue->text());
        }
        return;
    }
    else
    {
        ui->btn_safeUnplug->setEnabled(true);
        ui->btn_safeUnplug->setStyleSheet("background-color: rgba(100, 225, 100, 120);border-style: outset;border-width:  2px;"
                                          "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(0, 0, 0, 100);"
                                          "padding: 6px;outline: none; ");

        if((ui->lbl_pathValue->text() == "/media/sdcard/")||(ui->lbl_pathValue->text() == "/media/udisk/"))
        {
            fileRefresh(ui->lbl_pathValue->text());
        }
    }
}
