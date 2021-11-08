#include "udev.h"
#include "ui_udev.h"
#include "gpio_interface.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QTime>
#include <QMessageBox>
#include <QPalette>

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

void udev::find_device()
{
    QRegExp reg("/dev/sd[a-z]1");
    QString str_text = QString("df -h | awk '{print $1}'");

    proc->start("bash",QStringList() << "-c" << str_text);
    proc->waitForFinished(-1);
    QString ss = proc->readAllStandardOutput().data();
    QStringList data = ss.split("\n");

    mount_device.clear();
    int globall_index = 0;
    for(int x = 0; x < data.size();x++)
    {
        if(reg.exactMatch(data.at(x)) || data.at(x) == "/dev/mmcblk1p1")
        {
            mount_device << data.at(x);
//            qDebug() << "mount_device :" << mount_device << __LINE__ ;
            globall[globall_index++] = data.at(x);
        }
    }
//    qDebug() << "mount_device size:" << mount_device.size() << __LINE__;

    user_show.clear();
    for(int i=0; i < mount_device.size(); i++)
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
//        qDebug() << "user see: " << user_show << __LINE__;
//        qDebug() << "******>>>>globall[" << i << "]" << globall[i] << __LINE__;
    }

//    qDebug() << "***************************"<< __LINE__;
    disconnect(ui->mount,SIGNAL(currentIndexChanged(int)),this,SLOT(on_mount_currentIndexChanged(int)));
    ui->mount->clear();
//    qDebug() << "***************************" << __LINE__;

    for(int i = 0; i < user_show.size(); i++)
    {
//        qDebug() << "***************************" << __LINE__;
        ui->mount->addItem(user_show.at(i));
//        qDebug() << i  <<"****************" << user_show.at(i) << __LINE__;
    }
//    qDebug() << "***************************" << __LINE__;
    connect(ui->mount,SIGNAL(currentIndexChanged(int)),this,SLOT(on_mount_currentIndexChanged(int)));
//    qDebug() << "***************************" << __LINE__;
}

void udev::show_file(QString file_path)
{
    proc->start("bash",QStringList() << "-c" << QString(" ls -F %1 ").arg(file_path));
    proc->waitForFinished(-1);
    QString ls = proc->readAllStandardOutput().data();
    QStringList ls_data = ls.split("\n");
    ui->file->clear();
    for(int i = 0; i < ls_data.size(); i ++)
    {
//        qDebug() << "ls_data:" << ls_data.at(i) << __LINE__;
        ui->file->addItem(ls_data.at(i));
    }
}

void udev::on_choose_clicked()
{
    QString str = QFileDialog::getExistingDirectory(NULL,tr("choose"),QString("%1").arg(ui->label->text()));
    ui->label->setText(str);
    file_path = str;

    show_file(file_path);
}

void udev::on_refresh_clicked()
{
    find_device();
//    qDebug() << "***************************" << __LINE__;
//    qDebug() << "***************************" << __LINE__;
    if(mount_device.size() != 0)
    {
        if(umount_flag)
        {
            umount_flag = false;
            insert_device.clear();
//            qDebug() << "!!!!!!!!!!!!!!!!!---->mount_device:"<< mount_device.size()  << mount_device << __LINE__;

            QRegExp reg("sd[a-z]1");
            QString str_text = QString("cat /proc/partitions | awk '{print $4}'");
            proc->start("bash",QStringList() << "-c" << str_text);
            proc->waitForFinished(-1);
            QString str = proc->readAllStandardOutput().data();
            QStringList temp = str.split("\n");

            for(int i = 0; i < temp.size(); i ++)
            {
                if(reg.exactMatch(temp.at(i)) || temp.at(i) == "mmcblk1p1")
                {
                    insert_device << temp.at(i);
                }
            }
//            qDebug() << "insert_device :" << insert_device << __LINE__;

            for(int i = mount_device.size()-1; i >= 0; i --)
            {
                QString s = mount_device.at(i);
                s = s.remove(0,5);
//                qDebug() << "**************************  mount_device:"  << s << __LINE__;
                for(int x = insert_device.size()-1; x >=0; x--)
                {
//                    qDebug() << "**************************" << __LINE__;

                    QString ss = insert_device.at(x);
//                    qDebug() << "**************************" << __LINE__;

                    if(insert_device.at(x) == s)
                    {
//                        qDebug() << "inser_device:" << insert_device.at(x) << __LINE__;
                        insert_device.removeAt(x);
                        mount_device.removeAt(i);
//                        qDebug() << "inser_device:" << insert_device.at(i) << __LINE__;
                    }
                }
            }
//            qDebug() << "insert_device :" << insert_device << __LINE__;

            QProcess pro;
            for(int i = 0; i < insert_device.size(); i ++)
            {
                if(insert_device.at(i) == "mmcblk1p1")
                {
                    QString temp = insert_device.at(i);
//                    qDebug() << "temp :" << temp << __LINE__;
                    pro.start("bash", QStringList() << "-c" << QString("rm -rf /media/sdcard/%1").arg(temp));
                    pro.waitForFinished(-1);
                }
                else
                {
                    QString temp = insert_device.at(i);
//                    qDebug() << "temp :" << temp << __LINE__;
                    pro.start("bash", QStringList() << "-c" << QString("rm -rf /media/udisk/%1").arg(temp));
                    pro.waitForFinished(-1);
                }
            }
        }
        on_mount_currentIndexChanged(0);
    }
}

