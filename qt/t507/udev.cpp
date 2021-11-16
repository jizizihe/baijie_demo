#include "udev.h"
#include "ui_udev.h"
#include "gpio_interface.h"

//#include "mainwindow.h"
#include "xcombobox.h"

int device_index=0;

udev::udev(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::udev)
{
    ui->setupUi(this);

    proc = new QProcess();
    umount_flag = false;

    find_device();
    ui->label->setText("/media/udisk/");
    on_mount_currentIndexChanged(0);
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
    QString str = QFileDialog::getOpenFileName(NULL,tr("choose"),QString("%1").arg(ui->label->text()));
    QFileInfo info = QFileInfo(str);
    ui->label->setText(info.path());
    file_path = info.path();

    show_file(file_path);
    ui->files->setCurrentText(info.fileName());
}

void udev::on_refresh_clicked()  //清除挂载过移动设备的文件夹，控件内容刷新
{
    find_device();
    if(mount_device.size() != 0)
    {
        on_mount_currentIndexChanged(0);
    }
}

void udev::on_umount_clicked()  //安全退出
{
    QString mount_name = globall[device_index];
    QString user_show_name = user_show.at(device_index);

    QString command = QString("umount %1").arg(mount_name);
    proc->start("bash", QStringList() << "-c" << command);
    proc->waitForFinished(-1);

    if(!proc->waitForFinished())
    {
        QMessageBox::information(NULL,"INFO",QString(tr("umount %1 successful!")).arg(user_show_name));
    }

    find_device();
    umount_flag = true;
    device_index = 0;
    ui->files->clear();
    if(mount_device.size() != 0)
        on_mount_currentIndexChanged(0);
}

void udev::on_cp_clicked()
{
    QString temp = temp = ui->files->currentText();

    QStringList str = temp.split(',');
    temp.clear();
    for(int i = 0; i < str.size(); i++)
    {
        temp.append(str.at(i) + " ");
    }

    QString cp_file = ui->label->text()+"/"+ temp;

    QString cp_to_path = QFileDialog::getExistingDirectory(this);

    if(cp_to_path != "")
    {
        proc->start("bash",QStringList() << "-c" << QString("cp %1 %2 -r").arg(cp_file).arg(cp_to_path));
        bool flag = proc->waitForFinished(-1);
        if(flag)
        {
            QMessageBox::information(NULL,"INFO",tr("CP OK"));

            on_refresh_clicked();
        }

    }
    else
    {
        QMessageBox::information(NULL,"INFO",tr("Please select a legal path!"));
    }
}

void udev::on_mv_clicked()
{
    QString temp = temp = ui->files->currentText();
    QStringList str = temp.split(',');
    temp.clear();
    for(int i = 0; i < str.size(); i++)
    {
        temp.append(str.at(i) + " ");
    }
    QString mv_file = ui->label->text()+"/"+temp;
//    qDebug() << "mv_file:" << mv_file;

    QString mv_to_path = QFileDialog::getExistingDirectory();
//    qDebug() << "mv_to_path :" << mv_to_path;

    if(mv_to_path != "")
    {
        proc->start("bash",QStringList() << "-c" << QString("mv %1 %2 -u").arg(mv_file).arg(mv_to_path));
        bool flag = proc->waitForFinished(-1);
        if(flag)
        {
            QMessageBox::information(NULL,"INFO",tr("MV OK"));
            on_refresh_clicked();
        }
    }
    else
    {
        QMessageBox::information(NULL,"INFO",tr("Please select a legal path!"));
    }
}

void udev::on_del_clicked()
{
    QString temp = temp = ui->files->currentText();
    QStringList str = temp.split(',');
    temp.clear();
    for(int i = 0; i < str.size(); i++)
    {
        temp.append(str.at(i) + " ");
    }
    QString del_file = ui->label->text()+"/"+temp;
//    qDebug() << "del_file:" << del_file;

    int x = QMessageBox::warning(NULL, "critical", tr("Are you sure you want to delete this file ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(QMessageBox::Yes == x)
    {
//        qDebug("You clicked the Yes!!!!!\n");
        proc->start("bash",QStringList() << "-c" << QString("rm %1 -rf").arg(del_file));
        bool flag = proc->waitForFinished(-1);
        if(flag)
        {
            QMessageBox::information(NULL,"INFO",tr("DEL OK"));
            on_refresh_clicked();
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
}


void udev::language_reload()
{
    ui->retranslateUi(this);
}


