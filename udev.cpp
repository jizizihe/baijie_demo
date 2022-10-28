#include "udev.h"
#include "ui_udev.h"
#include <QDebug>
#include <QThread>
#include <QTimer>

static int screenWidth;
static int screenHeight;
static int screenFlag;
static int cpCtFlag;     // 0:null  1:copy  2:cut
static QScreen *screen;
static QString cpFile;
static QString ctFile;
static QStringList listFileCheck;
static QTimer *timer;

udev::udev(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::udev)
{
    ui->setupUi(this);
    proc = new QProcess();

    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line_2->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    udevFont();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(find_mount_file()));
    connect(&proPath, SIGNAL(readyReadStandardOutput()), this, SLOT(readBashStandardOutputInfo()));
    proPath.start("bash");
    find_mount_file();
}

udev::~udev()
{
    delete ui;
}

void udev::on_btn_safeUnplug_clicked()
{
    proPath.write("sync\n");
    proPath.write("sync\n");
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("safe unplug successful!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    if(screenFlag == 1)
        mesg.move(screenWidth*2/3,screenHeight/3);
    else
        mesg.move(screenWidth/3,screenHeight/3);
    mesg.exec();
    listFileCheck.clear();
}

void udev::on_btn_cp_clicked()
{
    if(cpCtFlag == 2)
    {
        ctFile.clear();
    }
    int isFolderFlag;                   // Whether to include folders
    if(listFileCheck.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please select the file that you want to copy!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        return;
    }
    cpCtFlag = 1;cpFile.clear();
    QString path = ui->lbl_pathValue->text();
    for(int i = 0; i< listFileCheck.size();++i)
    {
        QString str = listFileCheck.at(i);
        cpFile = QString("%1%2%3 ").arg(cpFile).arg(path).arg(str);
        str = str.remove(0,str.length()-1);
        if(str == "/")
        {
            isFolderFlag = 1;
        }
    }

    if(isFolderFlag  == 1)
    {
        cpFile = QString("cp -r %2 ").arg(cpFile);
    }
    else
    {
        cpFile = QString("cp %2 ").arg(cpFile);
    }

    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Copy complete!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    if(screenFlag == 1)
        mesg.move(screenWidth*2/3,screenHeight/3);
    else
        mesg.move(screenWidth/3,screenHeight/3);
    mesg.exec();
}

void udev::on_btn_cut_clicked()
{
    if(cpCtFlag == 1)
    {
        cpFile.clear();
    }
    if(listFileCheck.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please select the file that you want to cut!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        return;
    }
    cpCtFlag = 2; ctFile.clear();
    QString path = ui->lbl_pathValue->text();
    for(int i = 0; i< listFileCheck.size();++i)
    {
        QString str = listFileCheck.at(i);
        ctFile = QString("%1%2%3 ").arg(ctFile).arg(path).arg(str);
    }

    ctFile = QString("mv %1 ").arg(ctFile);
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Enter the cut state!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    if(screenFlag == 1)
        mesg.move(screenWidth*2/3,screenHeight/3);
    else
        mesg.move(screenWidth/3,screenHeight/3);
    mesg.exec();
    listFileCheck.clear();
}

void udev::on_btn_delete_clicked()
{
    if(listFileCheck.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please select the file that you want to delete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        return;
    }
    QString strr = QString(tr("Do you want to delete the file?"));
    QMessageBox mesg(QMessageBox::Question,
                     tr("QMessageBox::question()"),
                     tr(strr.toUtf8()),
                     0,this);

    QPushButton *yesButton = mesg.addButton(tr("Yes"), QMessageBox::ActionRole);
    QPushButton *noButton = mesg.addButton(tr("No"),QMessageBox::ActionRole);
    if(screenFlag == 1)
        mesg.move(screenWidth*2/3,screenHeight/3);
    else
        mesg.move(screenWidth/3,screenHeight/3);
    mesg.exec();

    if(mesg.clickedButton() == yesButton) {
        int flag;
        QString deleteFile;
        QString path = ui->lbl_pathValue->text();
        for(int i = 0; i< listFileCheck.size();++i)
        {
            QString str = listFileCheck.at(i);
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
        proPath.write(deleteFile.toUtf8());
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Delete complete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        listFileCheck.clear();
        fileReflesh(path);
    }
    else if (mesg.clickedButton() == noButton)
    {
        return;
    }
}

void udev::on_btn_ret_clicked()
{
    emit udev_back_msg();
    timer->stop();
}

void udev::languageReload()
{
    ui->retranslateUi(this);
    ui->lbl_pathValue->setText(filePath);
}

void udev::udevFont()
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
    QString out = proPath.readAllStandardOutput();
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
    fileReflesh("/");
    timer->start(4000);
    QWidget::showEvent(event);
}

void udev::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(Qt::Checked==item->checkState(0))
    {
        if(!listFileCheck.isEmpty())
        {
            for(int i = 0;i<listFileCheck.size();i++)
            {
                if(item->text(column)==listFileCheck.at(i))
                {
                    return;
                }
            }
            listFileCheck << item->text(column);
        }

        else
            listFileCheck << item->text(column);
    }
    else
    {
        for(int i = 0;i<listFileCheck.size();i++)
        {
            if(item->text(column)==listFileCheck.at(i))
            {
                listFileCheck.removeAt(i);break;
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
        proPath.write(s.toUtf8());
        proPath.write("ls -ap \n");
        ui->lbl_pathValue->setText(filePath);
        listFileCheck.clear();
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

void udev::on_btn_paste_clicked()
{
    QString path = ui->lbl_pathValue->text();
    if(!cpFile.isEmpty())
    {
        QString str = QString("%1%2 \n").arg(cpFile).arg(path);
        proPath.write(str.toUtf8());
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Paste complete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        cpFile.clear();
        fileReflesh(path); listFileCheck.clear();
    }
    else if(!ctFile.isEmpty())
    {
        QString str = QString("%1%2 \n").arg(ctFile).arg(path);
        proPath.write(str.toUtf8());
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Paste complete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        ctFile.clear();
        fileReflesh(path);listFileCheck.clear();
    }
    cpCtFlag = 0;
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
        proPath.write(s.toUtf8());
        proPath.write("ls -ap \n");
        ui->lbl_pathValue->setText(path);
        listFileCheck.clear();
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

void udev::fileReflesh(QString p)
{
    ui->lbl_pathValue->setText(p);
    QString str = QString("cd %1 \n").arg(p);
    proPath.write(str.toUtf8());
    proPath.write("ls -ap \n");
}

void udev::on_btn_mount_clicked()
{
    QRegExp reg("/dev/sd[a-z]1");
    QString str = QString("df -h | awk '{print $1}'");
    proc->start("bash",QStringList() << "-c" << str);
    proc->waitForFinished(-1);
    QString strResult = proc->readAllStandardOutput().data();
    QStringList data = strResult.split("\n");

    mountDevice.clear();
    int globallIndex = 0;
    for(int x = 0; x < data.size();x++)   //Find the mounted devices
    {
        if(reg.exactMatch(data.at(x)) || data.at(x) == "/dev/mmcblk1p1")
        {
            mountDevice << data.at(x);
            globall[globallIndex++] = data.at(x);
        }
    }
    if(mountDevice.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No device!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
        fileReflesh(ui->lbl_pathValue->text());
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
        filePath = QString("%1").arg(path);
        ui->lbl_pathValue->setText(filePath);
        fileReflesh(filePath);
    }
}

void udev::find_mount_file()
{
    QRegExp reg("/dev/sd[a-z]1");
    QString str_text = QString("df -h | awk '{print $1}'");
    proc->start("bash",QStringList() << "-c" << str_text);
    proc->waitForFinished(-1);
    QString ss = proc->readAllStandardOutput().data();
    QStringList data = ss.split("\n");

    mountDevice.clear();
    int globallIndex = 0;
    for(int x = 0; x < data.size();x++)   //Find the mounted devices
    {
        if(reg.exactMatch(data.at(x)) || data.at(x) == "/dev/mmcblk1p1")
        {
            mountDevice << data.at(x);
            globall[globallIndex++] = data.at(x);
        }
    }
    if(mountDevice.isEmpty())
    {
        ui->btn_safeUnplug->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                                          "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                                          "padding: 6px;outline: none; ");
        ui->btn_safeUnplug->setEnabled(false);
        if((ui->lbl_pathValue->text() == "/media/sdcard/")||(ui->lbl_pathValue->text() == "/media/udisk/"))
        {
            fileReflesh(ui->lbl_pathValue->text());
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
            fileReflesh(ui->lbl_pathValue->text());
        }
    }
}