void udev::on_umount_clicked()
{
//    qDebug() << "***************************"  << "<<< device_index>>>>" << device_index << "****"<< __LINE__;
    QString mount_name = globall[device_index];
//    qDebug() << "in mount button:" << mount_name << __LINE__;

    QString command = QString("umount %1").arg(mount_name);
    proc->start("bash", QStringList() << "-c" << command);
    proc->waitForFinished(-1);

    find_device();
    umount_flag = true;
    device_index = 0;
    ui->file->clear();
    if(mount_device.size() != 0)
        on_mount_currentIndexChanged(0);
}

void udev::on_cp_clicked()
{
    QString cp_file = ui->label->text()+"/"+ui->file->currentText();
    qDebug() << "cp_file:" << cp_file;

    QString cp_to_path = QFileDialog::getExistingDirectory(this);
    qDebug() << "cp_to_path :" << cp_to_path;

    if(cp_to_path != "")
    {
        qDebug() << "this isn't emtey \n";

        proc->start("bash",QStringList() << "-c" << QString("cp %1 %2 -rf").arg(cp_file).arg(cp_to_path));
        bool flag = proc->waitForFinished(-1);
        if(flag)
        {
            QMessageBox::information(NULL,"INFO","CP OK");

            on_refresh_clicked();
        }

    }
}

void udev::on_mv_clicked()
{
    QString mv_file = ui->label->text()+"/"+ui->file->currentText();
//    qDebug() << "mv_file:" << mv_file;

    QString mv_to_path = QFileDialog::getExistingDirectory();
//    qDebug() << "mv_to_path :" << mv_to_path;

    if(mv_to_path != "")
    {
        proc->start("bash",QStringList() << "-c" << QString("mv %1 %2").arg(mv_file).arg(mv_to_path));
        bool flag = proc->waitForFinished(-1);
        if(flag)
        {
            QMessageBox::information(NULL,"INFO","MV OK");
            on_refresh_clicked();
        }
    }
}

void udev::on_del_clicked()
{
    QString del_file = ui->label->text()+"/"+ui->file->currentText();
    qDebug() << "del_file:" << del_file;

    int x = QMessageBox::warning(NULL, "critical", "Are you sure you want to delete this file ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(QMessageBox::Yes == x)
    {
        qDebug("You clicked the Yes!!!!!\n");
        proc->start("bash",QStringList() << "-c" << QString("rm %1 -rf").arg(del_file));
        bool flag = proc->waitForFinished(-1);
        if(flag)
        {
            QMessageBox::information(NULL,"INFO","DEL OK");
            on_refresh_clicked();
        }
    }
    else if(QMessageBox::No == x)
    {
        qDebug("NO***NO***NO***NO***NO***NO***NO***\n");
    }

}

void udev::on_mount_currentIndexChanged(int index)
{
    device_index = index;

//    qDebug() << "----------------------"  << device_index  << "---------------"<< __LINE__;
    if(mount_device.size() != 0)
    {
//        qDebug() << "in combox cout :" << mount_device.at(device_index);
        QString open_mount = mount_device[device_index];
//        qDebug() << "-------------------" << mount_device[device_index] << "--------" << __LINE__;
        if(mount_device[device_index] != "/dev/mmcblk1p1")
        {
            open_mount = "/media/udisk/"+open_mount.right(4);
        }
        else
        {
           open_mount = "/media/sdcard/"+open_mount.remove(0,5);
        }
//        qDebug() << "----------------------" << open_mount << "----------------" << __LINE__;

        file_path = QString("%1").arg(open_mount);
        ui->label->setText(file_path);
        show_file(file_path);
    }
}

void udev::on_return_2_clicked()
{
    emit Mysignal();
}


//void udev::language_reload()
//{
//    ui->retranslateUi(this);
//}


