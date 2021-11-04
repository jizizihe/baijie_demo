#ifndef SERIAL_H
#define SERIAL_H

#include <QMainWindow>
#include <QProcess>
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QDateTime>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include<iostream>

#define SerialNumMax 10


namespace Ui {
class serial;
}

static QStringList m_serialPortName;

class serial : public QMainWindow
{
    Q_OBJECT

public:
    explicit serial(QWidget *parent = 0);
    ~serial();

    typedef struct port_num_t
    {
        int port_num = 0;
        int server_client_flag = 0;  // 1--server  2--client
    }port_num_s;

    QCheckBox *SerialCheckBox[SerialNumMax]= {NULL};

    //int port_num = -1;
    static void *thread_serial_port(void *);
    static int set_prop(int fd);
    int port_check_stat = 0, count = 0;

    void serial_thread(port_num_s * port_num_ptr);
    int check_flag[SerialNumMax] = {0};

signals:
    void Mysignal();

private:
    Ui::serial *ui;
    QProcess process;

    QPushButton * retBt;
    QLabel * TitleLabel;
    QLabel * ModeChooseLabel;
    QLabel * PortChooseLabel;
    QComboBox * ModeChooseBox;
    QComboBox * PortChooseBox;
    QComboBox * ConnectBox;
    QPushButton * StartBt;
    QPushButton * StopBt;
    QTextEdit * SerialMsgText;

    QStringList serialPortName;
    int port_count = 0;

    char * port_name_new[5];

private slots:
    void readyReadStandardOutput();

    void StartBt_clicked();
    void StopBt_clicked();
    void retBt_clicked();

};

#endif // SERIAL_H
