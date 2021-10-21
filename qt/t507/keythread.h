#ifndef KEYTHREAD_H
#define KEYTHREAD_H

#include <QThread>
#include <QDebug>

#include<sstream>
#include<iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <linux/input.h>

char * keytest_scan(char* keypath);
void key_test(char * event);

class keythread : public QThread
{
    Q_OBJECT
public:
    explicit keythread(QObject *parent = nullptr);
    ~keythread();

    void run();//线程入口函数（工作线程的主函数）

signals:

public slots:

};

#endif // KEYTHREAD_H
