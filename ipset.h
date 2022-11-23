#ifndef IPSET_H
#define IPSET_H

#include <QMainWindow>
#include "ipset_interface.h"
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QMovie>
#include <QScreen>
#include <QScrollBar>
#include <QListWidget>
#include <QInputDialog>
#include <QDialog>
#include <QMessageBox>
#include <QFileInfo>


namespace Ui {
class ipset;
}

class ipset : public QMainWindow
{
    Q_OBJECT

public:
    explicit ipset(QWidget *parent = 0);
    ~ipset();

    void languageReload();
    void setIpFont();
    void setIpText();
    void setSwitchText();

signals:
    void ipset_back_msg();

private slots:
    void switch_change_flag(bool flag);
    void on_btn_ret_clicked();
    void on_btn_ipShow_clicked();
    void on_btn_setIp_clicked();
    void on_btn_clear_clicked();
    void on_btn_setAutoIp_clicked();
    void on_btn_ok_clicked();
    void on_btn_setStaticIp_clicked();

private:
    Ui::ipset *ui;
};

#endif // IPSET_H
