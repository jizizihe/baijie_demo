#ifndef __GPIO_TEST_H_
#define __GPIO_TEST_H_

#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include <sys/mman.h>  
#include <sys/types.h>  
#include <ctype.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <QString>

#define SDRAM_PHY_Position 0xc0000000

extern int	calc_port_num(char port, int num);
extern void gpio_out(char port, int num, int val);
extern int gpio_in(char port, int num);

#endif
