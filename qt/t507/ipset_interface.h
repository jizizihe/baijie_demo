#ifndef IPSET_INTERFACE_H
#define IPSET_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <QProcess>

#define WAIT_SECOND 3

float getspeed();
long int getCurrentDownloadRates(long int * save_rate);
QString getipconfig();
QString getallip();
QString setsip(QString ipname,QString netcard,QString ipaddr,QString gateway);
QString addstaticip(QString ipname,QString netcard,QString ipaddr,QString gateway,bool flag);
QString modstaticip(QString ipname,QString ipaddr,QString gateway,bool flag);
QString delstaticip(QString ipname);
QString setdip(QString netcard);

#endif // IPSET_INTERFACE_H
