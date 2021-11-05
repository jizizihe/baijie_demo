#ifndef IPSET_INTERFACE_H
#define IPSET_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QtNetwork>


QString getipconfig();
QString getallip();
QString addstaticip(QString netcard,QString ipaddr);
QString modstaticip(QString ipaddr);
QString delstaticip();
QString networkup();
QString getifconfig();
QString GetCurrentIp(QString Name);
QString is_staticip();

#endif // IPSET_INTERFACE_H
