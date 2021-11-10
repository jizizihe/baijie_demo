#ifndef UDEV_H
#define UDEV_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QFileInfoList>
#include <QFileDialog>
#include <QDateTime>
#include <QTime>
#include <QMessageBox>
#include <QPalette>
#include <qfileinfo.h>


//#include <unistd.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>

//#include <sys/mman.h>
//#include <sys/types.h>
//#include <ctype.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <QDebug>



namespace Ui {
class udev;
}

class udev : public QMainWindow
{
    Q_OBJECT

public:
    explicit udev(QWidget *parent = 0);
    ~udev();
    void umount_device();
    void show_file(QString);
    void language_reload();

private slots:
//    void on_new_2_clicked();

//    void on_mount_currentIndexChanged(int index);

//    void on_umount_clicked();

//    void on_cp_clicked();

//    void on_mv_clicked();

//    void on_del_clicked();

    void find_device();

//    void on_refresh_clicked();
//    void on_usb_detection_clicked();

//    void on_sd_detection_clicked();

//    void on_sim_detection_clicked();

//    void readoutput();

    void on_return_2_clicked();

    void on_choose_clicked();

    void on_refresh_clicked();

    void on_umount_clicked();

    void on_cp_clicked();

    void on_mv_clicked();

    void on_del_clicked();

    void on_mount_currentIndexChanged(int index);

signals:
    void Mysignal();

private:
    Ui::udev *ui;
    QString file_path;
    QProcess *proc;
    QString globall[27];

    QStringList insert_device;
    QStringList mount_device;
    QStringList user_show;
    bool umount_flag;

//    QProcess *proc;
//    QString file_name;
//    QString arr;
};

#endif // UDEV_H
