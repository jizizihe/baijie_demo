#ifndef UDEV_H
#define UDEV_H

#include <QMainWindow>
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

private slots:
    void on_pushButton_3_clicked();

signals:
    void Mysignal();
private:
    Ui::udev *ui;
};

#endif // UDEV_H
