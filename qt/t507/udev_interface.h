#ifndef UDEV_INTERFACE_H
#define UDEV_INTERFACE_H


#include <QString>
#include <QProcess>
#include <QFile>
#include <QDebug>

QString get_new_usb();
QString get_new_sd();
QString get_new_sim();


#endif // UDEV_INTERFACE_H
