#ifndef IPSET_INTERFACE_H
#define IPSET_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <QProcess>

#define WAIT_SECOND 1.1

double getspeed();
long int getCurrentDownloadRates(long int * save_rate);
QString getipconfig();
QString getallip();
QString addstaticip(QString ipname,QString netcard,QString ipaddr);
QString modstaticip(QString ipname,QString ipaddr);
QString delstaticip(QString ipname);
QString setdip(QString netcard);

#endif // IPSET_INTERFACE_H
