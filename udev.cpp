#include "udev.h"
#include "ui_udev.h"
#include "xcombobox.h"
#include <QDebug>
#include <QThread>
#include <QTimer>

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;
static QString choose_filepath;
static int cp_ct_flag;
static QString cp_file;
static QString ct_file;
static QGraphicsView *file_view;
static int view_show;
static QStringList list_filecheck;
static QString mount_file;
static QTimer *timer;

udev::udev(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::udev)
{
    ui->setupUi(this);
    proc = new QProcess();

    //find_device();
   // on_mount_currentIndexChanged(0);
    screen = qApp->primaryScreen();
    s_width = screen->size().width();			//屏幕宽
    s_height = screen->size().height();
    if(s_width < s_height)
    {
        screen_flag = 1;ui->line_2->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    udev_font();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(mount_refresh()));
    connect(&pro_path, SIGNAL(readyReadStandardOutput()), this, SLOT(readBashStandardOutputInfo()));
    pro_path.start("bash");
    mount_refresh();
}

udev::~udev()
{
    delete ui;
}

//void udev::find_device()  //查找插入的设备，并将设备名以通俗的方式显示出来
//{
//    QRegExp reg("/dev/sd[a-z]1");
//    QString str_text = QString("df -h | awk '{print $1}'");

//    proc->start("bash",QStringList() << "-c" << str_text);
//    proc->waitForFinished(-1);
//    QString ss = proc->readAllStandardOutput().data();
//    QStringList data = ss.split("\n");

//    mount_device.clear();
//    int globall_index = 0;
//    for(int x = 0; x < data.size();x++)   //查找设备
//    {
//        if(reg.exactMatch(data.at(x)) || data.at(x) == "/dev/mmcblk1p1")
//        {
//            mount_device << data.at(x);
//            globall[globall_index++] = data.at(x);
//        }
//    }
//    qDebug() << mount_device;
//    user_show.clear();
//    for(int i=0; i < mount_device.size(); i++) //通俗显示
//    {
//        if(mount_device.at(i) == "/dev/mmcblk1p1")
//        {
//            user_show << "sd_device";
//        }
//        else
//        {
//            int index = 0;
//            char *num = NULL;
//            QString tt = mount_device.at(i);
//            tt = tt.remove(0,7).left(1);
//            QByteArray ba = tt.toLatin1();
//            num = ba.data();
//            index = *num - 'a' + 1;
//            user_show << QString("usb_device_%1").arg(index);
//        }
//    }
//   // 由于会清空该控件的内容，所以之前对该控件的信号槽操作必须先断开，才能进行下一步
//    disconnect(ui->mount,SIGNAL(currentIndexChanged(int)),this,SLOT(on_mount_currentIndexChanged(int)));
//    ui->mount->clear();

//    ui->mount->addItems(user_show);
//    connect(ui->mount,SIGNAL(currentIndexChanged(int)),this,SLOT(on_mount_currentIndexChanged(int)));
//}


//void udev::on_refresh_clicked()  //控件内容刷新
//{
//    find_device();
//    if(mount_device.size() != 0)
//    {
//        on_mount_currentIndexChanged(0);
//    }
//}

