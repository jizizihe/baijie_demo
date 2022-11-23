#include <QApplication>
#include <QTextCodec>
#include <QProcess>
#include "language_selection.h"
#include "database.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

void setCapacitorScreenTouchEvn();

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("tgtsml"));

    int fd;
    struct fb_var_screeninfo screenInfo;
    fd = open("/dev/fb0",O_RDWR);
    ioctl(fd,FBIOGET_VSCREENINFO,&screenInfo);          // Get screen resolution
    close(fd);
    if(screenInfo.xres < screenInfo.yres)
    {
        QString strResult = executeLinuxCmd("cat /sys/class/input/input*/name");
        if(strResult.contains("gt9xxnew_ts",Qt::CaseInsensitive))     // Determine whether the screen is a resistor or a capacitor
        {
            setenv("QT_QPA_EGLFS_ROTATION","90",1);                   // Rotate screen coordinates
            setCapacitorScreenTouchEvn();                             // Rotate touch coordinate
        }
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

void setCapacitorScreenTouchEvn()
{
    QStringList listEvent;
    listEvent<< "#!/bin/sh  \n"
             << "for InputDevices in /sys/class/input/input*  \n"
             << "do  \n"
             << "event_value=`ls $InputDevices/ | grep 'event*' | awk 'NR==1{print}'` \n"
             << "cat $InputDevices/name 2>/dev/null | grep gt9xxnew_ts  \n"
             << "if [ $? -eq '0' ]; then  \n"
             << "echo $event_value  \n"
             << "fi  \n"
             << "done  \n"
             <<  "!  \n";
    QFile file("/gettouchevent.sh");
    if (file.exists())
    {
        file.remove();
    }

    if(file.open( QIODevice::WriteOnly  ))
    {
        for(int i = 0; i< listEvent.size();++i)
        {
            QString tmp = listEvent.at(i);
            char * data = tmp.toLatin1().data();
            file.write(data);
        }
    }
    file.close();
    executeLinuxCmd("chmod +x /gettouchevent.sh");
    QString strResult = executeLinuxCmd("/gettouchevent.sh ");
    QStringList list = strResult.split("\n");
    QString eventValue = list.at(1);
    QString str = QString("/dev/input/%1:rotate=90").arg(eventValue);
    setenv("QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS",str.toUtf8(),1);     // Rotating touch coordinates
    executeLinuxCmd("rm /gettouchevent.sh");
}
