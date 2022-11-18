#ifndef BATTERY_INTERFACE_H
#define BATTERY_INTERFACE_H

#define BATTERY_PATH "/sys/class/power_supply/axp803-battery"
#define CPU_PATH "/sys/class/thermal/thermal_zone0"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<fcntl.h>
#include<linux/fb.h>
#include <dirent.h>
#include <errno.h>

int getBatteryLevel();
char *getBatteryStatus();
float getCPUTemp();
int getResolution(int *x,int *y);
char *getBoardName();
char *getOSName();
int getQTVersion(char *dirPath,char *fileName,char *fsFile);
char *getKernel();

#endif
