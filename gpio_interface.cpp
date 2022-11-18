#include "gpio_interface.h"

int intToStr(int m, char str[])
{
    int i = 0;
    int j = 1;
    int k = 0;
    char c;
    if (m < 0)
        str[i++] = '-';
    m = m > 0 ? m : -m;
    while (m > 0)
    {
        str[i] = '0' + m % 10;
        m = m / 10;
        i++;

    }
    if (str[0] == '-')
        j = 1;
    else
        j = 0;
    k = i - 1;
    while (j < k)
    {
        c = str[j];
        str[j] = str[k];
        str[k] = c;
        j++;
        k--;
    }
    str[i] = '\0';
    return i - 1;
}

int	calcPortNum(char port, int num)
{
    int	portNum;
    if(num > 32)
    {
        return portNum = -1;
    }

    switch(port){
    case 'a':
    case 'A':
        portNum = num;
        break;
    case 'b':
    case 'B':
        portNum = 32 + num;
        break;
    case 'c':
    case 'C':
        portNum = 64 + num;
        break;
    case 'd':
    case 'D':
        portNum = 96 + num;
        break;
    case 'e':
    case 'E':
        portNum = 128 + num;
        break;
    case 'f':
    case 'F':
        portNum = 160 + num;
        break;
    case 'g':
    case 'G':
        portNum = 192 + num;
        break;
    case 'h':
    case 'H':
        portNum = 224 + num;
        break;
    case 'i':
    case 'I':
        portNum = 256 + num;
        break;
    case 'j':
    case 'J':
        portNum = 288 + num;
        break;
    case 'k':
    case 'K':
        portNum = 320 + num;
        break;
    case 'l':
    case 'L':
        portNum = 352 + num;
        break;
    default:
        portNum = -1;
    }
    return portNum;
}

void calcPortStr(int num, char gpioPort[])
{
    if(num <= 32)
    {
        gpioPort[0] = 'a';
        intToStr(num,&gpioPort[1]);
    }
    else if(num > 32 && num <= 64)
    {
        gpioPort[0] = 'b';
        intToStr((num-32),&gpioPort[1]);
    }
    else if(num > 64 && num <= 96)
    {
        gpioPort[0] = 'c';
        intToStr((num-64),&gpioPort[1]);
    }
    else if(num > 96 && num <= 128)
    {
        gpioPort[0] = 'd';
        intToStr((num-96),&gpioPort[1]);
    }
    else if(num > 128 && num <= 160)
    {
        gpioPort[0] = 'e';
        intToStr((num-128),&gpioPort[1]);
    }
    else if(num > 160 && num <= 192)
    {
        gpioPort[0] = 'f';
        intToStr((num-160),&gpioPort[1]);
    }
    else if(num > 192 && num <= 224)
    {
        gpioPort[0] = 'g';
        intToStr((num-192),&gpioPort[1]);
    }
    else if(num > 224 && num <= 256)
    {
        gpioPort[0] = 'h';
        intToStr((num-224),&gpioPort[1]);
    }
    else if(num > 256 && num <= 288)
    {
        gpioPort[0] = 'i';
        intToStr((num-256),&gpioPort[1]);
    }
    else if(num > 288 && num <= 320)
    {
        gpioPort[0] = 'j';
        intToStr((num-288),&gpioPort[1]);
    }
    else if(num > 320 && num <= 352)
    {
        gpioPort[0] = 'k';
        intToStr((num-320),&gpioPort[1]);
    }
    else if(num > 352 && num <= 384)
    {
        gpioPort[0] = 'l';
        intToStr((num-352),&gpioPort[1]);
    }
}

int gpioExport(unsigned int gpio)
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

int gpioUnexport(unsigned int gpio)
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

int setGpioState(unsigned int gpio, char *state)
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

int setGpioValue(unsigned int gpio, int value)
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

int getGpioValue(unsigned int gpio)
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
    close(fd);
    return value;
}

int getGpioState(unsigned int gpio)
{
    int fd,value;
    char temp;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/direction", gpio);
    fd = open(buf, O_RDONLY);
    if(fd < 0)
    {
        //printf("gpio%d get direction failed!\n", gpio);
        return -1;
    }
    read(fd, &temp, 1);
    if(temp != 'i')
    {
        value = 1;
    }
    else
    {
        value = 0;
    }
    close(fd);
    return value;
}

bool getFileName(unsigned int gpio)
{
    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR"/gpio%d/", gpio);
    DIR *dir = opendir(buf);
    if(dir == NULL) {
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

int getOccupieGpio(int *saveGpio, int nLine)
{
    FILE * gpioDevFile;
    char buf[MAX_BUF];
    char  *match;

    if ( (gpioDevFile=fopen("/sys/kernel/debug/gpio", "r")) == NULL )
    {
        printf("open file gpio error!\n");
        return -1;
    }

    SetPositionByLine(gpioDevFile,nLine+1);
    fgets(buf, MAX_BUF, gpioDevFile);
    match = strstr(buf, "gpio-");
    if ( match == NULL )
    {
        return -1;
    }
    sscanf(match, "gpio-%d", saveGpio);
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
    while((iRet = read(pfile, buf, MAX_BUF)) > 0)
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
    //    printf("%d\n",num);
    close(pfile);
    return num;
}

static int judge(int *a, int i)
{
    int n;
    if (i == n - 1 && *(a + i) == 0)
    {
        return -1;
    }
    else if (*(a + i) == 0)
    {
        return judge(a, i + 1);
    }
    else
    {
        return i;
    }
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

OccupiedGpioStr getDebugGpio()
{
    struct OccupiedGpioStr occupiedGpio;
    int i,linenum;
    char *gpiopath = (char *)"/sys/kernel/debug/gpio";

    memset(occupiedGpio.gpio,0,MAX_BUF);
    linenum = get_file_linenum(gpiopath);

    for(i = 0;i < linenum;i++)
    {
        if(getOccupieGpio(occupiedGpio.gpio+i,i) == -1)
        {
            //            printf("get_occupied_gpio failed\n");
        }
    }
    occupiedGpio.len = CompactIntegers(occupiedGpio.gpio,i-1);
    for(i = 0;i < occupiedGpio.len;i++)
    {
        calcPortStr(occupiedGpio.gpio[i],occupiedGpio.portNum[i]);
    }
    return occupiedGpio;
}