void udev::on_umount_clicked()  //安全退出
{
    // int flag;
    //    QString str;
    //    if(list_filecheck.isEmpty())
    //    {
    //        QMessageBox mesg(QMessageBox::Information,
    //                         tr("QMessageBox::information()"),
    //                         tr("Please select the file that you want to safe exit!"),
    //                         0,this);
    //        mesg.addButton(tr("OK"),QMessageBox::YesRole);
    //        if(screen_flag == 1)
    //        mesg.move(s_width*2/3,s_height/3);
    //        else
    //        mesg.move(s_width/3,s_height/3);
    //        mesg.exec();
    //        return;
    //    }
    //    QString path = ui->label->text();//qDebug() << path;
    //    if((!QString::compare(QString(path),QString("/media/udisk/"),Qt::CaseSensitive)) || (!QString::compare(QString(path),QString("/media/sdcard/"),Qt::CaseSensitive)))
    //    {

    //        QString strr = "df | awk '{print $1}'|grep /dev ";
    //        proc->start("bash", QStringList() << "-c" << strr);
    //        proc->waitForFinished(-1);

    //        QString out = proc->readAllStandardOutput();
    //        QStringList list = out.split("\n");
    //        list.removeAll("");
    //        //qDebug() << list;

    //        QString umount_file;
    //        for(int i = 0; i< list_filecheck.size();i++)
    //        {
    //            QString dev = QString(list_filecheck.at(i));
    //            dev.remove(dev.length()-1,dev.length()-2);
    //            dev = QString("/dev/%1").arg(dev);
    //            for(int j=0;j<list.size();j++)
    //            {
    //                if(dev == list.at(j))
    //                {
    //                   // flag = 1;
    //                    str = list_filecheck.at(i);
    //                    umount_file = QString("%1%2%3 ").arg(umount_file).arg("/dev/").arg(str);
    //                }
    //            }

    //        }
    //        if(flag == 1)
    //        {
    //  umount_file = QString("%1%2%3 ").arg(umount_file).arg("/dev/").arg(str);
    // }
    //        else
    //        {
    //            QMessageBox mesg(QMessageBox::Information,
    //                             tr("QMessageBox::information()"),
    //                             tr("It is safe exit!"),
    //                             0,this);
    //            mesg.addButton(tr("OK"),QMessageBox::YesRole);
    //            if(screen_flag == 1)
    //                mesg.move(s_width*2/3,s_height/3);
    //            else
    //                mesg.move(s_width/3,s_height/3);
    //            mesg.exec();
    //            return;
    //        }


//    QRegExp reg("/dev/sd[a-z]1");
//    QString str_text = QString("df -h | awk '{print $1}'");
//    proc->start("bash",QStringList() << "-c" << str_text);
//    proc->waitForFinished(-1);
//    QString ss = proc->readAllStandardOutput().data();
//    QStringList data = ss.split("\n");

//    mount_device.clear();
//    int globall_index = 0;
//    for(int x = 0; x < data.size();x++)   //查找设备
//    {
//        if(reg.exactMatch(data.at(x)) || data.at(x) == "/dev/mmcblk1p1")
//        {
//            mount_device << data.at(x);
//            globall[globall_index++] = data.at(x);
//        }
//    }
//    if(mount_device.isEmpty())
//    {
//        ui->umount->setStyleSheet("background-color: rgba(211, 215, 207,180);");
//        ui->umount->setEnabled(false);
//        return;
//    }
    pro_path.write("sync\n");
    pro_path.write("sync\n");
    // umount_file = QString("umount %1 \n").arg(umount_file);qDebug() << umount_file;
    // pro_path.write(umount_file.toUtf8());
    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("safe unplug successful!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
    else
        mesg.move(s_width/3,s_height/3);
    mesg.exec();
    list_filecheck.clear();
    //        file_reflesh(path);
    //    }
    //    else
    //    {
    //        QMessageBox mesg(QMessageBox::Information,
    //                         tr("QMessageBox::information()"),
    //                         tr("Please select the correct path!"),
    //                         0,this);
    //        mesg.addButton(tr("OK"),QMessageBox::YesRole);
    //        if(screen_flag == 1)
    //            mesg.move(s_width*2/3,s_height/3);
    //        else
    //            mesg.move(s_width/3,s_height/3);
    //        mesg.exec();
    //        return;
    //    }
}

void udev::on_cp_clicked()
{
    if(cp_ct_flag == 2)
    {
         ct_file.clear();
    }
    int flag;
    if(list_filecheck.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please select the file that you want to copy!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
       // QTimer::singleShot(1500,mesg,SLOT(close()));
       // mesg.button(QMessageBox::Ok)->hide();

        mesg.exec();
      //  QThread::sleep(2);
      //  mesg.close();
        return;
    }
    cp_ct_flag = 1;cp_file.clear();
    QString path = ui->label->text();
    for(int i = 0; i< list_filecheck.size();++i)
    {
        QString str = list_filecheck.at(i);
        cp_file = QString("%1%2%3 ").arg(cp_file).arg(path).arg(str);
        str = str.remove(0,str.length()-1);
        if(str == "/")
        {
            flag = 1;
        }
    }

    if(flag  == 1)
    {
       cp_file = QString("cp -r %2 ").arg(cp_file);
    }
    else
    {
       cp_file = QString("cp %2 ").arg(cp_file);
    }

    QMessageBox mesg(QMessageBox::Information,
                     tr("QMessageBox::information()"),
                     tr("Copy complete!"),
                     0,this);
    mesg.addButton(tr("OK"),QMessageBox::YesRole);
    if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
    else
        mesg.move(s_width/3,s_height/3);
    mesg.exec();
   // list_filecheck.clear();
   // file_reflesh("/");
}

void udev::on_cut_clicked()
{
     if(cp_ct_flag == 1)
     {
         cp_file.clear();
     }
     if(list_filecheck.isEmpty())
     {
         QMessageBox mesg(QMessageBox::Information,
                          tr("QMessageBox::information()"),
                          tr("Please select the file that you want to cut!"),
                          0,this);
         mesg.addButton(tr("OK"),QMessageBox::YesRole);
         if(screen_flag == 1)
         mesg.move(s_width*2/3,s_height/3);
         else
         mesg.move(s_width/3,s_height/3);
         mesg.exec();
         return;
     }
     cp_ct_flag = 2;ct_file.clear();
     QString path = ui->label->text();
     for(int i = 0; i< list_filecheck.size();++i)
     {
         QString str = list_filecheck.at(i); //qDebug() << str;
         ct_file = QString("%1%2%3 ").arg(ct_file).arg(path).arg(str);
     }

     ct_file = QString("mv %1 ").arg(ct_file);

     QMessageBox mesg(QMessageBox::Information,
                      tr("QMessageBox::information()"),
                      tr("Enter the cut state!"),
                      0,this);
     mesg.addButton(tr("OK"),QMessageBox::YesRole);
     if(screen_flag == 1)
         mesg.move(s_width*2/3,s_height/3);
     else
         mesg.move(s_width/3,s_height/3);
     mesg.exec();
     list_filecheck.clear();
   //  file_reflesh("/");
}

void udev::on_del_clicked()
{
    if(list_filecheck.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("Please select the file that you want to delete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
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
     if(screen_flag == 1)
     mesg.move(s_width*2/3,s_height/3);
     else
     mesg.move(s_width/3,s_height/3);
     mesg.exec();

       if (mesg.clickedButton() == yesButton) {
           int flag;
           QString del_file;
           QString path = ui->label->text();
           for(int i = 0; i< list_filecheck.size();++i)
           {
               QString str = list_filecheck.at(i);
               del_file = QString("%1%2%3 ").arg(del_file).arg(path).arg(str);
               str = str.remove(0,str.length()-1);
               if(str == "/")
               {
                   flag = 1;
               }
           }
           if(flag == 1)
           {
              del_file = QString("rm -r %1 \n").arg(del_file);
           }
           else
           {
              del_file = QString("rm %1 \n").arg(del_file);
           }
           pro_path.write(del_file.toUtf8());
           QMessageBox mesg(QMessageBox::Information,
                            tr("QMessageBox::information()"),
                            tr("Delete complete!"),
                            0,this);
           mesg.addButton(tr("OK"),QMessageBox::YesRole);
           if(screen_flag == 1)
               mesg.move(s_width*2/3,s_height/3);
           else
               mesg.move(s_width/3,s_height/3);
           mesg.exec();
           list_filecheck.clear();
           file_reflesh(path);
       }
       else if (mesg.clickedButton() == noButton)
       {
           return;
       }
}

//void udev::on_mount_currentIndexChanged(int index)  //更改挂载的外部存储设备随之显示该设备的内容
//{
//    device_index = index;
//    if(mount_device.size() != 0)
//    {
//        QString open_mount = mount_device[device_index];
//        if(mount_device[device_index] != "/dev/mmcblk1p1")
//        {
//            open_mount = "/media/udisk/"+open_mount.right(4);
//        }
//        else
//        {
//           open_mount = "/media/sdcard/"+open_mount.remove(0,5);
//        }

//        file_path = QString("%1").arg(open_mount);
//        ui->label->setText(file_path);
//        file_reflesh(file_path);
//    }
//}

void udev::on_return_2_clicked()
{
    emit Mysignal();
    timer->stop();
    if(screen_flag == 1)
    {
        if(view_show == 1)
        {
            File_oprationw.hide();
            file_view->hide();
        }
    }
    else
    {
        File_oprationw.hide();
    }
}

void udev::language_reload()
{
    File_oprationw.language_reload();
    ui->retranslateUi(this);
    ui->label->setText(file_path);
}

void udev::udev_font()
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
   ui->cp->setFont(font);
   ui->cut->setFont(font);
   ui->del->setFont(font);
   ui->label->setFont(font);
   ui->label_2->setFont(font);
   ui->umount->setFont(font);
   ui->btn_paste->setFont(font);
   ui->btn_mount->setFont(font);
   ui->label_3->setFont(font);
}

void udev::readBashStandardOutputInfo()
{
    QString out = pro_path.readAllStandardOutput();
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
        if(ui->label->text() == "/media/udisk/")
        {
            str = str.remove(0,str.length()-1);
            if(str == "/")
                g->setIcon(0,QIcon(":/button_image/up.svg"));
        }
        else if(ui->label->text() == "/media/sdcard/")
        {
            str = str.remove(0,str.length()-1);
            if(str == "/")
                g->setIcon(0,QIcon(":/button_image/sdcard.svg"));
        }
        else if((ui->label->text() == "/media/")&&(str == "udisk/"))
        {
            g->setIcon(0,QIcon(":/button_image/up.svg"));
        }
        else if((ui->label->text() == "/media/")&&(str == "sdcard/"))
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
    file_reflesh("/");
    timer->start(5000);
    QWidget::showEvent(event);
}

void udev::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(Qt::Checked==item->checkState(0))
    {
        if(!list_filecheck.isEmpty())
        {
            for(int i = 0;i<list_filecheck.size();i++)
            {
                if(item->text(column)==list_filecheck.at(i))
                {
                    return;
                }
            }
            list_filecheck << item->text(column);
        }

        else
            list_filecheck << item->text(column);
    }
    else
    {
        for(int i = 0;i<list_filecheck.size();i++)
        {
            if(item->text(column)==list_filecheck.at(i))
            {
                list_filecheck.removeAt(i);break;
            }
        }
    }
    qDebug() << list_filecheck;
}

void udev::on_pushButton_clicked()
{
    QString file_path = ui->label->text();
    if(QString::compare(file_path,QString("/"),Qt::CaseSensitive))
    {
      file_path.chop(1);
      file_path = file_path.mid(0,file_path.lastIndexOf("/")+1);

      QString s = "cd "+ file_path + " \n";
      pro_path.write(s.toUtf8());
      pro_path.write("ls -ap \n");
      ui->label->setText(file_path);
      list_filecheck.clear();
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("The current path is the most forward path!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
        mesg.exec();
    }
}

void udev::on_btn_paste_clicked()
{
   QString path = ui->label->text();
   if(!cp_file.isEmpty())
   {
       QString str = QString("%1%2 \n").arg(cp_file).arg(path);
       pro_path.write(str.toUtf8());
       //qDebug() << str;
       QMessageBox mesg(QMessageBox::Information,
                        tr("QMessageBox::information()"),
                        tr("Paste complete!"),
                        0,this);
       mesg.addButton(tr("OK"),QMessageBox::YesRole);
       if(screen_flag == 1)
       mesg.move(s_width*2/3,s_height/3);
       else
       mesg.move(s_width/3,s_height/3);
       mesg.exec();
       cp_file.clear();
       file_reflesh(path); list_filecheck.clear();
   }
   else if(!ct_file.isEmpty())
   {
       QString str = QString("%1%2 \n").arg(ct_file).arg(path);
       pro_path.write(str.toUtf8());
       qDebug() << str;
       QMessageBox mesg(QMessageBox::Information,
                        tr("QMessageBox::information()"),
                        tr("Paste complete!"),
                        0,this);
       mesg.addButton(tr("OK"),QMessageBox::YesRole);
       if(screen_flag == 1)
       mesg.move(s_width*2/3,s_height/3);
       else
       mesg.move(s_width/3,s_height/3);
       mesg.exec();
       ct_file.clear();
       file_reflesh(path);list_filecheck.clear();
   }

    cp_ct_flag = 0;
}

void udev::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
    QString str = QString(item->text(column));
    QString path_last = str;
    if(str.remove(0,path_last.length()-1) == "/")
    {
        QString path_pri = ui->label->text();
        QString path= path_pri + path_last;
        QString s = "cd "+ path + " \n";
        pro_path.write(s.toUtf8());
        pro_path.write("ls -ap \n");
        ui->label->setText(path);
        list_filecheck.clear();
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

void udev::file_reflesh(QString p)
{
    ui->label->setText(p);
    QString s = QString("cd %1 \n").arg(p);
    pro_path.write(s.toUtf8());
    pro_path.write("ls -ap \n");
}

void udev::on_btn_mount_clicked()
{
    QRegExp reg("/dev/sd[a-z]1");
    QString str_text = QString("df -h | awk '{print $1}'");
    proc->start("bash",QStringList() << "-c" << str_text);
    proc->waitForFinished(-1);
    QString ss = proc->readAllStandardOutput().data();
    QStringList data = ss.split("\n");

    mount_device.clear();
    int globall_index = 0;
    for(int x = 0; x < data.size();x++)   //查找设备
    {
        if(reg.exactMatch(data.at(x)) || data.at(x) == "/dev/mmcblk1p1")
        {
            mount_device << data.at(x);
            globall[globall_index++] = data.at(x);
        }
    }
    if(mount_device.isEmpty())
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No mount device!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
        mesg.exec();
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
        file_path = QString("%1").arg(path);
        ui->label->setText(file_path);
        file_reflesh(file_path);
    }
}

void udev::mount_refresh()
{
    QRegExp reg("/dev/sd[a-z]1");
    QString str_text = QString("df -h | awk '{print $1}'");
    proc->start("bash",QStringList() << "-c" << str_text);
    proc->waitForFinished(-1);
    QString ss = proc->readAllStandardOutput().data();
    QStringList data = ss.split("\n");

    mount_device.clear();
    int globall_index = 0;
    for(int x = 0; x < data.size();x++)   //查找设备
    {
        if(reg.exactMatch(data.at(x)) || data.at(x) == "/dev/mmcblk1p1")
        {
            mount_device << data.at(x);
            globall[globall_index++] = data.at(x);
        }
    }
    if(mount_device.isEmpty())
    {
        ui->umount->setStyleSheet("background-color: rgba(211, 215, 207,180);border-style: outset;border-width:  2px;"
                                          "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(186, 189, 182);"
                                          "padding: 6px;outline: none; ");
        ui->umount->setEnabled(false);
        return;
    }
    else
    {
        ui->umount->setEnabled(true);
        ui->umount->setStyleSheet("background-color: rgba(100, 225, 100, 120);border-style: outset;border-width:  2px;"
                                 "border-radius: 10px;border-color: rgba(255, 225, 255, 30);color:rgba(0, 0, 0, 100);"
                                 "padding: 6px;outline: none; ");
    }
}
