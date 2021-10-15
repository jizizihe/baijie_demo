#ifndef UDEV_H
#define UDEV_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class udev;
}

class udev : public QMainWindow
{
    Q_OBJECT

public:
    explicit udev(QWidget *parent = 0);
    ~udev();

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
