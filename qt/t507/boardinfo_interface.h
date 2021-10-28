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

int get_battery_level();
char *get_battery_status();
float get_CPU_temp();
int get_resolution(int *x,int *y);
char *get_boardname();
char *get_OSname();
void get_QTversion(char *dirpath,char *filename,char *fs_file);
char *get_kernel();



#endif
