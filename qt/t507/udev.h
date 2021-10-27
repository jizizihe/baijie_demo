#ifndef UDEV_H
#define UDEV_H

#include <QMainWindow>
#include <QProcess>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>

namespace Ui {
class udev;
}

class udev : public QMainWindow
{
    Q_OBJECT

public:
    explicit udev(QWidget *parent = 0);
    ~udev();
    void language_reload();

private slots:

    void on_usb_detection_clicked();

    void on_sd_detection_clicked();

    void on_sim_detection_clicked();

    void readoutput();

    void on_return_2_clicked();

signals:
    void Mysignal();
private:
    Ui::udev *ui;
    QProcess *proc;
    QString file_name;
    QString array;
};

#endif // UDEV_H
