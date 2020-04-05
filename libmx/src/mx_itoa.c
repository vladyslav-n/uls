#include "../inc/libmx.h"

static short int numlen(int n) {
    short int len = 1;

    if (n < 0) {
        len++;
    }
    while (n /= 10) {
        len++;
    }
    return len;
}

char *mx_itoa(int number) {
    int len = 0;
    char *str = NULL;

    if (number == -2147483648)
        return "-2147483648";
    if (!number)
        return "0";
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
