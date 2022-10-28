#include "boardinfo_interface.h"
#include <QProcess>
#include <QString>
#include <QDebug>
int getBatteryLevel()
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
    fclose(fd);
    return level;
}

char *getBatteryStatus()
{
    FILE *fd;
    char *status;
    fd = fopen(BATTERY_PATH"/status", "r");
    if(NULL == fd)
    {
        printf("open battery status failed !\n");
        return (char *)"false";
    }
    status = (char *)malloc(sizeof(char) * 255);

    fscanf(fd, "%s", status);
    fclose(fd);
    return status;
}

float getCPUTemp()
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

int getResolution(int *x,int *y)
{
    int fd;
    struct fb_var_screeninfo screenInfo;
    char *path = (char *)"/dev/fb0";
    fd = open(path,O_RDWR);
    if(0 == fd)
    {
        printf("open fb0 failed !\n");
        return -1;
    }
    ioctl(fd,FBIOGET_VSCREENINFO,&screenInfo);
    *x = screenInfo.xres;
    *y = screenInfo.yres;
    close(fd);
    return 0;
}

char *getBoardName()
{
    FILE *fd;
    char *name;

    fd = fopen("/etc/hostname", "r");
    if(NULL == fd)
    {
        printf("open battery status failed !\n");
        return (char *)"false";
    }
    name = (char *)malloc(sizeof(char) * 255);

    fscanf(fd, "%s", name);
    fclose(fd);
    return name;
}

char *getOSName()
{
    FILE *fd;
    char *OS;
    char *path = (char *)"/etc/issue";
    fd = fopen(path,"r");
    if(NULL == fd)
    {
        printf("open %s failed !\n",path);
        return (char *)"false";
    }
    OS = (char *)malloc(sizeof(char) * 255);

    fscanf(fd, "%[^\n]", OS);
    QString str = QString::fromUtf8(OS);
    str.remove("\\n");
    str.remove("\\l");
    QByteArray ba = str.toLatin1();
    OS=ba.data();

    fclose(fd);
    return OS;
}

int getQTVersion(char *dirPath, char *fileName, char *fsFile)
{
    int i = 0,j = 0;
    char fw_file[64][64];
    char finish_file[64] = {0};

    DIR *dir = opendir(dirPath);
    if(dir == NULL) {
        qDebug("open %s failed \n",dirPath);
        return 0;
    }

    struct dirent *ent;
    while((ent = readdir(dir)) != NULL)
    {
        if(strncmp(ent->d_name,fileName ,9) == 0)
        {
            sprintf(fw_file[i],"%s",ent->d_name);
            i++;
        }
    }

    closedir(dir);
    for(j = 0;j < i;j++)
    {
        if(strcmp(finish_file,fw_file[j]) < 0)
        {
            strcpy(finish_file,fw_file[j]);
        }
    }

    sscanf(finish_file,"%*[^.]%*[^1-9]%s",fsFile);
    return i;
}

char *getKernel()
{
    char *kernel;
    kernel = (char *)malloc(255);
    QProcess *process = new QProcess();
    process->start("uname -m");
    if(!process->waitForFinished())
    {
        kernel = (char *)"Unknown";
        return kernel;
    }
    QString hardwareName = QString::fromLocal8Bit(process->readAllStandardOutput());
    hardwareName = hardwareName.simplified();

    process->start("uname -s");
    if(!process->waitForFinished())
    {
        kernel = (char *)"Unknown";
        return kernel;
    }
    QString kernelName = QString::fromLocal8Bit(process->readAllStandardOutput());
    kernelName = kernelName.simplified();

    process->start("uname -r");
    if(!process->waitForFinished())
    {
        kernel = (char *)"Unknown";
        return kernel;
    }
    QString kernelRelease = QString::fromLocal8Bit(process->readAllStandardOutput());process->close();
    kernelRelease = kernelRelease.simplified();
    kernel = (QString("%1 %2 %3").arg(hardwareName).arg(kernelName).arg(kernelRelease)).toLatin1().data();
    return kernel;
}
