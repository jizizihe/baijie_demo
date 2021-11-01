#include "boardinfo_interface.h"
#include <QProcess>
#include <QString>
#include <QDebug>
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

    status = (char *)malloc(sizeof(char) * 255);

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

    name = (char *)malloc(sizeof(char) * 255);

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

int get_QTversion(char *dirpath,char *filename,char *fs_file)
{
    int i = 0,j = 0;
    char fw_file[64][64];
    char finish_file[64] = {0};

    DIR *dir = opendir(dirpath);
    if(dir == NULL) {
        qDebug("open %s failed \n",dirpath);
        return 0;
    }

    struct dirent *ent;
    while((ent = readdir(dir)) != NULL)
    {
        if(strncmp(ent->d_name,filename ,9) == 0)
        {
            sprintf(fw_file[i],"%s",ent->d_name);
//            qDebug("fw: %s\n",fw_file[i]);
            i++;
        }
    }

//        qDebug("i = %d\n",i);
    closedir(dir);

    for(j = 0;j < i;j++)
    {
        if(strcmp(finish_file,fw_file[j]) < 0)
        {
            strcpy(finish_file,fw_file[j]);
        }
    }
//    qDebug("finish_file is %s\n",finish_file);

    sscanf(finish_file,"%*[^.]%*[^1-9]%s",fs_file);
//    qDebug("fs_file is %s\n",fs_file);
    return i;

}

char *get_kernel()
{
    char *kernel;

    kernel = (char *)malloc(255);
    QProcess *process = new QProcess();
    process->start("uname -m");
    if(!process->waitForFinished())
    {
//        ui->kernel_label->setText(QString(tr("kernel: Unknown")));
        kernel = "Unknown";
        return kernel;
    }
    QString hardware_name = QString::fromLocal8Bit(process->readAllStandardOutput());
    hardware_name = hardware_name.simplified();

    process->start("uname -s");
    if(!process->waitForFinished())
    {
//        ui->kernel_label->setText(QString(tr("kernel: Unknown")));
        kernel = "Unknown";
        return kernel;
    }
    QString kernel_name = QString::fromLocal8Bit(process->readAllStandardOutput());
    kernel_name = kernel_name.simplified();

    process->start("uname -r");
    if(!process->waitForFinished())
    {
//        ui->kernel_label->setText(QString(tr("kernel: Unknown")));
        kernel = "Unknown";
        return kernel;
    }
    QString kernel_release = QString::fromLocal8Bit(process->readAllStandardOutput());
    kernel_release = kernel_release.simplified();

    kernel = (QString("%1 %2 %3").arg(hardware_name).arg(kernel_name).arg(kernel_release)).toLatin1().data();
    return kernel;
}
