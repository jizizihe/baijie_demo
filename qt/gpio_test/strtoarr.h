#ifndef __STRTOARR_H_
#define __STRTOARR_H_

#include <stdio.h>

typedef struct temp_frame_s
{
	char gpio_sign;
    int gpio_num;
}frame_s;

extern char hexdec(char str);
extern frame_s str_to_arr(const char *str);

#endif
