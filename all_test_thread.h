#ifndef ALL_TEST_THREAD_H
#define ALL_TEST_THREAD_H

#include <QObject>
#include "all_test_interface.h"
#include "serial_dialog.h"
#include <QTimer>
#include <QThread>
#include <QDateTime>

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
#include <iostream>

class all_test_thread : public QObject
{
    Q_OBJECT
public:
    explicit all_test_thread(QObject *parent = nullptr);
    ~all_test_thread();

signals:
    void send_test_msg(int ,QString );
    void serial_test_msg(QString );

public slots:
    void network_test_thread();
    void usb_test_thread(int num);
    void rtc_test_thread();
    void sdcard_test_thread();
    void camera_test_thread();
    void battary_test_thread();
    void sim_test_thread();
    void audio_test_thread();
    void wifi_test_thread();
    void bluetooth_test_thread();
    void key_test_thread();
    void serial_test_thread_start(serial_config serialTest);
    void serial_test_thread_stop();
    void serialTimer_func();

private:
    bool g_stopFlag;
    QTimer* g_serialTimer;
    serial_config * g_serialConfigPtr;
    QString g_checkName;
    int		g_fd;
    QString g_mode;
};

#endif // ALL_TEST_THREAD_H
