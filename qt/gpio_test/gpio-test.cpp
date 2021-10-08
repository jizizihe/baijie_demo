#include "gpio-test.h"

QString num_cal(char port,QString num)
{
    QString port_num;
    switch (port)
    {
        case 'A':
        case 'a':
            port_num = num;
            break;
        case 'B':
        case 'b':
            port_num = num + 32;
            break;
        case 'C':
        case 'c':
            port_num = num + 64;
            break;
        case 'D':
        case 'd':
            port_num = num + 96;
            break;
        case 'E':
        case 'e':
            port_num = num + 128;
            break;
        case 'F':
        case 'f':
            port_num = num + 160;
            break;
        case 'G':
        case 'g':
            port_num = num + 192;
            break;
        case 'H':
        case 'h':
            port_num = num + 224;
            break;
        case 'I':
        case 'i':
            port_num = num + 256;
            break;
        case 'J':
        case 'j':
            port_num = num + 288;
            break;
        case 'K:
        case 'k':
            port_num = num + 320;
            break;
        case 'L':
        case 'l':
            port_num = num + 352;
            break;
        default:
            port_num = 0;
            break;
    }
    return port_num;
}
