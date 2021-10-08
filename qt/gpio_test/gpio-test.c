/*
#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include <sys/mman.h>  
#include <sys/types.h>  
#include <ctype.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
*/
#include "gpio_test.h"


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

void gpio_out(char port, int num, int val)
{
	int	port_num;
	FILE *p=NULL;
	char str[256];

	port_num = calc_port_num(port, num);
	p = fopen("/sys/class/gpio/export","w");
	fprintf(p,"%d",port_num);
	fclose(p);
	sprintf(str, "/sys/class/gpio/gpio%d/direction", port_num);
	p = fopen(str,"w");
	fprintf(p,"out");
	fclose(p);

	sprintf(str, "/sys/class/gpio/gpio%d/value", port_num);
	p = fopen(str,"w");
	fprintf(p,"%d",val>0 ? 1 : 0);
	fclose(p);
}

int gpio_in(char port, int num)
{
	int	port_num;
	FILE *p=NULL;
	int	val;
	char str[256];

	port_num = calc_port_num(port, num);
	p = fopen("/sys/class/gpio/export","w");
	fprintf(p,"%d",port_num);
	fclose(p);
	sprintf(str, "/sys/class/gpio/gpio%d/direction", port_num);
	p = fopen(str,"w");
	fprintf(p,"in");
	fclose(p);

	sprintf(str, "/sys/class/gpio/gpio%d/value", port_num);
	p = fopen(str,"r");
	fscanf(p, "%d", &val);
	fclose(p);

	return (int)val;
}


