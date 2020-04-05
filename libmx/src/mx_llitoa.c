#include "../inc/libmx.h"

static short int numlen(long long n) {
    short int len = 1;

    if (n < 0) {
        len++;
    }
    while (n /= 10) {
        len++;
    }
    return len;
}

char *mx_llitoa(long long number) {
    int len = 0;
    char *str = NULL;

    if (!number)
        return mx_strdup("0");
    len = numlen(number);
    str = (char*) malloc(sizeof(char) * (len + 1));
    str[len] = 0;
    if (number < 0) {
        str[0] = '-';
        number = -number;
    }
    for (int i = 0; number; i++) {
        str[len - 1 - i] = number % 10 + '0';
        number /= 10;
    }
    return str;
}
