#include "boardinfo_interface.h"

int get_battery_level()
{
	FILE *fd;
	int level;

	fd = fopen(BATTERY_PATH"/capacity", "r");
	if(NULL == fd)
	{
		printf("open battery capacity failed !\n");
		return -1;
	}
	fscanf(fd, "%d", &level);

    //printf("battery level is %d\n",level);
	fclose(fd);
	return level;
}

char *get_battery_status()
{
	FILE *fd;
    char *status;

	fd = fopen(BATTERY_PATH"/status", "r");
	if(NULL == fd)
	{
		printf("open battery status failed !\n");
		return "false";
	}

    status = malloc(sizeof(char) * 10);

    fscanf(fd, "%s", status);
    //printf("battery status is %s\n", buf);
	fclose(fd);

    return status;
}

float get_CPU_temp()
{
    FILE *fd;
    long int num;
    float temp,var;

    fd = fopen(CPU_PATH"/temp","r");
    if(NULL == fd)
    {
        printf("open CPU temperature failed !\n");
        return 0;
    }

    fscanf(fd, "%ld" , &num);
    temp = (float)num/1000;
    var = (int)(temp*10+0.5);
    temp = var/10.0;
    fclose(fd);

    return temp;
}



int get_resolution(int *x,int *y)
{
    int fd;
    struct fb_var_screeninfo screen_info;
    char *path = "/dev/fb0";
    fd = open(path,O_RDWR);
    if(0 == fd)
    {
        printf("open fb0 failed !\n");
        return -1;
    }
    ioctl(fd,FBIOGET_VSCREENINFO,&screen_info);
    *x = screen_info.xres;
    *y = screen_info.yres;
    printf("%d*%d\n",screen_info.xres,screen_info.yres);
    close(fd);
    return 0;
}

char *get_boardname()
{
    FILE *fd;
    char *name;

    fd = fopen("/etc/hostname", "r");
    if(NULL == fd)
    {
        printf("open battery status failed !\n");
        return "false";
    }

    name = malloc(sizeof(char) * 10);

    fscanf(fd, "%s", name);
    //printf("battery status is %s\n", buf);
    fclose(fd);

    return name;
}

char *get_OSname()
{
    FILE *fd;
    char *OS;
    char *path = "/etc/issue";
    fd = fopen(path,"r");
    if(NULL == fd)
    {
        printf("open %s failed !\n",path);
        return "false";
    }
    OS = malloc(sizeof(char) * 10);

    fscanf(fd, "%[^\n]", OS);

    fclose(fd);
    return OS;
}

