#include "udev_interface.h"

//#define DD
#ifdef DD
#define qdebug(format, ...)  qDebug("function:%s\tline:%s\t " format, __func__,__LINE__,##__VA_ARGS__);
#else
#define qdebug(format, ...)
#endif


QString get_new_usb()
{
    qDebug() << "in get_new_usb";
    bool flag = false;
    QByteArray array_data[10];

    QString failed = QObject::tr("USB Failed");
    QString file_name = QString("/data/1.txt");

    QProcess *proc = new QProcess();
    proc->start(QString("rm %1").arg(file_name));;
    proc->waitForFinished();

    proc->start(QString("touch %1").arg(file_name));
    proc->waitForFinished();

    QString str = QString("lsusb > %1").arg(file_name);
    proc->start("bash",QStringList() << "-c" << str);
    proc->waitForFinished();

    QFile file(file_name);
    file.open(QIODevice::ReadOnly);
    int i;
    for(i = 0; !file.atEnd();++i)
    {
        array_data[i] = file.readLine();
//        qDebug() <<__LINE__ <<array_data[i];
    }

    QByteArray temp;
    for(int x = 0; x < i; x++)
    {
        temp = array_data[x];
        array_data[x] = array_data[x].remove(0,28).left(4);
        if(array_data[x] == "0001" || array_data[x] == "0125" || array_data[x] == "0101" || array_data[x] == "0002" || array_data[x] == "772b")
        {
            flag = false;
        }
        else
        {
            flag = true;
            break;
        }
    }

    file.flush();
    file.close();

    proc->close();
    delete proc;
    proc = 0;

    if(flag)
    {
//        qDebug() << "*****usb******true: " << temp.data();
        qdebug("*****usb******true : %s",temp.data());
        return temp.data();
    }
    else
    {
//        qDebug() <<"*****usb******false: " << failed.data();
        qdebug("*****usb******flase: %s",failed.data());
        return failed;
    }

}

QString get_new_sd()
{
    QString failed = QObject::tr("SD Failed");
    QString sd_str = QString("fdisk -l | grep /dev/mmcblk1p1");
    QProcess sd_proc;
    sd_proc.start("bash",QStringList() << "-c" << sd_str);
    sd_proc.waitForFinished();
    QString status = sd_proc.readAllStandardOutput().data();
    if(status == NULL)
    {
        qdebug("*****sd****** false: %s",failed.data());
        return failed;
    }
    else
    {
        qdebug("*****sd****** true: %s",status.data());
        return status;
    }


}

QString get_new_sim()
{
    QString failed = QObject::tr("SIM Failed");
    QString sd_str = QString("ifconfig | grep ppp0");
    QProcess sd_proc;
    sd_proc.start("bash",QStringList() << "-c" << sd_str);
    sd_proc.waitForFinished();
    QString status = sd_proc.readAllStandardOutput().data();
    if(status == NULL)
    {
        qdebug("*****sim****** false: %s",failed.data());
        return failed;
    }
    else
    {
        qdebug("*****sim****** true: %s",status.data());
        return status;
    }
}
