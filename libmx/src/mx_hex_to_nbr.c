#include "../inc/libmx.h"

static char hexchar(char c) {
    if (mx_isdigit(c))
        return c;
    if ('a' == c || 'A' == c)
        return 58;
    if ('b' == c || 'B' == c)
        return 59;
    if ('c' == c || 'C' == c)
        return 60;
    if ('d' == c || 'D' == c)
        return 61;
    if ('e' == c || 'E' == c)
        return 62;
    if ('f' == c || 'F' == c)
        return 63;
    else
        return 0;
}

unsigned long mx_hex_to_nbr(const char *hex) {
    unsigned long num = 0;
    unsigned long i = 0;

    if (!hex)
        return 0;
    while (hexchar(hex[i])) {
        num = num * 16 + hexchar(hex[i]) - 48;
        i++;
    }
    return num;
}
