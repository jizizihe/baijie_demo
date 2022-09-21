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
bool add_static_ip(QString netname,QString ipaddr,QString mask,QString gate);
bool modify_static_ip(QString ipaddr);
bool delete_static_ip();
bool automatically_get_ip();
QString get_network_info();
QString get_current_ip(QString Name);
bool is_static_ip_exist();
void network_enable(bool flag);
QString executeLinuxCmd(QString strCmd);

#endif // IPSET_INTERFACE_H
