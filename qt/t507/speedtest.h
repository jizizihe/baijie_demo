#ifndef SPEEDTEST_H
#define SPEEDTEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WAIT_SECOND 3

float getspeed(float CurrentSpeed);
long int getCurrentDownloadRates(long int * save_rate);

#endif // SPEEDTEST_H
