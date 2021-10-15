#ifndef INTERFACE_GPIO_H
#define INTERFACE_GPIO_H


#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>

int	calc_port_num(char port, int num);

void gpio_out(char port, int num, int val);

void interface_gpio(int status);


#endif // INTERFACE_GPIO_H
