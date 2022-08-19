#include "udev.h"
#include "ui_udev.h"
#include "xcombobox.h"

static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;
static QString choose_filepath;
static int cp_ct_flag;
static QString cp_file;
static QString cut_file;
static int file_flag;
static QGraphicsView *file_view;
static int view_show;

udev::udev(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::udev)
{
    ui->setupUi(this);

    proc = new QProcess();

    find_device();
    on_mount_currentIndexChanged(0);
    screen = qApp->primaryScreen();
    s_width = screen->size().width();			//屏幕宽
    s_height = screen->size().height();
    if(s_width < s_height)
    {
        screen_flag = 1;
    }
    udev_font();
    connect(&File_oprationw,SIGNAL(file_rev2(QString)),this,SLOT(file_pathre(QString)));
    connect(&File_oprationw,SIGNAL(file_rev2(QString)),this,SLOT(file_pathre2(QString)));
    connect(&File_oprationw,SIGNAL(file_hide()),this,SLOT(re_file_hide()));
}

udev::~udev()
{
    delete ui;
}

void udev::find_device()  //查找插入的设备，并将设备名以通俗的方式显示出来
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

    user_show.clear();
    for(int i=0; i < mount_device.size(); i++) //通俗显示
    {
        if(mount_device.at(i) == "/dev/mmcblk1p1")
        {
            user_show << "sd_device";
        }
        else
        {
            int index = 0;
            char *num = NULL;
            QString tt = mount_device.at(i);
            tt = tt.remove(0,7).left(1);
            QByteArray ba = tt.toLatin1();
            num = ba.data();
            index = *num - 'a' + 1;
            user_show << QString("usb_device_%1").arg(index);
        }
    }
    //由于会清空该控件的内容，所以之前对该控件的信号槽操作必须先断开，才能进行下一步
    disconnect(ui->mount,SIGNAL(currentIndexChanged(int)),this,SLOT(on_mount_currentIndexChanged(int)));
    ui->mount->clear();

    ui->mount->addItems(user_show);
    connect(ui->mount,SIGNAL(currentIndexChanged(int)),this,SLOT(on_mount_currentIndexChanged(int)));
}

void udev::show_file(QString file_path)   //显示当前路径下的文件内容
{
    proc->start("bash",QStringList() << "-c" << QString(" ls -F %1 ").arg(file_path));
    proc->waitForFinished(-1);
    QString ls = proc->readAllStandardOutput().data();
    QStringList ls_data = ls.split("\n");
    ls_data.removeAll("");
    ui->files->clear();
    ui->files->AddItems(ls_data);

}

void udev::on_choose_clicked()   //选择其他路径的文件
{
//    QString str = QFileDialog::getOpenFileName(NULL,tr("choose"),QString("%1").arg(ui->label->text()));
//    QFileInfo info = QFileInfo(str);
//    ui->label->setText(info.path());
//    file_path = info.path();
//    show_file(file_path);
//    ui->files->setCurrentText(info.fileName());
    File_oprationw.filepath_flag = 1;
    file_choose_show();
}

void udev::on_refresh_clicked()  //控件内容刷新
{
    find_device();
    if(mount_device.size() != 0)
    {
        on_mount_currentIndexChanged(0);
    }
}

void udev::file_choose_show()
{
    if(screen_flag == 1)
    {
        if(file_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&File_oprationw);
            w->setRotation(90);

            file_view = new QGraphicsView(scene);
            file_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            file_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            file_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            file_view->resize(File_oprationw.height(),File_oprationw.width());
            File_oprationw.show();
            file_view->show();
            file_view->move(s_width/5,s_height/4);
            file_flag++;
            view_show++;
        }
        else
        {
            File_oprationw.show();
            file_view->show();
            file_view->move(s_width/4,s_width/4);view_show++;
        }
    }
    else
    {
        File_oprationw.show();
    }
}

void udev::re_file_hide()
{
    if(screen_flag == 1)
    {
        File_oprationw.hide();
        file_view->hide();
        this->hide();
        this->show();
        this->activateWindow();this->setFocus();view_show=0;
    }
    else
    {
        File_oprationw.hide();
    }

}

void udev::file_pathre(QString path)
{
    ui->label->setText(path);
    show_file(path);qDebug() << 11;
   // ui->files->setCurrentText(file);
}

void udev::file_pathre2(QString path)
{
    choose_filepath = path;
    if(cp_ct_flag == 1)
    {
        QString cp_to_path = choose_filepath;

        if(cp_to_path != "")
        {
            file_path = cp_to_path;
            ui->label->setText(file_path);
            proc->start("bash",QStringList() << "-c" << QString("cp %1 %2 -r").arg(cp_file).arg(cp_to_path));
            bool flag = proc->waitForFinished(-1);
            if(flag)
            {
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr("CP OK"),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                if(screen_flag == 1)
                mesg.move(s_width*2/3,s_height/3);
                else
                mesg.move(s_width/3,s_height/3);
                mesg.setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
                mesg.exec();
                show_file(file_path);
            }
        }
        if(cp_ct_flag == 2)
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Please select a legal path!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screen_flag == 1)
            mesg.move(s_width*2/3,s_height/3);
            else
            mesg.move(s_width/3,s_height/3);
            mesg.exec();
        }
    }
   if(cp_ct_flag == 2)
    {
        QString cut_to_path = choose_filepath;

        if(cut_to_path != "")
        {
            file_path = cut_to_path;
            ui->label->setText(file_path);

            proc->start("bash",QStringList() << "-c" << QString("mv %1 %2 -u").arg(cut_file).arg(cut_to_path));
            bool flag = proc->waitForFinished(-1);
            if(flag)
            {
                QMessageBox mesg(QMessageBox::Information,
                                 tr("QMessageBox::information()"),
                                 tr("CUT OK!"),
                                 0,this);
                mesg.addButton(tr("OK"),QMessageBox::YesRole);
                if(screen_flag == 1)
                mesg.move(s_width*2/3,s_height/3);
                else
                mesg.move(s_width/3,s_height/3);
                mesg.exec();
                show_file(file_path);
            }
        }
        else
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Please select a legal path!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screen_flag == 1)
            mesg.move(s_width*2/3,s_height/3);
            else
            mesg.move(s_width/3,s_height/3);
            mesg.exec();
        }
    }
}

