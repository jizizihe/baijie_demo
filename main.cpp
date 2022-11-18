#include <QApplication>
#include <QTextCodec>
#include <QDesktopWidget>
#include "language_selection.h"
#include "database.h"

#include<stdio.h>
#include<sys/ioctl.h>
#include<linux/fb.h>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("tgtsml"));

    int fd;
    struct fb_var_screeninfo screen_info;
    fd = open("/dev/fb0",O_RDWR);
    ioctl(fd,FBIOGET_VSCREENINFO,&screen_info);
    close(fd);
    if(screen_info.xres < screen_info.yres)
    {
        setenv("QT_QPA_EGLFS_ROTATION","90",1);
        setenv("QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS","/dev/input/event5:rotate=90",1);
    }

    globalApp a(argc,argv);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    language_selection languageSelectionWg;
    database databaseWg;

    QStringList list = databaseWg.tableShow("language");
    if(!list.isEmpty())
    {
       languageSelectionWg.desktopShow();
    }
    else
    {
        languageSelectionWg.show();
    }
    return a.exec();
}
