#include "gpio_set.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include<poll.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include<sys/epoll.h>

int	calc_port_num(char port, int num)
{
	int	port_num;
	
	if(num > 32)
	{	
		return port_num = -1;
	}

	switch(port){
	case 'a':
	case 'A':
		port_num = num;
		break;
	case 'b':
	case 'B':
		port_num = 32 + num;
		break;
	case 'c':
	case 'C':
		port_num = 64 + num;
		break;
	case 'd':
	case 'D':
		port_num = 96 + num;
		break;
	case 'e':
	case 'E':
		port_num = 128 + num;
		break;
	case 'f':
	case 'F':
		port_num = 160 + num;
		break;
	case 'g':
	case 'G':
		port_num = 192 + num;
		break;
	case 'h':
	case 'H':
		port_num = 224 + num;
		break;
	case 'i':
	case 'I':
		port_num = 256 + num;
		break;
	case 'j':
	case 'J':
		port_num = 288 + num;
		break;
	case 'k':
	case 'K':
		port_num = 320 + num;
		break;
	case 'l':
	case 'L':
		port_num = 352 + num;
		break;
	default:
		port_num = -1;
	}
	return port_num;
}

int gpio_export(unsigned int gpio)
{
	int fd;
	int len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR"/export", O_WRONLY);
	if(fd < 0)
	{
		printf("gpio%d port export failed!\n",gpio);
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

int gpio_unexport(unsigned int gpio)
{
	int fd;
	int len;
	char buf[MAX_BUF];
	fd = open(SYSFS_GPIO_DIR"/unexport",O_WRONLY);
	if(fd < 0)
	{
		printf("gpio%d port unexport failed!\n",gpio);
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

int gpio_set_state(unsigned int gpio, char *state)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf ,sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/direction", gpio);
	fd = open(buf, O_WRONLY);
    if(fd < 0)
	{
		printf("gpio%d set state failed!\n",gpio);
	}
	
	write(fd, state, strlen(state)+1);
	close(fd);
	return 0;
}

int gpio_set_value(unsigned int gpio, int value)
{
	int fd;
	char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/value", gpio);
	fd = open(buf, O_WRONLY);
	if(fd < 0)
	{
		printf("gpio%d set value failed!\n", gpio);
		return fd;
	}

	if(value > 0)
	{
		write(fd, "1", 2);
		printf("gpio%d set high succeed!\n", gpio);
	}
	else
	{
		write(fd, "0", 2);
		printf("gpio%d set low succeed!\n", gpio);
	}
	close(fd);

	return 0;
}

int gpio_get_value(unsigned int gpio)
{
	int fd,temp;
	char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/value", gpio);
	fd = open(buf, O_RDONLY);
	if(fd < 0) 
	{
		printf("gpio%d get value failed!\n", gpio);
		return fd;
	}	

	read(fd, &temp, 2);
	if(temp > 0)
	{
		value = 1;
	}	
	else 
	{
		value = 0;
	}
	printf("gpio%d get value succeed!\n", gpio);
	close(fd);

    return 0;
}

