#ifndef UDEV_INTERFACE_H
#define UDEV_INTERFACE_H


#include <QString>
#include <QProcess>
#include <QFile>
#include <QDebug>

QString get_new_usb();
QString get_new_sd();
QString get_new_sim();
QString get_gateway();
QString ping_gateway();
QString wifi_test();
QString bluetooth();


#endif // UDEV_INTERFACE_H
