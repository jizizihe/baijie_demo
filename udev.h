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
#include <QScreen>
#include <QTreeWidgetItem>

namespace Ui {
class udev;
}

class udev : public QMainWindow
{
    Q_OBJECT

public:
    explicit udev(QWidget *parent = 0);
    ~udev();
    void showEvent(QShowEvent *event);
    void languageReload();
    void setUdevFont();
    void fileRefresh(QString path);
    void copy();
    void paste();
    void cut();
    void remove();
    void safeUnplug();

private slots:
    void on_btn_ret_clicked();
    void on_btn_safeUnplug_clicked();
    void on_btn_cp_clicked();
    void on_btn_cut_clicked();
    void on_btn_delete_clicked();
    void readBashStandardOutputInfo();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_btn_back_clicked();
    void on_btn_paste_clicked();
    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);
    void on_btn_mount_clicked();
    void find_mount_file();

signals:
    void udev_back_msg();
private:
    Ui::udev *ui;
    QString g_filePath;
    QProcess *g_proc;
    QString g_globall[27];
    QStringList g_mountDevice;
    QProcess g_proPath;
};

#endif // UDEV_H
