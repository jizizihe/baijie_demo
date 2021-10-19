#include "speedtest.h"
//#include <QDebug>


long int getCurrentDownloadRates(long int * save_rate)
{
    FILE * net_dev_file;
    char buffer[1024];
    size_t bytes_read;
    char * match;
    if ( (net_dev_file=fopen("/proc/net/dev", "r")) == NULL )
    {
        printf("open file /proc/net/dev/ error!\n");
        return -1;
    }
    bytes_read = fread(buffer, 1, sizeof(buffer), net_dev_file);
    fclose(net_dev_file);
    if ( bytes_read == 0 )
    {
        return -1;
    }
    buffer[bytes_read] = '\0';
    match = strstr(buffer, "eth0:");
    if ( match == NULL )
    {
        printf("no eth0 keyword to find!\n");
        return -1;
    }
    sscanf(match, "eth0:%ld", save_rate);
    return *save_rate;
}

float getspeed(float speed)
{
    long int start_download_rates;
    long int end_download_rates;
    //float speed;
    speed = 0;
    //printf("-------------\n");
    //while(1)
	{
        if(getCurrentDownloadRates(&start_download_rates) == -1)
        {
            return -1;
        }
        sleep(WAIT_SECOND);
        if(getCurrentDownloadRates(&end_download_rates) == -1)
        {
            return -1;
        }
        speed = (float)(end_download_rates-start_download_rates)/WAIT_SECOND;
        //printf("download is : %.2lf Bytes/s\n", speed );
	}
    return speed;
}


