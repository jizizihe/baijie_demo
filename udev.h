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
#include <QComboBox>
#include <QMessageBox>
#include <QPalette>
#include <qfileinfo.h>
#include "file_opration.h"
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScreen>

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
    void language_reload();
    void udev_font();
    void showEvent(QShowEvent *event);
    void file_reflesh(QString p);

private slots:

    void on_return_2_clicked();

    void on_umount_clicked();

    void on_cp_clicked();
    void on_cut_clicked();
//    void on_mv_clicked();

    void on_del_clicked();

    void readBashStandardOutputInfo();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_clicked();

    void on_btn_paste_clicked();

    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

    void on_btn_mount_clicked();

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
    File_opration File_oprationw;
    int device_index=0;
    QProcess pro_path;

//    QProcess *proc;
//    QString file_name;
//    QString arr;
};

#endif // UDEV_H
