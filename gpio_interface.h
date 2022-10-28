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

enum PIN_DIRECTION{
    INPUT_PIN=0,
    OUTPUT_PIN=1
};

enum PIN_VALUE{
    LOW=0,
    HIGH=1
};

struct occupiedGpioStr
{
    int gpio[MAX_BUF];
    char portnum[MAX_BUF][4];
    int len;
};

int intToStr(int m, char str[]);
int calcPortNum(char port, int num);
void calcPortStr(int num,char gpioPort[]);
int gpioExport(unsigned int gpio);
int gpioUnexport(unsigned int gpio);
int gpioSetState(unsigned int gpio, char *state);
int gpioSetValue(unsigned int gpio, int value);
int gpioGetState(unsigned int gpio);
int gpioGetValue(unsigned int gpio);
bool getFileName(unsigned int gpio);
struct occupiedGpioStr getDebugGpio();
int getOccupieGpio(int *saveGpio,int nLine);

#endif
