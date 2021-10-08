#include "strtoarr.h"

char hexdec(char str)
{
    char temp = 0;
	if ((str >= '0') && (str <= '9')) //当前字符为数字0~9时
		temp = str - '0' ;//转为十六进制

	if ((str >= 'A') && (str <= 'Z')) //当前字符为大写字母A~Z时
		temp = str - '7';//转为十六进制

	if ((str >= 'a') && (str <= 'z')) //当前字符为小写字母a~z时
		temp = str - 'W';  //转为十六进制

    return temp;
}

frame_s str_to_arr(const char *str)
{
    frame_s frame;
	
    frame.gpio_sign = hexdec(*str) << 4;
    frame.gpio_num = (int)hexdec(*(str+1));
	
	return frame;
}