void udev::on_umount_clicked()  //安全退出
{
    ui->label->clear();
    if(mount_device.size() == 0)
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("All devices have been safely logged out, please check whether the device is inserted, or try to reinsert!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
        mesg.exec();
        return ;
    }
    QString mount_name = globall[device_index];
    QString user_show_name = user_show.at(device_index);

    QString command = QString("umount %1").arg(mount_name);
    proc->start("bash", QStringList() << "-c" << command);
    proc->waitForFinished(-1);

    if(!proc->waitForFinished())
    {
        QString str = QString(tr("umount %1 successful!")).arg(user_show_name);
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr(str.toUtf8()),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
        mesg.exec();
    }

    find_device();
    device_index = 0;
    ui->files->clear();
    if(mount_device.size() != 0)
    on_mount_currentIndexChanged(0);
}

void udev::on_cp_clicked()
{
    cp_file = ui->files->currentText();
    if(ui->files->currentText() == NULL)
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
        mesg.exec();
        cp_ct_flag = 0;
        return;
    }

    QStringList str = cp_file.split(',');
    cp_file.clear();
    for(int i = 0; i < str.size(); i++)
    {
        cp_file.append(ui->label->text()+str.at(i) + " ");
    }
    File_oprationw.filepath_flag = 1;
    file_choose_show();
    cp_ct_flag = 1;
}

void udev::on_cut_clicked()
{
    cut_file = ui->files->currentText();
    if(ui->files->currentText() == NULL)
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
        ui->label->setText(file_path);
        cp_ct_flag = 0;
        return;
    }
    QStringList str = cut_file.split(',');
    cut_file.clear();
    for(int i = 0; i < str.size(); i++)
    {
        cut_file.append(ui->label->text()+str.at(i) + " ");
    }
    File_oprationw.filepath_flag = 1;
    file_choose_show();
    cp_ct_flag = 2;
}

void udev::on_del_clicked()
{
    QString del_file = ui->files->currentText();
    if(del_file == NULL)
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

    QStringList str = del_file.split(',');
    del_file.clear();
    for(int i = 0; i < str.size(); i++)
    {
        del_file.append(ui->label->text()+str.at(i) + " ");
    }
     file_path = ui->label->text();

     QMessageBox mesg(QMessageBox::Question,
                      tr("QMessageBox::question()"),
                      tr("Are you sure you want to delete this file ?"),
                      0,this);
      QPushButton *yesButton = mesg.addButton(tr("Yes"), QMessageBox::ActionRole);
      mesg.addButton(tr("No"),QMessageBox::ActionRole);
      if(screen_flag == 1)
      mesg.move(s_width*2/3,s_height/3);
      else
      mesg.move(s_width/3,s_height/3);
      mesg.exec();
      if (mesg.clickedButton() == yesButton)
      {
          proc->start("bash",QStringList() << "-c" << QString("rm %1 -rf").arg(del_file));
          bool flag = proc->waitForFinished(-1);
          if(flag)
          {
              QMessageBox mesg(QMessageBox::Information,
                               tr("QMessageBox::information()"),
                               tr("DEL OK!"),
                               0,this);
               mesg.addButton(tr("OK"),QMessageBox::YesRole);
               if(screen_flag == 1)
               mesg.move(s_width*2/3,s_height/3);
               else
               mesg.move(s_width/3,s_height/3);
               mesg.exec();
              show_file(file_path);
          }
      }
}

void udev::on_mount_currentIndexChanged(int index)  //更改挂载的外部存储设备随之显示该设备的内容
{
    device_index = index;
    if(mount_device.size() != 0)
    {
        QString open_mount = mount_device[device_index];
        if(mount_device[device_index] != "/dev/mmcblk1p1")
        {
            open_mount = "/media/udisk/"+open_mount.right(4);
        }
        else
        {
           open_mount = "/media/sdcard/"+open_mount.remove(0,5);
        }

        file_path = QString("%1").arg(open_mount);
        ui->label->setText(file_path);
        show_file(file_path);
    }
}

void udev::on_return_2_clicked()
{
    emit Mysignal();
    if(screen_flag == 1)
    {
        if(view_show == 1)
        {
            File_oprationw.hide();qDebug() << 11;
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
   ui->label_3->setFont(font);
   ui->label_4->setFont(font);
   ui->refresh->setFont(font);
   ui->umount->setFont(font);
   ui->files->setFont(font);
   ui->mount->setFont(font);
}
