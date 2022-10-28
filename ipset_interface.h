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

bool addStaticIp(QString netName,QString ipAddr,QString mask,QString gate);
bool modifyStaticIp(QString ipAddr);
bool deleteStaticIp();
bool automaticallyGetIp();
QString getNetworkInfo();
QString getCurrentIp(QString name);
bool isStaticIpExist();
void networkEnable(bool flag);
QString executeLinuxCmd(QString strCmd);

#endif // IPSET_INTERFACE_H
