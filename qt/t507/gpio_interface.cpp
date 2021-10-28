#include "gpio_interface.h"

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
    int fd,value;
    char temp;
	char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/value", gpio);
    fd = open(buf, O_RDONLY);
	if(fd < 0) 
	{
		printf("gpio%d get value failed!\n", gpio);
        return -1;
	}	

    read(fd, &temp, 1);
    if(temp != '0')
	{
		value = 1;
	}	
	else 
	{
		value = 0;
	}
	printf("gpio%d get value succeed!\n", gpio);
	close(fd);

    return value;
}


bool getFileName(unsigned int gpio)
{
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/", gpio);

    DIR *dir = opendir(buf);
    if(dir == NULL) {
        printf("open %s failed \n",buf);
        return false;
    }

    return true;
}

static int SetPositionByLine(FILE *fp, int nLine)
{
    int i = 0;
    char buffer[MAX_BUF + 1];
    fpos_t pos;

    rewind(fp);
    for (i = 0; i < nLine; i++)
    {
        fgets(buffer, MAX_BUF, fp);

    }
    fgetpos(fp, &pos);

    return 0;
}

static int extractname(char * filename,int *save_gpio){
    char buffer[1024];
    FILE *fd;
    size_t bytes_read;
    char  *match;

    if((fd = fopen("/read_gpio","r")) == NULL)
    {
        return -1;
    }
    bytes_read = fread(buffer, 1, sizeof(buffer), fd);

    fclose(fd);
    if ( bytes_read == 0 )
    {
        printf("bytes_read = 0\n");
//        return -1;
    }

    buffer[bytes_read] = '\0';
    match = strstr(buffer, "gpio-");
    if ( match == NULL )
    {
        printf("no gpio- keyword to find!\n");
        return -1;
    }
    sscanf(match, "gpio-%d", save_gpio);
    return *save_gpio;
}

int get_occupied_gpio(int *save_gpio,int nLine)
{
    FILE * gpio_dev_file;
    char buf[1024];


    if ( (gpio_dev_file=fopen("/sys/kernel/debug/gpio", "r")) == NULL )
    {
        printf("open file gpio error!\n");
        return -1;
    }

    SetPositionByLine(gpio_dev_file,nLine+1);

    fgets(buf, MAX_BUF, gpio_dev_file);

    if((gpio_dev_file = fopen("/read_gpio","w")) == NULL)
    {
        printf("open file read_gpio error!\n");
        return -1;
    }
    fputs(buf,gpio_dev_file);
    fclose(gpio_dev_file);

    extractname("/read_gpio",save_gpio);
    return 0;
}


static int get_file_linenum(char * filename)
{
    int pfile = open(filename, O_CREAT | O_RDONLY, 0666);
    if (-1 == pfile)
    {
        return -1;
    }

    char buf[MAX_BUF] = {0};
    int i =0;
    int num = 0;
    int iRet = -1;
    while((iRet = read(pfile, buf, MAX_BUF)) > 0)	//保证文件内容全部读完
    {
        for(i=0;i<MAX_BUF;i++)
        {
            if(buf[i] == '\n')
            {
                num = num + 1;
            }
        }
        memset(buf,0,MAX_BUF);
    }
    printf("%d\n",num);
    close(pfile);
    return num;
}

static int judge(int *a, int i)
{
    int n;

    if (i == n - 1 && *(a + i) == 0)
        return -1;
    else if (*(a + i) == 0)
    {
        return judge(a, i + 1);
    }
    else
        return i;
}

static int CompactIntegers(int *a, int len)
{
    int tempin, temp, newlen = 0;
    for (int i = 0; i < len; i++)
    {
        if (a[i] == 0)
        {
            tempin = judge(a, i);
            newlen=i;
            if (tempin == -1)
                break;
            temp = a[i];
            a[i] = a[tempin];
            a[tempin] = temp;

        }
    }
    return newlen;
}


struct occupied_gpio_s get_debug_gpio()
{
    struct occupied_gpio_s occupied_gpio;
    int i,linenum;
    char *gpiopath = "/sys/kernel/debug/gpio";

    memset(occupied_gpio.gpio,0,MAX_BUF);
    linenum = get_file_linenum(gpiopath);
    printf("linenum = %d\n",linenum);

    for(i = 0;i < linenum;i++)
    {
        if(get_occupied_gpio(occupied_gpio.gpio+i,i) == -1)
        {
            printf("get_occupied_gpio failed\n");
        }
    }

    occupied_gpio.len = CompactIntegers(occupied_gpio.gpio,i-1);

    printf("len = %d\n",occupied_gpio.len);
    printf("i = %d\n",i);
    for(i = 0;i < occupied_gpio.len;i++)
    {
        printf("%d: gpio-%d\n",i,occupied_gpio.gpio[i]);
    }

    return occupied_gpio;
}
