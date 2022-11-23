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
bool isStaticIpExist();
QString getNetworkInfo();
QString getCurrentIp(QString name);
QString executeLinuxCmd(QString strCmd);
void networkEnable(bool flag);

#endif // IPSET_INTERFACE_H
