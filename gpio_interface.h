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

struct occupied_gpio_s
{
    int gpio[MAX_BUF];
    char portnum[MAX_BUF][4];
    int len;
};



int intTostr(int m, char str[]);
int calc_port_num(char port, int num);
void portnum_cal(int num,char gpio_port[]);
int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_state(unsigned int gpio, char *state);
int gpio_set_value(unsigned int gpio, int value);
int gpio_get_state(unsigned int gpio);
int gpio_get_value(unsigned int gpio);
bool getFileName(unsigned int gpio);
struct occupied_gpio_s get_debug_gpio();
int get_occupied_gpio(int *save_gpio,int nLine);

#endif
