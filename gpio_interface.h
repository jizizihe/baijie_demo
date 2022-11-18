#ifndef __GPIO_INTERFACE_H_
#define __GPIO_INTERFACE_H_

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include<sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <QDebug>

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 256

struct OccupiedGpioStr
{
    int gpio[MAX_BUF];
    char portNum[MAX_BUF][4];
    int len;
};

int intToStr(int m, char str[]);
int calcPortNum(char port, int num);
void calcPortStr(int num,char gpioPort[]);
int gpioExport(unsigned int gpio);
int gpioUnexport(unsigned int gpio);
int setGpioState(unsigned int gpio, char *state);
int setGpioValue(unsigned int gpio, int value);
int getGpioState(unsigned int gpio);
int getGpioValue(unsigned int gpio);
bool getFileName(unsigned int gpio);
struct OccupiedGpioStr getDebugGpio();
int getOccupieGpio(int *saveGpio,int nLine);

#endif
